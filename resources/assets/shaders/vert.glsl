#version 460

//In
layout(location = 0) in vec2 aPos;

//Out
out vec2 pPixelCoords;

//Shader
void main()
{
    pPixelCoords = aPos * 0.5 + 0.5;
    gl_Position = vec4(aPos, 0, 1);
}
