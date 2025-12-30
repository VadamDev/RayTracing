#version 460

uniform float test;

void main() {
    gl_Position = vec4(test, 0, 0, 0);
}