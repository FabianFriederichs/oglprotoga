#version 330 core

//Vertex inputs
layout (location = 0) in vec3 position;
layout (location = 0) in vec3 uv;
layout (location = 0) in vec3 normal;
layout (location = 0) in vec3 tangent;

//Transformation matrices
uniform mat4 world;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = vec4(position.x, position.y, position.z, 1.0);
}