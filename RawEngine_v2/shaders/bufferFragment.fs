#version 400 core

out vec4 FragColor;
in vec3 fPos;
in vec3 fNor;
in vec2 uv;

uniform sampler2D screenTexture;
uniform vec4 color;
uniform bool invert;

void main()
{
   vec4 colorResult = texture(screenTexture, uv);
   if(invert){
        colorResult = vec4(vec3(1.0 - texture(screenTexture, uv)), 1.0); //inversion
   }
   colorResult += color;
   FragColor = colorResult;

}