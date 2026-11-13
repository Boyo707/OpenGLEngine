#version 400 core
out vec4 FragColor;

in vec3 fPos;
in vec3 fNor;
in vec2 uv;
in mat3 TBN;

in vec3 cameraPosition;
in vec3 lightPosition;

uniform vec3 objectColor;

//textures
uniform sampler2D albedoMap;
uniform sampler2D normalMap;
uniform bool hasAlbedoMap;
uniform bool hasNormalMap;

//Light variables
uniform vec3 lightColor;
uniform float ambientIntensity;
uniform vec3 specularColor;
uniform float specularIntensity;
uniform float specularPower;
uniform float quadraticAttenuation;

void main()
{
   vec3 albedo = vec3(1,1,1);
   vec3 normal = fNor;
   if(hasAlbedoMap)
   {
       albedo = texture(albedoMap, uv).xyz;
   }
   if(hasNormalMap)
   {
      vec3 normalTex = texture(normalMap, uv).xyz;
      normal = normalize(normalTex * 2.0 - 1.0);
   }

   vec3 lightDir = lightPosition - fPos;
   vec3 cameraDir = normalize(cameraPosition - fPos);

   vec3 newLight = vec3(1,1,1) * 0.01 + lightColor;
   vec3 newObjColor = vec3(1,1,1) * 0.01 + objectColor * albedo;

   vec3 normalLightDir = normalize(lightDir);
   vec3 ambient = ambientIntensity * newLight;

   vec3 diffuse = max(dot(normalLightDir, normal), 0) * newLight;
   float distance = length(lightDir);
   float attenuation = 1.0 / (0.1 + distance + quadraticAttenuation * pow(distance,2));

   vec3 reflectDir = reflect(-normalLightDir, normal);
   vec3 specular = specularIntensity * pow(max(dot(cameraDir, reflectDir), 0), specularPower) * specularColor * newLight;

   vec3 result = (ambient + (diffuse + specular) * attenuation) * newObjColor;
   FragColor = vec4(result, 1);
}