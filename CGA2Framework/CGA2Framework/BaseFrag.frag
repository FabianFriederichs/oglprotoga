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
	float range;
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
	float range;
};

//Materials

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
    float alpha;
    sampler2D mtex[MAX_TEXTURES]; // 0: diffuse // 1: spec // 2: gloss // 3: normal // 4: height
    int texcount;
}; 

//input uniforms
uniform vec3 camerapos;

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
	mat3 TBN;
}   vertexdat;

//output
out vec4 color;

//light calculating functions
vec3 CalcDirLight(DirLight light);
vec3 CalcPointLight(PointLight light);
vec3 CalcSpotLight(SpotLight light);


void main()
{
	vec3 outcol;

	for(int i = 0; i < dirlightcount; i++)
	{
		outcol += CalcDirLight(dirlights[i]);
	}

	for(int i = 0; i < pointlightcount; i++)
	{
		float dist = length(pointlights[i].lightpos - vertexdat.pos);
		if(dist <= pointlights[i].range)
		{
			outcol += CalcPointLight(pointlights[i]);
		}
	}

	for(int i = 0; i < spotlightcount; i++)
	{
		float dist = length(spotlights[i].lightpos - vertexdat.pos);
		if(dist <= spotlights[i].range)
		{
			outcol += CalcSpotLight(spotlights[i]);
		}
	}

    color = vec4(outcol, 1.0f);
}

vec3 CalcDirLight(DirLight light)
{
	 // Ambient
    vec3 ambient = 0.1f * vec3(texture(material.mtex[0], vertexdat.uv));
  	
    // Diffuse 
    vec3 norm = vertexdat.TBN * normalize(texture(material.mtex[3], vertexdat.uv).rgb);
    vec3 lightDir = normalize(-light.lightdir);  
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.lightcol * diff * texture(material.mtex[0], vertexdat.uv).rgb;  
    
    // Specular
    vec3 viewDir = normalize(camerapos - vertexdat.pos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), length(texture(material.mtex[2], vertexdat.uv).rgb));
    vec3 specular = light.lightcol * spec * texture(material.mtex[1], vertexdat.uv).rgb;
            
    return ambient + diffuse + specular;  
}

vec3 CalcPointLight(PointLight light)
{
	// Ambient
    vec3 ambient = 0.1f * texture(material.mtex[0], vertexdat.uv).rgb;
  	
    // Diffuse 
    vec3 norm = vertexdat.TBN * normalize(texture(material.mtex[3], vertexdat.uv).rgb);
    vec3 lightDir = normalize(light.lightpos - vertexdat.pos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.lightcol * diff * texture(material.mtex[0], vertexdat.uv).rgb;  
    
    // Specular
    vec3 viewDir = normalize(camerapos - vertexdat.pos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), length(texture(material.mtex[2], vertexdat.uv).rgb));
    vec3 specular = light.lightcol * spec *  texture(material.mtex[1], vertexdat.uv).rgb;
    
    // Attenuation
    float distance    = length(light.lightpos - vertexdat.pos);
    float attenuation = 1.0f / (light.cterm + light.lterm * distance + light.qterm * (distance * distance));    

    ambient  *= attenuation;  
    diffuse  *= attenuation;
    specular *= attenuation;   
            
    return ambient + diffuse + specular;  
}

vec3 CalcSpotLight(SpotLight light)
{
	vec3 lightDir = normalize(light.lightpos - vertexdat.pos);
	float theta = dot(lightDir, normalize(-light.lightdir));    
	if(theta > light.innercone) 
	{       
		  // Diffuse 
		vec3 norm = vertexdat.TBN * normalize(texture(material.mtex[3], vertexdat.uv).rgb);   
    
		float diff = max(dot(norm, lightDir), 0.0);
		vec3 diffuse = light.lightcol * diff * texture(material.mtex[0], vertexdat.uv).rgb;  
    
		// Specular
		vec3 viewDir = normalize(camerapos - vertexdat.pos);
		vec3 reflectDir = reflect(-lightDir, norm);  
		float spec = pow(max(dot(viewDir, reflectDir), 0.0), length(texture(material.mtex[2], vertexdat.uv).rgb));
		vec3 specular = light.lightcol * spec *  texture(material.mtex[1], vertexdat.uv).rgb;
    
		// Spotlight (soft edges)
		theta = dot(lightDir, normalize(-light.lightdir)); 
		float epsilon = (light.innercone - light.outercone);
		float intensity = clamp((theta - light.outercone) / epsilon, 0.0, 1.0);
		diffuse  *= intensity;
		specular *= intensity;
    
		// Attenuation
		float distance    = length(light.lightpos - vertexdat.pos);
		float attenuation = 1.0f / (light.cterm + light.lterm * distance + light.qterm * (distance * distance));    
		//ambient  *= attenuation; 
		diffuse  *= attenuation;
		specular *= attenuation;   
            
		return diffuse + specular;
	}
	else  // else, use ambient light so scene isn't completely dark outside the spotlight.
		return 0.1f * texture(material.mtex[0], vertexdat.uv).rgb;
    
    
}