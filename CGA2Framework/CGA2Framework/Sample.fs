#version 330 core

in vec4 modelpos;
out vec4 outcol;

void main()
{
    outcol = vec4(sin(2.0f+modelpos.y), (0.4f+modelpos.z), (0.5f+modelpos.x), 1);
} 