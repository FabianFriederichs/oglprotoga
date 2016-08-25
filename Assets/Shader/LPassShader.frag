#version 430 core

#define MAX_DIR_LIGHTS 8
#define MAX_POINT_LIGHTS 800
#define MAX_SPOT_LIGHTS 200

#define MAX_SHININESS 100.0f


//output
out vec4 color;

//input
in vec2 TexCoords;

//textures
uniform sampler2D position;
uniform sampler2D normal;
uniform sampler2D albedo;
uniform sampler2D specular;
uniform sampler2D gloss;
uniform sampler2D height;
uniform sampler2D depth;

//camera
uniform vec3 camerapos;

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

//input interface block
//in struct VertexData
//{
//	vec3 pos;
//	vec2 uv;
//	vec3 normal;
//	vec3 tangent;
//	mat3 TBN;
//}   vertexdat;

uniform DirLight dirlights[MAX_DIR_LIGHTS];
uniform int dirlightcount;

uniform PointLight pointlights[MAX_POINT_LIGHTS];
uniform int pointlightcount;

uniform SpotLight spotlights[MAX_SPOT_LIGHTS];
uniform int spotlightcount;

//skybox
uniform samplerCube skybox;

float linearizeDepth(float near, float far, float depthval)
{
    return (2 * near) / (far + near - depthval * (far - near));
}

vec3 CalcDirLight(DirLight light, vec3 _pos, vec3 _norm, vec4 _alb, vec4 _spec, vec4 _glo, vec4 _hgt, float _dpth);
vec3 CalcPointLight(PointLight light, vec3 _pos, vec3 _norm, vec4 _alb, vec4 _spec,vec4 _glo, vec4 _hgt, float _dpth);
vec3 CalcSpotLight(SpotLight light, vec3 _pos, vec3 _norm, vec4 _alb, vec4 _spec, vec4 _glo,vec4 _hgt, float _dpth);

void main()
{
	vec3 outcol;
	
	//texturelookups
	vec3 pos = texture(position, TexCoords).rgb;
	vec3 norm = texture(normal, TexCoords).rgb;
	vec4 alb = texture(albedo, TexCoords);
	vec4 spec = texture(specular, TexCoords);
	vec4 glo = texture(gloss, TexCoords);
	vec4 hgt = texture(height, TexCoords);
	float dpth = texture(depth, TexCoords).r;

	for(int i = 0; i < dirlightcount; i++)
	{
		outcol += CalcDirLight(dirlights[i], pos, norm, alb, spec, glo, hgt, dpth);
	}

	for(int i = 0; i < pointlightcount; i++)
	{
		float dist = length(pointlights[i].lightpos - pos);
		if(dist <= pointlights[i].range)
		{
			outcol += CalcPointLight(pointlights[i], pos, norm, alb, spec, glo, hgt, dpth);
		}
	}

	for(int i = 0; i < spotlightcount; i++)
	{
		float dist = length(spotlights[i].lightpos - pos);
		if(dist <= spotlights[i].range)
		{
			outcol += CalcSpotLight(spotlights[i], pos, norm, alb, spec, glo, hgt, dpth);
		}
	}

    color = vec4(outcol, 1.0f);
}

vec3 CalcDirLight(DirLight light, vec3 _pos, vec3 _norm, vec4 _alb, vec4 _spec, vec4 _glo, vec4 _hgt, float _dpth)
{
	 // _ambient
    vec3 _ambient = 0.1f * _alb.rgb;
  	
    // _diffuse 
    vec3 lightDir = normalize(-light.lightdir);  
    float diff = max(dot(_norm, lightDir), 0.0);
    vec3 _diffuse = light.lightcol.rgb * diff * _alb;  
    
    // _specular
    vec3 viewDir = normalize(camerapos - _pos);
    vec3 reflectDir = reflect(-lightDir, _norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), clamp(_glo.r, 0.1f, 1.0f) * MAX_SHININESS);
    vec3 _specular = light.lightcol.rgb * spec;
            
    return _ambient + _diffuse + _specular;
	//reflectDir = reflect(-viewDir, _norm); 
	//return texture(skybox, reflectDir).rgb;
}

vec3 CalcPointLight(PointLight light, vec3 _pos, vec3 _norm, vec4 _alb, vec4 _spec, vec4 _glo, vec4 _hgt, float _dpth)
{
	// _ambient
    vec3 _ambient = 0.1f * _alb.rgb;
  	
    // _diffuse 
    vec3 lightDir = normalize(light.lightpos - _pos); 
    float diff = max(dot(_norm, lightDir), 0.0);
    vec3 _diffuse = light.lightcol.rgb * diff * _alb;  
    
    // _specular
    vec3 viewDir = normalize(camerapos - _pos);
    vec3 reflectDir = reflect(-lightDir, _norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), clamp(_glo.r, 0.1f, 1.0f) * MAX_SHININESS);
    vec3 _specular = light.lightcol.rgb * spec;
    
    // Attenuation
    float distance    = length(light.lightpos - _pos);
    float attenuation = 1.0f / (light.cterm + light.lterm * distance + light.qterm * (distance * distance));    

    _ambient  *= attenuation;  
    _diffuse  *= attenuation;
    _specular *= attenuation;   
            
	
    return _ambient + _diffuse + _specular;  
}

vec3 CalcSpotLight(SpotLight light, vec3 _pos, vec3 _norm, vec4 _alb, vec4 _spec, vec4 _glo, vec4 _hgt, float _dpth)
{
	vec3 lightDir = normalize(light.lightpos - _pos);
	float theta = dot(lightDir, normalize(-light.lightdir));    
	if(theta > light.innercone) 
	{       
		// _diffuse 
		vec3 lightDir = normalize(-light.lightdir);  
		float diff = max(dot(_norm, lightDir), 0.0);
		vec3 _diffuse = light.lightcol.rgb * diff * _alb;  
    
		// _specular
		vec3 viewDir = normalize(camerapos - _pos);
		vec3 reflectDir = reflect(-lightDir, _norm);  
		float spec = pow(max(dot(viewDir, reflectDir), 0.0), clamp(_glo.r, 0.1f, 1.0f) * MAX_SHININESS);
		vec3 _specular = light.lightcol.rgb * spec;
    
		// Spotlight (soft edges)
		theta = dot(lightDir, normalize(-light.lightdir)); 
		float epsilon = (light.innercone - light.outercone);
		float intensity = clamp((theta - light.outercone) / epsilon, 0.0, 1.0);
		_diffuse  *= intensity;
		_specular *= intensity;
    
		// Attenuation
		float distance    = length(light.lightpos - _pos);
		float attenuation = 1.0f / (light.cterm + light.lterm * distance + light.qterm * (distance * distance));    
		//_ambient  *= attenuation; 
		_diffuse  *= attenuation;
		_specular *= attenuation;   
            
		return _diffuse+_specular;
	}
	else  // else, use _ambient light so scene isn't completely dark outside the spotlight.
		return 0.1f * _alb.rgb;
    
    
}