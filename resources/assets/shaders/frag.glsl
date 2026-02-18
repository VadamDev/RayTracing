#version 460

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

in vec2 pPixelCoords;

out vec4 fragColor;

uniform float currentFrameTime;
uniform ivec2 screenParams;
uniform vec3 viewParams; // planeWidth, planeHeight, focalLength;
uniform vec3 cameraPos;

uniform mat4 localToWorld;

uniform int numSpheres;
uniform int maxBounces;
uniform int raysPerPixel;

layout(std430, binding = 0) buffer SphereBuffer {
    Sphere spheres[];
};

const float INFINITY = 1.0 / 0.0;

const float PI = 3.1415926;
const float TAU = PI * 2;

/*
  Random Number Generation
  Thanks to: https://www.pcg-random.org/index.html and https://observablehq.com/@riccardoscalco/pcg-random-number-generators-in-glsl
*/

float rand(inout uint rgnState)
{
    rgnState = rgnState * 747796405 + 2891336453;
    uint result = ((rgnState >> ((rgnState >> 28) + 4)) ^ rgnState) * 277803737;
    result = (result >> 22) ^ result;
    return result / 4294967295.0;
}

float randNormalDistribution(inout uint rgnState)
{
    float theta = TAU * rand(rgnState);
    return sqrt(-2 * log(rand(rgnState))) * cos(theta);
}

vec3 randomDir(inout uint rgnState)
{
    float x = randNormalDistribution(rgnState);
    float y = randNormalDistribution(rgnState);
    float z = randNormalDistribution(rgnState);
    return normalize(vec3(x, y, z));
}

vec3 randomDirInEmisphere(vec3 normal, inout uint rgnState)
{
    vec3 dir = randomDir(rgnState);
    return dir * sign(dot(normal, dir));
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

    for(int i = 0; i < numSpheres; i++)
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

vec3 traceRay(Ray ray, inout uint rgnState)
{
    vec3 outColor;
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

        ray.origin = hitResult.hitPos;
        ray.dir = randomDirInEmisphere(hitResult.normal, rgnState);
    }

    return outColor;
}

void main()
{
    //Generate Random Seed
    ivec2 pixelCoords = ivec2(pPixelCoords * screenParams);
    uint rgnState = uint((pixelCoords.y * screenParams.x + pixelCoords.x) /*+ currentFrameTime * 719393*/);

    //Create ray
    vec3 viewPointLocal = vec3(pPixelCoords - 0.5, 1) * viewParams;
    vec3 viewPoint = (localToWorld * vec4(viewPointLocal, 1)).xyz;

    Ray ray;
    ray.origin = cameraPos;
    ray.dir = normalize(viewPoint - ray.origin);

    //Shoot ray and average color
    vec3 totalLight;
    for(int i = 0; i < raysPerPixel; i++)
        totalLight += traceRay(ray, rgnState);

    fragColor = vec4(totalLight / raysPerPixel, 1);
}
