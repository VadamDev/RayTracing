#version 460

in vec2 pTextureCoords;

out vec4 fragColor;

void main() {
    fragColor = vec4(pTextureCoords, 0, 1);
}