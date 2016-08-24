#version 330 core
layout(location = 0) out vec4 color;
in vec2 TexCoords;

uniform sampler2D tex;

float linearizeDepth(float near, float far, float depthval)
{
    return (2 * near) / (far + near - depthval * (far - near));
}

void main()
{ 
    float ldepth = linearizeDepth(0.1f, 100.0f, texture(tex, TexCoords).r);
    color = vec4(ldepth, ldepth, ldepth, 1.0f);
}