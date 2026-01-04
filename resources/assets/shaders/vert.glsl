#version 460

layout(location = 0) in vec2 aPos;

out vec2 pTextureCoords;

void main() {
    pTextureCoords = aPos * 0.5 + 0.5;

    gl_Position = vec4(aPos, 0, 1);
}