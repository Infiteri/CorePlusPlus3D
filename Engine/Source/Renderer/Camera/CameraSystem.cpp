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
        CameraActivationInformation info;
        info.Name = name;
        info.Fov = fov;
        info.Aspect = aspect;
        info.Near = near;
        info.Far = far;
        info.MatrixMode = Camera::TransformMatrix;
        return Generate(info);
    }

    PerspectiveCamera *CameraSystem::Generate(CameraActivationInformation info)
    {
        PerspectiveCamera *camera = new PerspectiveCamera(info.Fov, info.Aspect, info.Near, info.Far);
        camera->SetMatrixMode(info.MatrixMode);
        perspectives[info.Name] = camera;

        return camera;
    }

    void CameraSystem::Activate(const std::string &name)
    {
        if (perspectives[name] == nullptr)
        {
            CE_CORE_ERROR("CameraSystem::Activate: Unable to find camera '%s'.", name.c_str());
            return;
        }

        activeCamera = perspectives[name];

        CE_CORE_TRACE("Active camera: %s", name.c_str());
    }

    void CameraSystem::Activate(PerspectiveCamera *camera)
    {
        CE_TRACE("Activated camera with a instance.");
        activeCamera = camera;
    }

    void CameraSystem::UpdateProjection(const std::string &name, float aspect)
    {
        if (perspectives[name] == nullptr)
        {
            CE_CORE_ERROR("CameraSystem::UpdateProjection: Unable to find camera '%s'.", name.c_str());
            return;
        }

        perspectives[name]->UpdateProjection(aspect);
    }

    void CameraSystem::UpdateProjectionForActiveCamera(float aspect)
    {
        if (activeCamera)
            activeCamera->UpdateProjection(aspect);
    }

    void CameraSystem::UpdateProjectionForAll(float aspect)
    {
        UpdateProjectionForActiveCamera(aspect);

        for (auto it = perspectives.begin(); it != perspectives.end(); it++)
        {
            auto camera = it->second;
            camera->UpdateProjection(aspect);
        }
    }

    bool CameraSystem::DoesCameraExist(const std::string &name)
    {
        return perspectives[name] != nullptr;
    }

    void CameraSystem::SetActiveCameraToNone()
    {
        activeCamera = nullptr;
    }
}