#version 460

//In
in vec2 pPixelCoords;

//Uniforms
uniform vec3 viewParams; //planeWidth, planeHeight, focalLength;
uniform vec3 cameraPos;

uniform mat4 localToWorld;

//Out
out vec4 fragColor;

//Shader
struct Ray
{
    vec3 origin;
    vec3 dir;
};

void main()
{
    vec3 viewPointLocal = vec3(pPixelCoords - 0.5, 1) * viewParams;
    vec3 viewPoint = (localToWorld * vec4(viewPointLocal, 1)).xyz;

    Ray ray;
    ray.origin = cameraPos;
    ray.dir = normalize(viewPoint - ray.origin); //viewPoint

    fragColor = vec4(ray.dir, 1);
}
