#include "PointLight.h"
#include "Core/Logger.h"

#include <sstream>

#include "Renderer/Shader.h"
#include "Renderer/ShaderSystem.h"
#include "Scene/Scene.h"
#include "Scene/World.h"

namespace Core
{
    static CeU32 GID = 0;
    PointLight::PointLight()
    {
        transform = Transform();
        color = Color(255, 255, 255, 255);
        specular = Vector3(1, 1, 1);
        intensity = 10;
        radius = 100;
        id = GID;
        GID++;
    }

    PointLight::~PointLight()
    {
        GID--;
        id = -1;
    }

    void PointLight::Update()
    {
        CE_PROFILE_FUNCTION();

        Shader *shd = ShaderSystem::Get("EngineResources/Shaders/Object");

        if (!shd || !World::GetActive())
            return;

        World::GetActive()->GetEnvironment()->pointLightCount++;

        std::stringstream indexStr;
        indexStr << id;

        std::string positionName = "pointLights[" + indexStr.str() + "].position";
        shd->Vec3(&transform.position, positionName.c_str());

        std::string colorName = "pointLights[" + indexStr.str() + "].color";
        shd->Vec4(color.r / 255, color.g / 255, color.b / 255, color.a / 255, colorName.c_str());

        std::string specularName = "pointLights[" + indexStr.str() + "].specular";
        shd->Vec3(&specular, specularName.c_str());

        std::string constantName = "pointLights[" + indexStr.str() + "].constant";
        shd->Float(constant, constantName.c_str());

        std::string linearName = "pointLights[" + indexStr.str() + "].linear";
        shd->Float(linear, linearName.c_str());

        std::string quadraticName = "pointLights[" + indexStr.str() + "].quadratic";
        shd->Float(quadratic, quadraticName.c_str());

        std::string radiusName = "pointLights[" + indexStr.str() + "].radius";
        shd->Float(radius, radiusName.c_str());

        std::string intensityName = "pointLights[" + indexStr.str() + "].intensity";
        shd->Float(intensity, intensityName.c_str());
    }

    void PointLight::IncremenetGolbalID()
    {
        GID++;
    }

    void PointLight::SetGlobalID0()
    {
        GID = 0;
    }

    void PointLight::DecremenetGolbalID()
    {
        GID--;
    }

    CeU32 PointLight::GetPointLightGID()
    {
        return GID;
    }
}