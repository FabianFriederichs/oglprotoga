#version 330 core
  
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoords;
uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;


out vec2 TexCoords;
void main()
{
	vec4 worldpos = model * vec4(position.x, position.y, position.z, 1.0);
	TexCoords = texCoords;
    gl_Position = proj * view * worldpos;
}