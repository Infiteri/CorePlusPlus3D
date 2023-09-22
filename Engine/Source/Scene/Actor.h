#pragma once

#include "Base.h"
#include "Math/Transform.h"
#include "Renderer/Objects/Mesh.h"


namespace Core
{
    /// @brief Not Object3D, the objects that are used in components are 3D.
    /// Still have a transform.
    class CE_API Actor
    {
    private:
        Transform transform;
        Mesh* mesh;

    public:
        Actor();
        ~Actor();

        /// @brief At the beginning of the actor.
        void Init();

        /// @brief At the end of the actor.
        void Destroy();

        void Update();
        void Render();

        /// @brief When the actor starts to play.
        void Start();

        /// @brief When the actor stops to play.
        void Stop();

        inline Transform *GetTransform() { return &transform; };
    };
}