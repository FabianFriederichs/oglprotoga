#version 330 core
#define MAX_DIR_LIGHTS 8
#define MAX_POINT_LIGHTS 16
#define MAX_SPOT_LIGHTS 16
#define MAX_TEXTURES 8

//lighting
struct DirLight
{
    vec3 lightdir;
    vec3 lightcol;
};

struct PointLight
{
    vec3 lightpos;
    vec3 lightcol;
    float cterm;
    float lterm;
    float qterm;
};

struct SpotLight
{
    vec3 lightpos;
    vec3 lightcol;
    vec3 lightdir;
    float cterm;
    float lterm;
    float qterm;    
    float innercone;
    float outercone;
};

//Materials

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
    float alpha;
    sampler2D mtex[MAX_TEXTURES];
    int texcount;
}; 

//input uniforms
uniform DirLight dirlights[MAX_DIR_LIGHTS];
uniform int dirlightcount;

uniform PointLight pointlights[MAX_POINT_LIGHTS];
uniform int pointlightcount;

uniform SpotLight spotlights[MAX_SPOT_LIGHTS];
uniform int spotlightcount;

uniform Material material;

//input interface blocks DO IT!

in struct VertexData
{
	vec3 pos;
	vec2 uv;
	vec3 normal;
	vec3 tangent;
}   vertexdat;

//output
out vec4 color;

//light calculating functions
vec3 CalcDirLight(DirLight light, vec3 vpos, vec2 vuv, vec3 vnormal, vec3 vtangent);
vec3 CalcPointLight(PointLight light, vec3 vpos, vec2 vuv, vec3 vnormal, vec3 vtangent);
vec3 CalcSpotLight(SpotLight light, vec3 vpos, vec2 vuv, vec3 vnormal, vec3 vtangent);


void main()
{
	vec3 outcol;



    color = vec4(outcol, 1.0f);
}

vec3 CalcDirLight(DirLight light, vec3 vpos, vec2 vuv, vec3 vnormal, vec3 vtangent)
{

}

vec3 CalcPointLight(PointLight light, vec3 vpos, vec2 vuv, vec3 vnormal, vec3 vtangent)
{

}

vec3 CalcSpotLight(SpotLight light, vec3 vpos, vec2 vuv, vec3 vnormal, vec3 vtangent)
{

}