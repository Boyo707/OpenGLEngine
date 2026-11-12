#version 400 core
out vec4 FragColor;

in vec3 fPos;
in vec3 fNor;
in vec2 uv;

uniform vec3 cameraPos;
uniform vec3 objectColor;

//Light variables
uniform vec3 lightPosition;
uniform vec3 lightColor;
uniform float ambientIntensity;
uniform vec3 specularColor;
uniform float specularIntensity;
uniform float specularPower;
uniform float quadraticAttenuation;

void main()
{
   vec3 newLight = vec3(1,1,1) * 0.01 + lightColor;
   vec3 newObjColor = vec3(1,1,1) * 0.01 + objectColor ;
   vec3 lightDir = lightPosition - fPos;
   vec3 normalLightDir = normalize(lightDir);
   vec3 ambient = ambientIntensity * newLight;

   vec3 diffuse = max(dot(normalLightDir, fNor), 0) * newLight;
   float distance = length(lightDir);
   float attenuation = 1.0 / (0.1 + distance + quadraticAttenuation * pow(distance,2));

   vec3 reflectDir = reflect(-normalLightDir, fNor);
   vec3 cameraDir = normalize(cameraPos - fPos);
   vec3 specular = specularIntensity * pow(max(dot(cameraDir, reflectDir), 0), specularPower) * specularColor * newLight;

   vec3 result = (ambient + (diffuse + specular) * attenuation) * newObjColor;
   FragColor = vec4(result, 1);
}