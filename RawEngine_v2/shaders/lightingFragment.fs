#version 400 core
out vec4 FragColor;

in vec3 fPos;
in vec3 fNor;
in vec2 uv;

uniform vec3 lightPosition;
uniform vec3 lightColor;

uniform vec3 cameraPos;

uniform vec3 objectColor;

uniform float ambientIntensity;

uniform vec3 specularColor;
uniform float specularIntensity;
uniform float specularPower;

uniform float quadraticAttenuation;

void main()
{
   vec3 lightDir = fPos - lightPosition;
   vec3 normalLightDir = normalize(lightDir);
   vec3 ambient = ambientIntensity * lightColor;

   vec3 diffuse = max(dot(normalLightDir, fNor), 0) * lightColor;
   float distance = length(lightDir);
   float attenuation = 1.0 / (0.1 + distance + quadraticAttenuation * pow(distance,2));

   vec3 reflectDir = reflect(-normalLightDir, fNor);
   vec3 cameraDir = normalize(cameraPos - fPos);
   vec3 specular = specularIntensity * pow(max(dot(cameraDir, reflectDir), 0), specularPower) * specularColor * lightColor;

   vec3 result = (ambient + (diffuse + specular) * attenuation) * objectColor;
   FragColor = vec4(result, 1);
}