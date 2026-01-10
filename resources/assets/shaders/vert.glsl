#version 460

layout(location = 0) in vec2 aPos;

uniform float time;

out vec2 pTextureCoords;

void main() {
    pTextureCoords = aPos * 0.5 + 0.5;

    vec3 pos = vec3(aPos, 1);
    pos.y += sin(time * 2);

    gl_Position = vec4(pos, 1);
}
