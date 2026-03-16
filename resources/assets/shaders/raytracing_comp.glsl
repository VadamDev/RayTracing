#version 460

layout(local_size_x = 8, local_size_y = 8, local_size_z = 1) in;

layout(rgba32f, binding = 0) uniform image2D resultImage;

/*
  Structs
*/

// Raytraced Objects
struct Material
{
    vec3 color, emissionColor;
    float emissionStrenght;

    float smoothness;
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

struct TriangleMesh
{
    int triangleIndex;
    int numTriangles;

    vec3 boxMin, boxMax;

    Material material;
};

// Rays management
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

/*
  IO
*/

uniform uint frameIndex;
uniform vec3 viewParams; // planeWidth, planeHeight, focalLength;
uniform vec3 cameraPos;

uniform mat4 localToWorld;
uniform bool accumulate;
uniform int maxBounces;
uniform int raysPerPixel;
uniform bool environmentLight;

layout(std430, binding = 0) buffer SphereBuffer {
    Sphere spheres[];
};

layout(std430, binding = 1) buffer BoxBuffer {
    Box boxes[];
};

layout(std430, binding = 1) buffer TriangleBuffer {
    Triangle triangles[];
};

layout(std430, binding = 1) buffer MeshesBuffer {
    TriangleMesh meshes[];
};

const float INFINITY = 1.0 / 0.0;
const float EPSILON = 0.00001;
const float PI = 3.1415926;
const float TAU = PI * 2;

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

vec3 randomDir(inout uint rngState)
{
    float x = randNormalDistribution(rngState);
    float y = randNormalDistribution(rngState);
    float z = randNormalDistribution(rngState);
    return normalize(vec3(x, y, z));
}

/*
  Intersections
*/

// Thanks to: https://raytracing.github.io/books/RayTracingInOneWeekend.html#surfacenormalsandmultipleobjects/simplifyingtheray-sphereintersectioncode
HitInfo intersectSphere(Ray ray, Sphere sphere)
{
    HitInfo result;
    result.didHit = false;

    vec3 oc = sphere.pos - ray.origin;

    float a = dot(ray.dir, ray.dir);
    float h = dot(ray.dir, oc);
    float c = dot(oc, oc) - sphere.radius * sphere.radius;

    float discreminant = h * h - a * c;

    if(discreminant >= 0)
    {
        float dst = (h - sqrt(discreminant)) / a;

        if(dst >= 0)
        {
            result.didHit = true;
            result.dst = dst;
            result.hitPos = ray.origin + ray.dir * dst;
            result.normal = normalize(result.hitPos - sphere.pos);
        }
    }

    return result;
}

// Möller–Trumbore intersection algorithm, thanks to https://youtu.be/Qz0KTGYJtUk
HitInfo intersectTriangle(Ray ray, Triangle tri)
{
    HitInfo result;
    result.didHit = false;

    vec3 edgeAB = tri.posB - tri.posA;
    vec3 edgeAC = tri.posC - tri.posA;
    vec3 normal = cross(edgeAB, edgeAC);

    float discreminant = -dot(ray.dir, normal);
    if(discreminant >= EPSILON)
    {
        float invDiscreminant = 1.0 / discreminant;

        vec3 ao = ray.origin - tri.posA;
        vec3 dao = cross(ao, ray.dir);

        float dst = dot(ao, normal) * invDiscreminant;
        float u = dot(edgeAC, dao) * invDiscreminant;
        float v = -dot(edgeAB, dao) * invDiscreminant;
        float w = 1 - u - v;

        if(dst >= 0 && u >= 0 && v >= 0 && w >= 0)
        {
            result.didHit = true;
            result.dst = dst;
            result.hitPos = ray.origin + ray.dir * dst;
            result.normal = normalize(tri.normalA * w + tri.normalB * u + tri.normalC * v);
        }
    }

    return result;
}

// AABB intersection based of https://gist.github.com/DomNomNom/46bb1ce47f68d255fd5d
HitInfo intersectAABB(Ray ray, Box box)
{
    HitInfo result;
    result.didHit = false;

    vec3 tMin = (box.boxMin - ray.origin) * ray.invDir;
    vec3 tMax = (box.boxMax - ray.origin) * ray.invDir;

    vec3 t1 = min(tMin, tMax);
    vec3 t2 = max(tMin, tMax);

    float tNear = max(max(t1.x, t1.y), t1.z);;
    float tFar = min(min(t2.x, t2.y), t2.z);

    if(tNear <= tFar && tFar >= 0)
    {
        result.didHit = true;
        result.dst = tNear;
        result.hitPos = ray.origin + ray.dir * tNear;
        result.normal = -sign(ray.dir) * step(vec3(tNear - EPSILON), t1);
    }

    return result;
}

bool intersectAABB(Ray ray, vec3 boxMin, vec3 boxMax)
{
    vec3 tMin = (boxMin - ray.origin) * ray.invDir;
    vec3 tMax = (boxMax - ray.origin) * ray.invDir;

    vec3 t1 = min(tMin, tMax);
    vec3 t2 = max(tMin, tMax);

    float tNear = max(max(t1.x, t1.y), t1.z);;
    float tFar = min(min(t2.x, t2.y), t2.z);

    return tNear <= tFar && tFar >= 0;
}

/*
  Shader
*/

HitInfo intersectScene(Ray ray)
{
    HitInfo result;
    result.didHit = false;
    result.dst = INFINITY;

    //Spheres
    for(int i = 0; i < spheres.length(); i++)
    {
        Sphere sphere = spheres[i];

        HitInfo intersection = intersectSphere(ray, sphere);
        if(!intersection.didHit || intersection.dst > result.dst)
            continue;

        result = intersection;
        result.material = sphere.material;
    }

    //Boxes
    for(int i = 0; i < boxes.length(); i++)
    {
        Box box = boxes[i];

        HitInfo intersection = intersectAABB(ray, box);
        if(!intersection.didHit || intersection.dst > result.dst)
            continue;

        result = intersection;
        result.material = box.material;
    }

    //Meshes
    for(int i = 0; i < meshes.length(); i++)
    {
        TriangleMesh mesh = meshes[i];
        if(!intersectAABB(ray, mesh.boxMin, mesh.boxMax))
            continue;

        for(int j = mesh.triangleIndex; j < mesh.numTriangles; j++)
        {
            Triangle tri = triangles[j];

            HitInfo intersection = intersectTriangle(ray, tri);
            if(!intersection.didHit || intersection.dst > result.dst)
                continue;

            result = intersection;
            result.material = mesh.material;
        }
    }

    return result;
}

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

vec3 traceRay(Ray ray, inout uint rngState)
{
    vec3 outColor = vec3(0);
    vec3 rayColor = vec3(1);

    for(int i = 0; i <= maxBounces; i++)
    {
        HitInfo hitResult = intersectScene(ray);
        if(!hitResult.didHit) {
            outColor += calculateEnvironmentLight(ray) * rayColor;
            break;
        }

        //Calculate lighting
        Material material = hitResult.material;
        vec3 emittedLight = material.emissionColor * material.emissionStrenght;

        outColor += emittedLight * rayColor;
        rayColor *= material.color;

        //Bounce ray
        ray.origin = hitResult.hitPos + hitResult.normal * EPSILON;

        vec3 diffuseDir = normalize(hitResult.normal + randomDir(rngState));
        vec3 specularDir = reflect(ray.dir, hitResult.normal);
        ray.dir = mix(diffuseDir, specularDir, material.smoothness);
        ray.invDir = 1.0 / ray.dir;
    }

    return outColor;
}

void main()
{
    ivec2 pixelCoords = ivec2(gl_GlobalInvocationID.xy);
    ivec2 screenSize = imageSize(resultImage);

    //Generate Random Seed
    uint rngState = uint(pixelCoords.y * screenSize.x + pixelCoords.x + (frameIndex * 719393 * uint(accumulate)));

    //Create ray
    vec3 viewPointLocal = vec3(vec2(pixelCoords) / screenSize - 0.5, 1) * viewParams;
    vec3 viewPoint = vec4(localToWorld * vec4(viewPointLocal, 1)).xyz;

    Ray ray;
    ray.origin = cameraPos;
    ray.dir = normalize(viewPoint - ray.origin);
    ray.invDir = 1.0 / ray.dir;

    //Shoot ray and average color
    vec3 totalLight = vec3(0);
    for(int i = 0; i < raysPerPixel; i++)
        totalLight += traceRay(ray, rngState);

    //Accumulate
    vec3 finalColor = totalLight / raysPerPixel;
    if(accumulate)
    {
        float weight = 1.0 / frameIndex;
        vec3 accumulatedColor = imageLoad(resultImage, pixelCoords).rgb;
        finalColor = mix(accumulatedColor, finalColor, weight);
    }

    imageStore(resultImage, pixelCoords, vec4(finalColor, 1));
}
