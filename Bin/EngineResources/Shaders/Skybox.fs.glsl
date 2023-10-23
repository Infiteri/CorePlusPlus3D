#version 330 core

in vec3 vUVs;

out vec4 FragColor;

uniform samplerCube uSkybox;
uniform vec4 uTint;

void main() {
    FragColor = uTint * texture(uSkybox, vUVs);
}
