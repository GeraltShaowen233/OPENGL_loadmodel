#version 330 core
//in vec4 vertexColor;
in vec2 TexCoord;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
}; 

struct LightDirectional
{
	vec3 pos;
	vec3 dirToLight;
	vec3 color;


};
struct LightSpot
{
	vec3 pos;
	vec3 dirToLight;
	vec3 color;
	float constant;
	float linear;
	float quadratic;
    float cosPhyInner;
    float cosPhyOuter;
};

struct LightPoint
{
	vec3 pos;
	vec3 dirToLight;
	vec3 color;
	float constant;
	float linear;
	float quadratic;
};

uniform Material material;
uniform LightPoint lightP0;
uniform LightPoint lightP1;
uniform LightPoint lightP2;
uniform LightPoint lightP3;
uniform LightDirectional lightD;
uniform LightSpot lightS;

in vec3 FragPos;
in vec3 Normal;

//uniform sampler2D ourTexture;
//uniform sampler2D ourFace;
//uniform vec4 ourColor;

uniform vec3 objColor;
uniform vec3 ambientColor;
uniform vec3 cameraPos;

vec3 CalLightDirectional(LightDirectional light,vec3 uNormal,vec3 dirToCamera)
{
	//diffuse  max(dot(Light,normal),0)
	float diffIntensity = max(dot(light.dirToLight,uNormal),0);
	vec3 diffColor = diffIntensity*light.color*texture(material.diffuse,TexCoord).rgb;

	//specular pow(max(dot(reflect,cam),0),shininess)
	vec3 reflect = normalize(reflect(-light.dirToLight,uNormal));
	float specIntensity = pow(max(dot(reflect,dirToCamera),0),material.shininess);
	vec3 specColor = specIntensity*light.color*texture(material.specular,TexCoord).rgb;

	vec3 result = diffColor+specColor;
	return result;


}

vec3 CalLightPoint(LightPoint light,vec3 uNormal,vec3 dirToCamera)
{
	//attenuation
	float dist = length(light.pos - FragPos);
	float attenuation = 1.0f/(light.constant+light.linear*dist+light.quadratic*(dist*dist));

	//diffuse
	float diffIntensity = max(dot(normalize(light.pos-FragPos),uNormal),0)*attenuation;
	vec3 diffColor = diffIntensity*light.color*texture(material.diffuse,TexCoord).rgb;

	//specular
	vec3 reflect = normalize(reflect(-normalize(light.pos-FragPos),uNormal));
	float specIntensity = pow(max(dot(reflect,dirToCamera),0),material.shininess)*attenuation;
	vec3 specColor = specIntensity*light.color*texture(material.specular,TexCoord).rgb;

	vec3 result = diffColor+specColor;
	return result;

}

vec3 CalLightSpot(LightSpot light,vec3 uNormal,vec3 dirToCamera)
{
	//attenuation
	float dist = length(light.pos - FragPos);
	float attenuation = 1.0f/(light.constant+light.linear*dist+light.quadratic*(dist*dist));
	float spotRatio;
	float CosTheta = dot(normalize(FragPos - light.pos),-light.dirToLight);

	if(CosTheta> light.cosPhyInner)
	{
		spotRatio = 1.0f;
	}else if(CosTheta>light.cosPhyOuter)
	{
		spotRatio = (CosTheta-light.cosPhyOuter)/(light.cosPhyInner-light.cosPhyOuter);
	
	}else
	{
		spotRatio = 0.0f;
	}

	//diffuse
	float diffIntensity = max(dot(normalize(light.pos-FragPos),uNormal),0)*attenuation*spotRatio;
	vec3 diffColor = diffIntensity*light.color*texture(material.diffuse,TexCoord).rgb*vec3(0.5f,0.5f,0.5f);

	//specular
	vec3 reflect = normalize(reflect(-normalize(light.pos-FragPos),uNormal));
	float specIntensity = pow(max(dot(reflect,dirToCamera),0),material.shininess)*attenuation*spotRatio;
	vec3 specColor = specIntensity*light.color*texture(material.specular,TexCoord).rgb;

	vec3 result = diffColor+specColor;
	return result;

}


out vec4 FragColor;
void main()
{
	vec3 finalResult = vec3(0,0,0);
	vec3 uNormal = normalize(Normal);
	vec3 dirToCamera = normalize(cameraPos-FragPos);

	finalResult += CalLightDirectional(lightD,uNormal,dirToCamera);
	finalResult += CalLightPoint(lightP0,uNormal,dirToCamera);

	finalResult += ambientColor*texture(material.diffuse,TexCoord).rgb;
    
	FragColor = vec4(finalResult,1.0f);

	
}