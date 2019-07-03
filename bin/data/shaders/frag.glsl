#version 330

layout(location = 0) out vec4 fragColor;

in VertexData{
    vec4 color;
} VertexIn;

void main(void)
{
    fragColor = VertexIn.color;
}