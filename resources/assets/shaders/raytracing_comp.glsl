#version 460

layout(local_size_x = 8, local_size_y = 8, local_size_z = 1) in;
layout(rgba32f, binding = 0) uniform image2D resultImage;

/*
  Structs
*/

// Primitives
struct Material
{
    vec3 color;
    float smoothness;

    vec3 specularColor;
    float specularProbability;

    vec3 emissionColor;
    float emissionStrenght;

    int type;
};

struct Sphere
{
    vec3 pos;
    float radius;

    Material material;
};

struct Box
{
    vec3 boxMin, boxMax;

    Material material;
};

struct Triangle
{
    vec3 posA, posB, posC;
    vec3 normalA, normalB, normalC;
};

// BVH
struct BVHNode
{
    vec3 boxMin, boxMax;

    int leftChildIdx;
    int triIndex, triCount;
};

struct TriangleMesh
{
    int rootBVHNodeIndex;
    mat4 localToWorld, worldToLocal;

    Material material;
};

// Rays
struct Ray
{
    vec3 origin;
    vec3 dir, invDir;
};

struct HitInfo
{
    bool didHit;
    float dst;
    vec3 hitPos, normal;

    Material material;
};

struct TriHitInfo
{
    bool didHit;
    float dst;
    vec3 hitPos, normal;
};

/*
  IO
*/

uniform uint frameIndex; // Index of the frame currently being rendered
uniform vec3 viewParams; // planeWidth, planeHeight, focalLength;
uniform mat4 localToWorld; // Camera localToWorld matrix
uniform int drawDebugMode;
uniform vec2 statsThresholds;

uniform bool accumulate; // Should accumulate the result with the previous frame?
uniform int maxBounces; // Max number of bounces for a given ray
uniform int raysPerPixel; // Number of rays to shoot per pixel, makes the editor less noisy TODO: Multiple Importance Sampling
uniform bool environmentLight; // Should the ray gather light from the environment (=fake skybox) if it doesn't hit anything?
uniform float divergeStrength; // Blur used for a rudimentary AA, or whole scene blurring
uniform float defocusStrength; // Blur used for DOF. change the focal length parameter to focus on different parts of the scene

layout(std430, binding = 0) readonly buffer SphereBuffer {
    Sphere spheres[];
};

layout(std430, binding = 1) readonly buffer BoxBuffer {
    Box boxes[];
};

layout(std430, binding = 2) readonly buffer TriangleBuffer {
    Triangle triangles[];
};

layout(std430, binding = 3) readonly buffer BVHNodeBuffer {
    BVHNode nodes[];
};

layout(std430, binding = 4) readonly buffer MeshesBuffer {
    TriangleMesh meshes[];
};

const float INFINITY = 1.0 / 0.0;
const float EPSILON = 0.0000001;
const float PI = 3.1415926;
const float TAU = PI * 2;

#define BVH_TARGET_DEPTH 32

/*
  Random Generation
*/

// Permuted congruential generator, thanks to https://observablehq.com/@riccardoscalco/pcg-random-number-generators-in-glsl
float rand(inout uint rngState)
{
    rngState = rngState * 747796405 + 2891336453;
    uint result = ((rngState >> ((rngState >> 28) + 4)) ^ rngState) * 277803737;
    result = (result >> 22) ^ result;
    return result / 4294967295.0;
}

// Cosine weighted distribution
float randNormalDistribution(inout uint rngState)
{
    float theta = TAU * rand(rngState);
    return sqrt(-2 * log(rand(rngState))) * cos(theta);
}

vec3 randDir(inout uint rngState)
{
    float x = randNormalDistribution(rngState);
    float y = randNormalDistribution(rngState);
    float z = randNormalDistribution(rngState);
    return normalize(vec3(x, y, z));
}

vec2 randPointInCircle(inout uint rngState)
{
    float theta = TAU * rand(rngState);
    vec2 point = vec2(cos(theta), sin(theta));

    return point * sqrt(rand(rngState));
}

/*
  Intersections
*/

// Thanks to: https://raytracing.github.io/books/RayTracingInOneWeekend.html#surfacenormalsandmultipleobjects/simplifyingtheray-sphereintersectioncode
HitInfo intersectSphere(Ray ray, vec3 pos, float radius)
{
    HitInfo result;
    result.didHit = false;

    vec3 oc = pos - ray.origin;

    float a = dot(ray.dir, ray.dir);
    float h = dot(ray.dir, oc);
    float c = dot(oc, oc) - radius * radius;

    float discreminant = h * h - a * c;

    if(discreminant >= 0)
    {
        float dst = (h - sqrt(discreminant)) / a;

        if(dst >= EPSILON)
        {
            result.didHit = true;
            result.dst = dst;
            result.hitPos = ray.origin + ray.dir * dst;
            result.normal = normalize(result.hitPos - pos);
        }
    }

    return result;
}

