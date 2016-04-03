#version 330 core

out vec4 gl_FragColor;

void main()
{
    gl_FragColor = vec4(sin(gl_FragCoord.x), 0.5f, 0.5f, 1);
} 