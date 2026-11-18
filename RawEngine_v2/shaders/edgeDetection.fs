#version 400 core

out vec4 FragColor;
in vec3 fPos;
in vec3 fNor;
in vec2 uv;

uniform sampler2D screenTexture;
uniform float edgeIntensity;
void main()
{
    float offset = 0.001;
    vec2 offsets[9] = vec2[](
        vec2(-offset, offset), vec2(0.0, offset), vec2(offset, offset),
        vec2(-offset, 0.0), vec2(0.0, 0.0), vec2(offset, 0.0),
        vec2(-offset, -offset), vec2(0.0, -offset), vec2(offset, -offset)
    );

    float kernel[9] = float[](
        1.0, 1.0, 1.0,
        1.0, -edgeIntensity, 1.0,
        1.0, 1.0, 1.0
    );
    vec4 sampleTex[9];
    vec4 colorResult = vec4(0.0);

    for(int i = 0; i < 9; i++){
        colorResult += texture(screenTexture, uv + offsets[i]) * kernel[i];
    }

    colorResult.a = 1.0;
    FragColor = texture(screenTexture, uv) + clamp(colorResult, 0, 1);
}