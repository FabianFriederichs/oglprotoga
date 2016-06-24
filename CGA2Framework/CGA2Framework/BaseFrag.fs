#version 330 core
#define MAX_TEXTURES = 16;
//inputs

uniform vec4 ambientcolor;
uniform vec4 diffusecolor;
uniform vec4 specularcolor;
uniform float shininess;
uniform float alpha;

//texture inputs
uniform sampler2D textures[MAX_TEXTURES];

//lights

out vec4 color;

void main()
{
    color = vec4(1.0f, 0.5f, 0.2f, 1.0f);
} 