// Möller–Trumbore intersection algorithm, thanks to https://cadxfem.org/inf/Fast%20MinimumStorage%20RayTriangle%20Intersection.pdf
TriHitInfo intersectTriangle(Ray ray, Triangle tri)
{
    TriHitInfo result;
    result.didHit = false;

    vec3 edge1 = tri.posB - tri.posA;
    vec3 edge2 = tri.posC - tri.posA;
    vec3 pvec = cross(ray.dir, edge2);

    float det = dot(edge1, pvec);
    if(det < EPSILON)
        return result;

    vec3 tvec = ray.origin - tri.posA;
    float u = dot(tvec, pvec);
    if(u < 0 || u > det)
        return result;

    vec3 qvec = cross(tvec, edge1);
    float v = dot(ray.dir, qvec);
    if(v < 0 || u + v > det)
        return result;

    float invDet = 1.0 / det;
    float t = dot(edge2, qvec) * invDet;

    u *= invDet;
    v *= invDet;
    float w = 1 - u - v;

    result.didHit = true;
    result.dst = t;
    result.hitPos = ray.origin + ray.dir * t;
    result.normal = normalize(tri.normalA * w + tri.normalB * u + tri.normalC * v);
    return result;
}

// Slab Method AABB intersection. Thanks to: https://tavianator.com/2022/ray_box_boundary.html
float intersectAABB(Ray ray, vec3 boxMin, vec3 boxMax)
{
    vec3 tMin = (boxMin - ray.origin) * ray.invDir;
    vec3 tMax = (boxMax - ray.origin) * ray.invDir;

    vec3 t1 = min(tMin, tMax);
    vec3 t2 = max(tMin, tMax);

    float tNear = max(max(t1.x, t1.y), t1.z);
    float tFar = min(min(t2.x, t2.y), t2.z);

    return tNear <= tFar && tFar > 0 ? tNear : INFINITY;
}

/*
  Shader
*/

TriHitInfo traverseBVH(Ray localRay, int rootNodeIndex, inout vec2 stats)
{
    TriHitInfo result;
    result.didHit = false;
    result.dst = INFINITY;

    int stack[BVH_TARGET_DEPTH];
    int stackPtr = 0;
    stack[stackPtr++] = rootNodeIndex;

    while(stackPtr > 0)
    {
        BVHNode node = nodes[stack[--stackPtr]];

        if(node.triCount > 0)
        {
            stats.y += node.triCount;

            for(int i = node.triIndex; i < node.triIndex + node.triCount; i++)
            {
                TriHitInfo intersection = intersectTriangle(localRay, triangles[i]);
                if(!intersection.didHit || intersection.dst >= result.dst)
                    continue;

                result = intersection;
            }
        }
        else
        {
            int leftChildIdx = node.leftChildIdx;
            int rightChildIdx = node.leftChildIdx + 1;
            BVHNode leftChild = nodes[leftChildIdx];
            BVHNode rightChild = nodes[rightChildIdx];

            float dstLeft = intersectAABB(localRay, leftChild.boxMin, leftChild.boxMax);
            float dstRight = intersectAABB(localRay, rightChild.boxMin, rightChild.boxMax);
            stats.x += 2;

            if(max(dstLeft, dstRight) < result.dst)
                stack[stackPtr++] = dstLeft < dstRight ? rightChildIdx : leftChildIdx;

            if(min(dstLeft, dstRight) < result.dst)
                stack[stackPtr++] = dstLeft < dstRight ? leftChildIdx : rightChildIdx;
        }
    }

    return result;
}

HitInfo intersectScene(Ray ray, inout vec2 stats)
{
    HitInfo result;
    result.didHit = false;
    result.dst = INFINITY;

    //Spheres
    for(int i = 0; i < spheres.length(); i++)
    {
        Sphere sphere = spheres[i];

        HitInfo intersection = intersectSphere(ray, sphere.pos, sphere.radius);
        if(!intersection.didHit || intersection.dst >= result.dst)
            continue;

        result = intersection;
        result.material = sphere.material;
    }

    //Boxes
    for(int i = 0; i < boxes.length(); i++)
    {
        Box box = boxes[i];

        float dst = intersectAABB(ray, box.boxMin, box.boxMax);
        if(dst <= 0 || dst >= result.dst)
            continue;

        result.didHit = true;
        result.dst = dst;
        result.hitPos = ray.origin + ray.dir * dst;
        result.normal = -sign(ray.dir) * step(vec3(dst), min((box.boxMin - ray.origin) * ray.invDir, (box.boxMax - ray.origin) * ray.invDir));
        result.material = box.material;

        // Checkerboard Material
        if(result.material.type == 1)
        {
            vec2 checkerboard = mod(floor(result.hitPos.xz), 2);
            if(checkerboard.x == checkerboard.y)
                result.material.color = result.material.emissionColor;
        }
    }

    // Meshes
    for(int i = 0; i < meshes.length(); i++)
    {
        TriangleMesh mesh = meshes[i];

        Ray localRay;
        localRay.origin = (mesh.worldToLocal * vec4(ray.origin, 1)).xyz;
        localRay.dir = (mesh.worldToLocal * vec4(ray.dir, 0)).xyz;
        localRay.invDir = 1.0 / localRay.dir;

        TriHitInfo intersection = traverseBVH(localRay, mesh.rootBVHNodeIndex, stats);
        if(!intersection.didHit || intersection.dst >= result.dst)
            continue;

        result.didHit = true;
        result.dst = intersection.dst;
        result.hitPos = ray.origin + ray.dir * intersection.dst;
        result.normal = normalize((mesh.localToWorld * vec4(intersection.normal, 0)).xyz);
        result.material = mesh.material;
    }

    return result;
}

