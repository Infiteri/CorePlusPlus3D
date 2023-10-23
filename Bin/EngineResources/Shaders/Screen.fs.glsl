#version 330 core

in vec2 vUVs;

uniform sampler2D uScreenTexture;

out vec4 oColor;

void main() {
    vec3 colorNoAlpha = texture(uScreenTexture, vUVs).rgb;
    oColor = vec4(colorNoAlpha, 1.0);
}