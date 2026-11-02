#version 400 core

out vec4 FragColor;
in vec2 texCoords;

uniform sampler2D screenTexture;
uniform vec4 color;
uniform bool invert;

void main()
{
   vec4 color = texture(screenTexture, texCoords) + color;
   if(invert){
        color = vec4(vec3(1.0 - texture(screenTexture, texCoords)), 1.0); //inversion
   }
   FragColor = color;

}