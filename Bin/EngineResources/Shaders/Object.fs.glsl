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

struct PointLight {
    vec3 position;
    vec4 color;
    vec3 specular;
    float constant;
    float linear;
    float quadratic;
    float radius;
    float intensity;
};

uniform DirectionalLight directionalLight;
uniform vec4 uColor;
uniform sampler2D uColorTexture;

// World related
uniform vec3 uCameraPosition;

// Point Light stuff
#define CE_NR_POINT_LIGHTS 24  
uniform PointLight pointLights[CE_NR_POINT_LIGHTS];
uniform int uPointLightCount = 0;
out vec4 outColor;

vec3 CalcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);

    vec3 diffuse = diff * light.color.rgb * light.intensity; // DONE: Strength
    vec3 specular = spec * light.specular;//* vec3(texture(uNormalTexture, vUVs));

    return diffuse + specular;
}

vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);

    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    // Calculate attenuation based on radius and intensity
    float radiusFactor = 1.0 - clamp(distance / light.radius, 0.0, 1.0);
    attenuation *= radiusFactor * (1.0 / (1.0 + light.linear * distance + light.quadratic * (distance * distance))) * light.intensity;

    // combine results
    vec3 diffuse = light.color.rgb * diff * light.intensity;
    vec3 specular = light.specular * spec;

    diffuse *= attenuation;
    specular *= attenuation;

    // Return the sum of diffuse and specular components
    return (diffuse + specular);
}

void main() {
    // Sample the normal texture and normalize it
    vec3 normal = normalize(vNormal); // Normalize the normal
    vec3 viewDirection = normalize(uCameraPosition - vFragPos);

    vec3 directionalLightResult = CalcDirLight(directionalLight, normal, viewDirection);
    vec3 pointLightResult = vec3(0);

    for(int i = 0; i < uPointLightCount; i++) {
        pointLightResult += CalculatePointLight(pointLights[i], normal, vFragPos, viewDirection);
    }

    // Combine the lighting result with object color
    vec4 objectColor = texture(uColorTexture, vUV) * uColor;
    vec4 result = vec4(1.0, 1.0, 1.0, 1.0) + vec4(directionalLightResult, 1.0) + vec4(pointLightResult, 1.0);
    result *= objectColor;

    outColor = result;
}
