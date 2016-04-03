#version 330 core

out vec4 outcol;

void main()
{
    outcol = vec4(sin(gl_FragCoord.x), 0.5f, 0.5f, 1);
} 