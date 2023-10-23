#version 330 core
layout(location = 0) in vec2 aPosition;
layout(location = 1) in vec2 aUVs;

out vec2 vUVs;

void main() {
    vUVs = aUVs;
    gl_Position = vec4(aPosition.x, aPosition.y, 0.0, 1.0);
}