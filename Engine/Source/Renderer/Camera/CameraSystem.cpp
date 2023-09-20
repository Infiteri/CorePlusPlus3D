#include "CameraSystem.h"

#include "Core/Logger.h"

#include <unordered_map>

namespace Core
{
    static std::unordered_map<std::string, PerspectiveCamera *> perspectives;
    static PerspectiveCamera *activeCamera = nullptr;

    void CameraSystem::Init()
    {
    }

    void CameraSystem::Shutdown()
    {
        for (auto it = perspectives.begin(); it != perspectives.end(); it++)
        {
            auto camera = it->second;
            delete camera;
        }

        perspectives.clear();
    }

    PerspectiveCamera *CameraSystem::GetActive()
    {
        return activeCamera;
    }

    PerspectiveCamera *CameraSystem::Get(const std::string &name)
    {
        return perspectives[name] != nullptr ? perspectives[name] : nullptr;
    }

    PerspectiveCamera *CameraSystem::Generate(const std::string &name, float fov, float aspect, float near, float far)
    {
        PerspectiveCamera *camera = new PerspectiveCamera(fov, aspect, near, far);

        perspectives[name] = camera;

        return camera;
    }

    void CameraSystem::Activate(const std::string &name)
    {
        if (perspectives[name] == nullptr)
        {
            CE_ERROR("Unable to find camera '%s'.", name.c_str());
            return;
        }

        activeCamera = perspectives[name];
    }

    void CameraSystem::UpdateProjection(const std::string &name, float aspect)
    {
        if (perspectives[name] == nullptr)
        {
            CE_ERROR("Unable to find camera '%s'.", name.c_str());
            return;
        }

        perspectives[name]->UpdateProjection(aspect);
    }

    void CameraSystem::UpdateProjectionForActiveCamera(float aspect)
    {
        if (activeCamera)
            activeCamera->UpdateProjection(aspect);
    }
}