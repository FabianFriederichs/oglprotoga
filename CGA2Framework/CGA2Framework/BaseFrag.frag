#version 330 core
#define MAX_DIR_LIGHTS 8
#define MAX_POINT_LIGHTS 16
#define MAX_SPOT_LIGHTS 16
#define MAX_TEXTURES 8

#define MAX_SHININESS 50.0f

//lighting
struct DirLight
{
    vec3 lightdir;
    vec4 lightcol;
};

struct PointLight
{
    vec3 lightpos;
    vec4 lightcol;
    float cterm;
    float lterm;
    float qterm;
	float range;
};

struct SpotLight
{
    vec3 lightpos;
    vec4 lightcol;
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
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    float shininess;
    float alpha;
    sampler2D mtex[MAX_TEXTURES]; // 0: diffuse // 1: spec // 2: gloss // 3: normal // 4: height
    int texcount;
}; 

uniform Material material;

//input uniforms
uniform vec3 camerapos;

uniform DirLight dirlights[MAX_DIR_LIGHTS];
uniform int dirlightcount;

uniform PointLight pointlights[MAX_POINT_LIGHTS];
uniform int pointlightcount;

uniform SpotLight spotlights[MAX_SPOT_LIGHTS];
uniform int spotlightcount;



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
	int texturecount = material.texcount;

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

	//color = texture(material.mtex[4], vec2(vertexdat.uv.x, -vertexdat.uv.y));
}

vec3 CalcDirLight(DirLight light)
{
	 // Ambient
    vec3 ambient = 0.1f * vec3(texture(material.mtex[0], vec2(vertexdat.uv.x, vertexdat.uv.y)));
  	
    // Diffuse 
    vec3 norm = vertexdat.TBN * normalize(texture(material.mtex[3], vec2(vertexdat.uv.x, vertexdat.uv.y)).rgb);
    vec3 lightDir = normalize(-light.lightdir);  
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.lightcol.rgb * diff * texture(material.mtex[0], vec2(vertexdat.uv.x, vertexdat.uv.y)).rgb;  
    
    // Specular
    vec3 viewDir = normalize(camerapos - vertexdat.pos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), clamp(texture(material.mtex[2], vec2(vertexdat.uv.x, vertexdat.uv.y)).r, 0.0f, 1.0f) * MAX_SHININESS);
    vec3 specular = light.lightcol.rgb * spec * texture(material.mtex[1], vec2(vertexdat.uv.x, vertexdat.uv.y)).rgb;
            
    return ambient + diffuse + specular;  
}

vec3 CalcPointLight(PointLight light)
{
	// Ambient
    vec3 ambient = 0.1f * texture(material.mtex[0], vec2(vertexdat.uv.x, -vertexdat.uv.y)).rgb;
  	
    // Diffuse 
    vec3 norm = vertexdat.TBN * normalize(texture(material.mtex[3], vec2(vertexdat.uv.x, -vertexdat.uv.y)).rgb);
    vec3 lightDir = normalize(light.lightpos - vertexdat.pos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.lightcol.rgb * diff * texture(material.mtex[0], vec2(vertexdat.uv.x, -vertexdat.uv.y)).rgb;  
    
    // Specular
    vec3 viewDir = normalize(camerapos - vertexdat.pos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), clamp(texture(material.mtex[2], vec2(vertexdat.uv.x, -vertexdat.uv.y)).r, 0.0f, 1.0f) * MAX_SHININESS);
    vec3 specular = light.lightcol.rgb * spec *  texture(material.mtex[1], vec2(vertexdat.uv.x, -vertexdat.uv.y)).rgb;
    
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
		vec3 norm = vertexdat.TBN * normalize(texture(material.mtex[3], vec2(vertexdat.uv.x, -vertexdat.uv.y)).rgb);   
    
		float diff = max(dot(norm, lightDir), 0.0);
		vec3 diffuse = light.lightcol.rgb * diff * texture(material.mtex[0], vec2(vertexdat.uv.x, -vertexdat.uv.y)).rgb;  
    
		// Specular
		vec3 viewDir = normalize(camerapos - vertexdat.pos);
		vec3 reflectDir = reflect(-lightDir, norm);  
		float spec = pow(max(dot(viewDir, reflectDir), 0.0), clamp(texture(material.mtex[2], vec2(vertexdat.uv.x, -vertexdat.uv.y)).r, 0.0f, 1.0f) * MAX_SHININESS);
		vec3 specular = light.lightcol.rgb * spec *  texture(material.mtex[1], vec2(vertexdat.uv.x, -vertexdat.uv.y)).rgb;
    
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
		return 0.1f * texture(material.mtex[0], vec2(vertexdat.uv.x, -vertexdat.uv.y)).rgb;
    
    
}