#include "SpotLight.h"
#include "Math/Math.h"
#include "Core/Logger.h"

#include <sstream>

#include "Renderer/Shader.h"
#include "Renderer/ShaderSystem.h"
#include "Scene/Scene.h"
#include "Scene/World.h"

namespace Core
{
    static CeU32 GID = 0;

    SpotLight::SpotLight() : id(0),
                             intensity(1.0f),
                             cutOff(Math::DegToRad(12.5f)),
                             outerCutOff(Math::DegToRad(17.5f)),
                             specular(Vector3(1.0f, 1.0f, 1.0f)),
                             constant(1.0f),
                             linear(0.09f),
                             quadratic(0.032f)
    {
        transform = Transform();
        color = Color(255, 255, 255, 255);
        intensity = 1;
        id = GID;
        GID++;
    }

    SpotLight::~SpotLight()
    {
        GID--;
        id = -1;
    }

    void SpotLight::Update()
    {
        CE_PROFILE_FUNCTION();

        // Get the active shader and world
        Shader *shd = ShaderSystem::Get("EngineResources/Shaders/Object");
        Scene *activeWorld = World::GetActive();

        // Check if the shader and world are valid
        if (!shd || !activeWorld)
            return;

        // Increment the spot light count in the active world's environment
        activeWorld->GetEnvironment()->spotLightCount++;

        // Convert the spot light's ID to a string
        std::stringstream indexStr;
        indexStr << id;

        // Construct the uniform names using the spot light's ID
        std::string positionName = "spotLights[" + indexStr.str() + "].position";
        std::string dirName = "spotLights[" + indexStr.str() + "].direction";
        std::string colorName = "spotLights[" + indexStr.str() + "].color";
        std::string intensityName = "spotLights[" + indexStr.str() + "].intensity";
        std::string cutOffName = "spotLights[" + indexStr.str() + "].cutOff";
        std::string outerCutOffName = "spotLights[" + indexStr.str() + "].outerCutOff";
        std::string specularName = "spotLights[" + indexStr.str() + "].specular";
        std::string constantName = "spotLights[" + indexStr.str() + "].constant";
        std::string linearName = "spotLights[" + indexStr.str() + "].linear";
        std::string quadraticName = "spotLights[" + indexStr.str() + "].quadratic";

        // Set the uniform values in the shader
        shd->Vec3(&transform.position, positionName.c_str());
        shd->Vec3(&transform.rotation, dirName.c_str());
        shd->Vec4(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a / 255.0f, colorName.c_str());
        shd->Float(intensity, intensityName.c_str());
        shd->Float(cutOff, cutOffName.c_str());
        shd->Float(outerCutOff, outerCutOffName.c_str());
        shd->Vec3(specular, specularName.c_str());
        shd->Float(constant, constantName.c_str());
        shd->Float(linear, linearName.c_str());
        shd->Float(quadratic, quadraticName.c_str());
    }

    void SpotLight::IncrementGlobalID()
    {
        GID++;
    }

    void SpotLight::SetGlobalIDZero()
    {
        GID = 0;
    }

    void SpotLight::DecrementGlobalID()
    {
        GID--;
    }

    CeU32 SpotLight::GetSpotLightGID()
    {
        return GID;
    }
}
