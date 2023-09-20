#version 330 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec2 aUV;

uniform mat4 uProjection;
uniform mat4 uView;
uniform mat4 uModel;

out vec2 vUV;

void main() {
    vUV = aUV;
    gl_Position = uProjection * uView * vec4(aPosition, 1.0);
}