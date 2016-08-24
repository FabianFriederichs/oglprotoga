#version 330 core

//output
layout(location = 0) out vec4 position;
layout(location = 1) out vec4 normal;
layout(location = 2) out vec4 albedo;
layout(location = 3) out vec4 specular;

//Materials
struct Material {
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    float shininess;
    float alpha;
    sampler2D mtex[MAX_TEXTURES]; // 0: diffuse // 1: spec // 2: gloss // 3: normal // 4: height
    int texcount;
}; 

uniform Material material;

//camera
uniform vec3 camerapos;

//input interface block
in struct VertexData
{
	vec3 pos;
	vec2 uv;
	vec3 normal;
	vec3 tangent;
	mat3 TBN;
}   vertexdat;

//skybox
uniform samplerCube skybox;

void main()
{
}