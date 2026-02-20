#version 460

layout(local_size_x = 8, local_size_y = 4, local_size_z = 1) in;

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
    vec3 position;
    float radius;

    Material material;
};

struct Box
{
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

uniform int frameIndex;
uniform vec3 viewParams; // planeWidth, planeHeight, focalLength;
uniform vec3 cameraPos;

uniform mat4 localToWorld;

uniform bool accumulate;
uniform int maxBounces;
uniform int raysPerPixel;

layout(std430, binding = 0) buffer SphereBuffer {
    Sphere spheres[];
};

layout(std430, binding = 1) buffer BoxBuffer {
    Box boxes[];
};

const float INFINITY = 1.0 / 0.0;
const float EPSILON = 0.00001;
const float PI = 3.1415926;
const float TAU = PI * 2;

/*
  Random Number Generation
  Thanks to: https://www.pcg-random.org/index.html and https://observablehq.com/@riccardoscalco/pcg-random-number-generators-in-glsl
*/

float rand(inout uint rngState)
{
    rngState = rngState * 747796405 + 2891336453;
    uint result = ((rngState >> ((rngState >> 28) + 4)) ^ rngState) * 277803737;
    result = (result >> 22) ^ result;
    return result / 4294967295.0;
}

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

// Thanks to: https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-sphere-intersection.html and https://www.youtube.com/watch?v=Qz0KTGYJtUk
HitInfo intersectSphere(Ray ray, vec3 position, float radius)
{
    HitInfo result;
    result.didHit = false;

    vec3 offsetRayOrigin = ray.origin - position;

    float a = dot(ray.dir, ray.dir);
    float b = dot(offsetRayOrigin, ray.dir) * 2;
    float c = dot(offsetRayOrigin, offsetRayOrigin) - radius * radius;

    float discreminant = b * b - 4 * a * c;

    if(discreminant >= 0)
    {
        float dst = (-b - sqrt(discreminant)) / (2 * a);

        if(dst >= 0)
        {
            result.didHit = true;
            result.dst = dst;
            result.hitPos = ray.origin + ray.dir * dst;
            result.normal = normalize(result.hitPos - position);
        }
    }

    return result;
}

//Based from https://gist.github.com/DomNomNom/46bb1ce47f68d255fd5d
HitInfo intersectAABB(Ray ray, vec3 boxMin, vec3 boxMax)
{
    HitInfo result;
    result.didHit = false;

    vec3 tMin = (boxMin - ray.origin) * ray.invDir;
    vec3 tMax = (boxMax - ray.origin) * ray.invDir;

    vec3 t1 = min(tMin, tMax);
    vec3 t2 = max(tMin, tMax);

    float tNear = max(max(t1.x, t1.y), t1.z);;
    float tFar = min(min(t2.x, t2.y), t2.z);

    if(tNear <= tFar && tFar >= 0)
    {
        result.didHit = true;
        result.dst = tNear;
        result.hitPos = ray.origin + ray.dir * tNear;
        result.normal = -sign(ray.dir) * step(vec3(tNear - EPSILON), t1); //TODO: verify if normalize is required here
    }

    return result;
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

        HitInfo intersection = intersectSphere(ray, sphere.position, sphere.radius);
        if(!intersection.didHit || intersection.dst > result.dst)
            continue;

        result = intersection;
        result.material = sphere.material;
    }

    //Boxes
    for(int i = 0; i < boxes.length(); i++)
    {
        Box box = boxes[i];

        HitInfo intersection = intersectAABB(ray, box.boxMin, box.boxMax);
        if(!intersection.didHit || intersection.dst > result.dst)
            continue;

        result = intersection;
        result.material = box.material;
    }

    return result;
}

vec3 traceRay(Ray ray, inout uint rngState)
{
    vec3 outColor = vec3(0);
    vec3 rayColor = vec3(1);

    for(int i = 0; i <= maxBounces; i++)
    {
        HitInfo hitResult = intersectScene(ray);
        if(!hitResult.didHit)
            break;

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
        ray.invDir = 1 / ray.dir;
    }

    return outColor;
}

void main()
{
    ivec2 pixelCoords = ivec2(gl_GlobalInvocationID.xy);
    ivec2 screenSize = imageSize(resultImage);

    //Generate Random Seed
    uint rngState = uint(pixelCoords.y * screenSize.x + pixelCoords.x + (frameIndex * 719393 * int(accumulate)));

    //Create ray
    vec3 viewPointLocal = vec3(vec2(pixelCoords) / screenSize - 0.5, 1) * viewParams;
    vec3 viewPoint = vec4(localToWorld * vec4(viewPointLocal, 1)).xyz;

    Ray ray;
    ray.origin = cameraPos;
    ray.dir = normalize(viewPoint - ray.origin);
    ray.invDir = 1 / ray.dir;

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
