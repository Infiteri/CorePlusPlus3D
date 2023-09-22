#version 400

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec2 aUV;
layout(location = 2) in vec3 aNormal;

uniform mat4 uProjection;
uniform mat4 uView;
uniform mat4 uTransform;

out vec2 vUV;
out vec3 vNormal;
out vec3 vFragPos;

void main() {
    vUV = aUV;
    vNormal = aNormal;
    vFragPos = vec3(uTransform * vec4(aPosition, 1.0));
    gl_Position = uProjection * uView * uTransform * vec4(aPosition, 1.0);
}