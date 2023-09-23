#pragma once

#include "Base.h"

#include "Actor.h"

#include <string>
#include <vector>

namespace Core
{
    enum class SceneState
    {
        Created,
        Init,
        Running,
        Stopped,
        Destroyed
    };

    class CE_API Scene
    {
    private:
        std::string name;
        std::string sceneCameraName;
        std::vector<Actor *> actors;

        SceneState state;

    public:
        Scene();
        ~Scene();

        /// @brief At the beginning of the scene.
        void Init();

        /// @brief At the end of the scene.
        void Destroy();

        void Update();
        void Render();

        /// @brief When the scene starts to play.
        void Start();

        /// @brief When the scene stops to play.
        void Stop();

        void AddActor(Actor *a);

        void ActivateSceneCamera();

        inline std::string GetName() { return name; };
        void SetName(const char *_name);

        void SetSceneCameraName(const std::string &cameraName);
        std::string GetSceneCameraName() const;

        void GenerateAndActivateSceneCamera(const std::string &cameraName, float fov, float aspect, float near, float far);
    };
}