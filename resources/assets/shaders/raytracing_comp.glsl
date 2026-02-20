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
};

struct Sphere
{
    vec3 position;
    float radius;

    Material material;
};

// Rays management
struct Ray
{
    vec3 origin, dir;
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

uniform int maxBounces;
uniform int raysPerPixel;

layout(std430, binding = 0) buffer SphereBuffer {
    Sphere spheres[];
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

/*
  Shader
*/

HitInfo intersectScene(Ray ray)
{
    HitInfo result;
    result.didHit = false;
    result.dst = INFINITY;

    for(int i = 0; i < spheres.length(); i++)
    {
        Sphere sphere = spheres[i];

        HitInfo intersection = intersectSphere(ray, sphere.position, sphere.radius);
        if(!intersection.didHit || intersection.dst > result.dst)
            continue;

        result = intersection;
        result.material = sphere.material;
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

        Material material = hitResult.material;
        vec3 emittedLight = material.emissionColor * material.emissionStrenght;

        outColor += emittedLight * rayColor;
        rayColor *= material.color;

        ray.origin = hitResult.hitPos + hitResult.normal * EPSILON;
        ray.dir = normalize(hitResult.normal + randomDir(rngState));
    }

    return outColor;
}

void main()
{
    ivec2 pixelCoords = ivec2(gl_GlobalInvocationID.xy);
    ivec2 screenSize = imageSize(resultImage);

    //Generate Random Seed
    uint rngState = uint(pixelCoords.y * screenSize.x + pixelCoords.x + (frameIndex * 719393));

    //Create ray
    vec3 viewPointLocal = vec3(vec2(pixelCoords) / screenSize - 0.5, 1) * viewParams;
    vec3 viewPoint = vec4(localToWorld * vec4(viewPointLocal, 1)).xyz;

    Ray ray;
    ray.origin = cameraPos;
    ray.dir = normalize(viewPoint - ray.origin);

    //Shoot ray and average color
    vec3 totalLight = vec3(0);
    for(int i = 0; i < raysPerPixel; i++)
        totalLight += traceRay(ray, rngState);

    float weight = 1.0 / frameIndex;
    vec3 accumulatedColor = imageLoad(resultImage, pixelCoords).rgb;
    vec3 finalColor = mix(accumulatedColor, totalLight / raysPerPixel, weight);

    imageStore(resultImage, pixelCoords, vec4(finalColor, 1));
}
