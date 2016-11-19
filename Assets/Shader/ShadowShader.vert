#version 330 core
  
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uv;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec3 tangent;

uniform mat4 model;
uniform mat4 ligmat;
out vec2 uvs;
void main()
{
uvs=uv;
    gl_Position = ligmat * model*vec4(position, 1.0f);
}