// Simple fake sky box TODO: add sun dir
vec3 calculateEnvironmentLight(Ray ray)
{
    if(!environmentLight)
        return vec3(0);

    float verticalPos = clamp(ray.dir.y, 0, 1);
    vec3 skyColor = mix(vec3(0.5, 0.7, 1.0), vec3(0.1, 0.3, 0.8), verticalPos);

    float haze = pow(1.0 - verticalPos, 4.0);
    skyColor = mix(skyColor, vec3(0.8, 0.8, 0.9), haze);

    return skyColor * vec3(1.0, 0.9, 0.7);
}

// Main raytracing function, shoot the ray and gather the color it brings back
vec3 traceRay(Ray ray, inout uint rngState, inout vec2 stats)
{
    vec3 outColor = vec3(0);
    vec3 rayColor = vec3(1);

    for(int i = 0; i <= maxBounces; i++)
    {
        HitInfo intersection = intersectScene(ray, stats);
        if(!intersection.didHit) {
            outColor += calculateEnvironmentLight(ray) * rayColor;
            break;
        }

        if(drawDebugMode == 4)
        {
            outColor = intersection.normal * 0.5 + 0.5;
            break;
        }

        //Calculate lighting
        Material material = intersection.material;
        vec3 emittedLight = material.emissionColor * material.emissionStrenght;

        bool isSpecularBounce = bool(rand(rngState) <= material.specularProbability);

        outColor += emittedLight * rayColor;
        rayColor *= mix(material.color, material.specularColor, isSpecularBounce);

        //Bounce ray
        ray.origin = intersection.hitPos + intersection.normal * EPSILON;

        vec3 diffuseDir = normalize(intersection.normal + randDir(rngState));
        vec3 specularDir = reflect(ray.dir, intersection.normal);

        ray.dir = mix(diffuseDir, specularDir, isSpecularBounce ? material.smoothness : 0);
        ray.invDir = 1.0 / ray.dir;
    }

    return outColor;
}

vec3 colorizeStats(vec2 stats)
{
    stats /= statsThresholds;

    switch(drawDebugMode)
    {
        case 1: // Box Tests Count
            if(stats.x >= 1)
                return vec3(1, 0, 0);

            return vec3(stats.x);
        case 2: // Tri Tests Count
            if(stats.y >= 1)
                return vec3(1, 0, 0);

            return vec3(stats.y);
        case 3: // Box+Tri Tests Count
            if(max(stats.x, stats.y) >= 1)
                return vec3(1);

            return vec3(stats.x, 0, stats.y);
        default:
            return vec3(0);
    }
}

void main()
{
    ivec2 pixelCoords = ivec2(gl_GlobalInvocationID.xy);
    ivec2 screenSize = imageSize(resultImage);

    // Generate Random Seed
    uint rngState = uint(pixelCoords.y * screenSize.x + pixelCoords.x + (frameIndex * 719393 * uint(accumulate))); // 719393 is a prime number used to to recreate the seed between frames

    // Calculate viewpoint
    vec3 viewPointLocal = vec3(vec2(pixelCoords) / screenSize - 0.5, 1) * viewParams;
    vec3 viewPoint = vec4(localToWorld * vec4(viewPointLocal, 1)).xyz;

    vec3 camRight = localToWorld[0].xyz;
    vec3 camUp = localToWorld[1].xyz;
    vec3 camPos = localToWorld[3].xyz;

    // Shoot ray and average color
    vec3 totalLight = vec3(0);
    vec2 stats = vec2(0);

    for(int i = 0; i < raysPerPixel; i++)
    {
        // Defocus Jitter (used for DOF)
        vec2 defocusJitter = randPointInCircle(rngState) * defocusStrength / screenSize.x;
        vec3 jitteredOrigin = camPos + defocusJitter.x * camRight + defocusJitter.y * camUp;

        // Diverge Jitter (used for AA or whole scene blur)
        vec2 divergeJitter = randPointInCircle(rngState) * divergeStrength / screenSize.x;
        vec3 jitteredViewPoint = viewPoint + divergeJitter.x * camRight + divergeJitter.y * camUp;

        // Create Ray
        Ray ray;
        ray.origin = jitteredOrigin;
        ray.dir = normalize(jitteredViewPoint - ray.origin);
        ray.invDir = 1.0 / ray.dir;

        // Shoot Ray
        totalLight += traceRay(ray, rngState, stats);
    }

    // Accumulate
    vec3 finalColor = totalLight / raysPerPixel + colorizeStats(stats);
    if(accumulate)
    {
        vec3 accumulatedColor = imageLoad(resultImage, pixelCoords).rgb;
        finalColor = mix(accumulatedColor, finalColor, 1.0 / frameIndex);
    }

    imageStore(resultImage, pixelCoords, vec4(finalColor, 1));
}
