#version 330

uniform mat4 modelViewProjectionMatrix;

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 color;

out VertexData{
    vec4 color;
} VertexOut;

void main(void)
{
    VertexOut.color = color;
    gl_Position =  position;
}