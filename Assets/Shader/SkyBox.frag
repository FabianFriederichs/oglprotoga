#version 330 core

in vec3 TexCoords;

uniform samplerCube skybox;

layout(location = 0) out vec4 color;
void main()
{
	color = texture(skybox, vec3(TexCoords.x, TexCoords.y, TexCoords.z));
}