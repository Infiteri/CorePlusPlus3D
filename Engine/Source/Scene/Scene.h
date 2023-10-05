#pragma once

#include "Base.h"
#include "Actor.h"

#include "Core/Scope.h"

#include "Renderer/Light/DirectionalLight.h"
#include "Renderer/Sky/Sky.h"

#include <string>
#include <vector>

namespace Core
{
    struct SceneEnvironment
    {
        DirectionalLight *directionalLight;
        Sky *sky;

        ~SceneEnvironment();
    };

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
        Scope<SceneEnvironment> environment;

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
        void RemoveActorByName(const std::string &name);
        void RemoveActorByID(CeU32 id);

        void ActivateSceneCamera();

        inline std::string GetName() { return name; };
        void SetName(const char *_name);

        void SetSceneCameraName(const std::string &cameraName);
        std::string GetSceneCameraName() const;

        void GenerateAndActivateSceneCamera(const std::string &cameraName, float fov, float aspect, float near, float far);
        void GenerateSceneCamera(const std::string &cameraName, float fov, float aspect, float near, float far);

        std::vector<Actor *> GetActors();

        SceneEnvironment *GetEnvironment();
        DirectionalLight *GetEnvironmentDirectionalLight();
    };
}