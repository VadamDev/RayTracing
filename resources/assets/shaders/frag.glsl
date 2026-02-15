#version 460

/*
  Structs
*/

// Raytraced Objects
struct Material
{
    vec4 color;
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

uniform vec2 screenParams;
uniform vec3 viewParams; // planeWidth, planeHeight, focalLength;
uniform vec3 cameraPos;

uniform mat4 localToWorld;

uniform int numSpheres;

layout(std430, binding = 0) buffer SphereBuffer {
    Sphere spheres[];
};

/*
  Shader
*/

const float INFINITY = 1.0 / 0.0;

// Thanks to: https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-sphere-intersection.html and https://www.youtube.com/watch?v=Qz0KTGYJtUk
HitInfo intersectSphere(Ray ray, vec3 position, float radius)
{
    HitInfo result;
    result.didHit = false;

    vec3 offsetRayOrigin = ray.origin - position;

    float a = dot(ray.dir, ray.dir);
    float b = 2 * dot(offsetRayOrigin, ray.dir);
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

void main()
{
    //Create ray
    vec3 viewPointLocal = vec3(pPixelCoords - 0.5, 1) * viewParams;
    vec3 viewPoint = (localToWorld * vec4(viewPointLocal, 1)).xyz;

    Ray ray;
    ray.origin = cameraPos;
    ray.dir = normalize(viewPoint - ray.origin);

    //Find the nearest object
    HitInfo hitResult = intersectScene(ray);
    Material material = hitResult.material;

    fragColor = vec4(material.color.rgb, 1);
}
