#version 330 core

in vec2 vUV;
in vec3 vNormal;
in vec3 vFragPos;

struct DirectionalLight {
    vec4 color;
    vec3 direction;
    vec3 specular;
    float intensity;
};

uniform DirectionalLight directionalLight;
uniform vec4 uColor;
uniform sampler2D uColorTexture;

// World related
uniform vec3 uCameraPosition;

out vec4 outColor;

vec3 CalcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);

    vec3 diffuse = diff * light.color.rgb * light.intensity; // DONE: Strength
    vec3 specular = 0.52 * spec * light.specular;//* vec3(texture(uNormalTexture, vUVs));

    return diffuse + specular;
}

void main() {
    // Sample the normal texture and normalize it
    vec3 normal = normalize(vNormal); // Normalize the normal
    vec3 viewDirection = normalize(uCameraPosition - vFragPos);

    vec3 directionalLightResult = CalcDirLight(directionalLight, normal, viewDirection);

    directionalLightResult += vec3(1.0, 1.0, 1.0);

    // Combine the lighting result with object color
    vec4 objectColor = texture(uColorTexture, vUV) * uColor;
    vec4 result = vec4(directionalLightResult, 1.0) * objectColor;

    outColor = result;
}
