#version 400
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNor;
layout (location = 2) in vec2 aUv;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

uniform mat4 mvpMatrix;
uniform mat4 modelMatrix;
uniform vec3 cameraPos;
uniform vec3 lightPos;
uniform bool hasNormalMap;

out vec3 fPos;
out vec3 fNor;
out vec2 uv;
out vec3 cameraPosition;
out vec3 lightPosition;

void main() {
  cameraPosition = cameraPos;
  lightPosition = lightPos;

  vec4 worldSpacePos = modelMatrix * vec4(aPos, 1);
  fPos = vec3(worldSpacePos.x, worldSpacePos.y, worldSpacePos.z);

  vec4 worldSpaceNormal = normalize(modelMatrix * vec4(aNor, 0));
  fNor = vec3(worldSpaceNormal.x, worldSpaceNormal.y, worldSpaceNormal.z);

  vec3 worldTangent = normalize(vec3(modelMatrix * vec4(aTangent, 0)));
  vec3 worldBitangent = normalize(vec3(modelMatrix * vec4(aBitangent, 0)));
  mat3 TBN = transpose(mat3(worldTangent, worldBitangent, fNor));

  if(hasNormalMap){
    cameraPosition = TBN * cameraPosition;
    lightPosition = TBN * lightPosition;
    fPos = TBN * fPos;
  }

  uv = aUv;
  gl_Position = mvpMatrix * vec4(aPos, 1.0);
}