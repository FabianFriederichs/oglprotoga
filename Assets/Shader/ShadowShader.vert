#version 330 core
  
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uv;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec3 tangent;

uniform mat4 model;
uniform mat4 ligmat;

void main()
{
    gl_Position = ligmat * vec4(position, 1.0f);
}