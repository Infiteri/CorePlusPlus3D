#include "DirectionalLight.h"

#include "Renderer/ShaderSystem.h"

namespace Core
{
    DirectionalLight::DirectionalLight()
    {
        transform = Transform();
        color = Color(255, 255, 255, 255);
        direction = Vector3(0, 0, -1);
    }

    DirectionalLight::~DirectionalLight()
    {
    }

    void DirectionalLight::Update()
    {
        CE_PROFILE_FUNCTION();

        Shader *shd = ShaderSystem::Get("EngineResources/Shaders/Object");

        if (!shd)
            return;

        shd->Vec3(&direction, "directionalLight.direction");
        shd->Vec4(color.r / 255, color.g / 255, color.b / 255, color.a / 255, "directionalLight.color");
        shd->Vec3({1, 1, 1}, "directionalLight.specular");
        shd->Float(1.0, "directionalLight.intensity");
    }
}