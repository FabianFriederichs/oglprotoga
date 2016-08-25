#version 330 core

#define MAX_TEXTURES 8

//output
layout(location = 0) out vec4 position;
layout(location = 1) out vec4 normal;
layout(location = 2) out vec4 albedo;
layout(location = 3) out vec4 specular;
layout(location = 4) out vec4 gloss;
layout(location = 5) out vec4 height;

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


void main()
{
	//position
	if(texture(material.mtex[0], vertexdat.uv).a<0.9)
		discard;
	position = vec4(vertexdat.pos, 1.0f);
	
	//will later be modified for parallax mappping
	vec2 uv = vec2(vertexdat.uv.x, 1 - vertexdat.uv.y);
	
	//offset uvs for parallax mapping here
	
	
	
	//sample textures
	//albedo
	albedo = texture(material.mtex[0], uv);
	
	//specular
	specular = texture(material.mtex[1], uv);
	
	//gloss
	gloss = texture(material.mtex[2], uv);
	
	//normal map
	vec3 norm = vertexdat.TBN * normalize(texture(material.mtex[3], uv).rgb);
	normal = vec4(norm.rgb, 1.0f);
	
	//height
	height = texture(material.mtex[4], uv);	
}