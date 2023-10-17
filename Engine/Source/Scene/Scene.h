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
        std::vector<Actor *> actors;

        SceneState state;
        Scope<SceneEnvironment> environment;

    public:
        Scene();
        ~Scene();

        static Scene *GetCopyOfScene(Scene *other);

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

        // --- CAMERA ---
        void ActivateSceneCamera();
        PerspectiveCameraComponent *GetActorCameraComponent();
        // ------------

        inline std::string GetName() { return name; };
        void SetName(const std::string &_name);

        std::vector<Actor *> GetActors();
        SceneEnvironment *GetEnvironment();
        DirectionalLight *GetEnvironmentDirectionalLight();
    };
}