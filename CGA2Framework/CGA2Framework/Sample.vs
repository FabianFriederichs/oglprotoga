#version 330 core
  
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uv;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec3 tangent;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

struct Vertex
{
	vec3 pos;
	vec2 uv;
	vec3 normal;
	vec3 tangent;
}

out Vertex vert;


void main()
{
    gl_Position = projection * view * model * vec4(position.x, position.y, position.z, 1.0);
	vert.uv = uc;
	vert.normal = normal;
	vert.pos = vec3(model * vec4(position, 1.0f));
	vert.tangent = vec3(model * vec4(tangent, 1.0f));
}