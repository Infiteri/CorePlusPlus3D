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

struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;

    vec3 color;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
    float intensity;
};

uniform DirectionalLight directionalLight;
uniform vec4 uColor;
uniform float uShininess;
uniform sampler2D uColorTexture;
uniform sampler2D uNormalTexture;

// World related
uniform vec3 uCameraPosition;
uniform int uRenderMode;

// Point Light stuff
#define CE_NR_POINT_LIGHTS 24
uniform PointLight pointLights[CE_NR_POINT_LIGHTS];
uniform int uPointLightCount = 0;

#define CE_NR_SPOT_LIGHTS 24
uniform SpotLight spotLights[CE_NR_SPOT_LIGHTS];
uniform int uSpotLightCount = 0;

out vec4 outColor;

vec3 CalcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), uShininess);

    vec3 diffuse = diff * light.color.rgb;
    vec3 specular = spec * light.specular;

    return diffuse + specular;
}

vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), uShininess);

    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    attenuation *= (1.0 / (1.0 + light.linear * distance + light.quadratic * (distance * distance))) * light.intensity;

    vec3 diffuse = light.color.rgb * diff * light.intensity;
    vec3 specular = light.specular * spec;

    diffuse *= attenuation;
    specular *= attenuation;

    return (diffuse + specular);
}

vec3 CalculateSpotLight(SpotLight light, vec3 normal, vec3 fragPos) {
    vec3 lightDir = normalize(light.position - fragPos);

    // Calculate spotlight direction based on non-zero components
    vec3 spotlightDir = normalize(light.direction);

    // Ensure spotlightDir has a non-zero length
    if(length(spotlightDir) == 0.0) {
        return vec3(0.0); // Spotlight direction is not set, return no contribution
    }

    // Use the absolute value of the dot product to avoid issues with direction close to (0, 1, 0)
    float theta = dot(lightDir, -spotlightDir);

    // Adjust the theta range based on outer and inner cutoff angles
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    // Calculate the spotlight condition
    float spotlightCondition = smoothstep(light.outerCutOff, light.cutOff, theta);

    float diff = max(dot(normal, spotlightDir), 0.0);
    vec3 reflectDir = reflect(-spotlightDir, normal);
    vec3 viewDirection = normalize(uCameraPosition - vFragPos);
    float spec = pow(max(dot(viewDirection, reflectDir), 0.0), uShininess);

    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    attenuation *= (1.0 / (1.0 + light.linear * distance + light.quadratic * (distance * distance))) * light.intensity;

    vec3 diffuse = light.color * diff * light.intensity * attenuation * intensity * spotlightCondition;
    vec3 specular = light.specular * spec * light.intensity * attenuation * spotlightCondition;

    return (diffuse + specular);
}
vec3 GetNormal() {
    return vNormal;
}

void main() {
    vec3 normal = normalize(GetNormal()) * texture2D(uNormalTexture, vUV).rgb;
    vec3 viewDirection = normalize(uCameraPosition - vFragPos);

    if(uRenderMode == 0) {
        vec3 directionalLightResult = CalcDirLight(directionalLight, normal, viewDirection);
        vec3 pointLightResult = vec3(0);
        vec3 spotLightResult = vec3(0);

        for(int i = 0; i < uPointLightCount; i++) {
            pointLightResult += CalculatePointLight(pointLights[i], normal, vFragPos, viewDirection);
        }

        for(int i = 0; i < uSpotLightCount; i++) {
            spotLightResult += CalculateSpotLight(spotLights[i], normal, vFragPos);
        }

        vec4 objectColor = texture(uColorTexture, vUV) * uColor;
        vec4 result = vec4(1.0, 1.0, 1.0, 1.0); // Initialize with some base color, if needed

        // Accumulate directional light result
        result += vec4(directionalLightResult, 1.0);

        // Accumulate point light results
        result += vec4(pointLightResult, 1.0);

        // Accumulate spot light results
        result += vec4(spotLightResult, 1.0);

        result *= objectColor;

        outColor = result;

    } else if(uRenderMode == 1) {
        outColor = uColor;
    } else if(uRenderMode == 2) {
        vec3 positiveNormal = max(normalize(GetNormal()), 0.0).rgb;
        outColor = vec4(positiveNormal * texture2D(uNormalTexture, vUV).rgb, 1.0);
    } else if(uRenderMode == 3) {
        outColor = vec4(normalize(GetNormal()).rgb * texture2D(uNormalTexture, vUV).rgb, 1.0);
    } else if(uRenderMode == 4) {
        vec3 directionalLightResult = CalcDirLight(directionalLight, normal, viewDirection);
        vec3 pointLightResult = vec3(0);
        vec3 spotLightResult = vec3(0);

        for(int i = 0; i < uPointLightCount; i++) {
            pointLightResult += CalculatePointLight(pointLights[i], normal, vFragPos, viewDirection);
        }

        for(int i = 0; i < uSpotLightCount; i++) {
            spotLightResult += CalculateSpotLight(spotLights[i], normal, vFragPos);
        }

        vec4 result = vec4(directionalLightResult, 1.0) + vec4(pointLightResult, 1.0) + vec4(spotLightResult, 1.0);
        vec4 objectColor = uColor;
        result *= objectColor;
        result -= (objectColor / 2);
        outColor = result;
    }
}
