#pragma once

#include "Base.h"
#include "Math/Transform.h"
#include "Renderer/Objects/Mesh.h"

#include "Components/Components.h"

#include <vector>

namespace Core
{
    enum class ActorState
    {
        /// @brief Constructor, just created and no methods called.
        Created,

        /// @brief Init has been called and nothing else.
        Init,

        /// @brief Start has been called and nothing else, if this is the state, the actor can Render / Update.
        Started,

        /// @brief Stop has been called.
        Stopped,

        /// @brief Destroyed, cannot be used anymore.
        Destroyed
    };

    /// @brief Not Object3D, the objects that are used in components are 3D.
    /// Still have a transform.
    class CE_API Actor
    {
    private:
        Transform transform;
        ActorState state;
        CeU32 id;

        // List of components
        std::vector<Component *> components;
        std::string name;

    public:
        Actor();
        ~Actor();

        static Actor* From(Actor* other);

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

        void SetName(const std::string &name);
        std::string GetName();

        std::vector<Component *> GetComponents() { return components; };

        // ---------- COMPONENTS ----------

        template <typename T, typename... Args>
        T *AddComponent(Args... args)
        {
            T *comp = new T(args...);
            comp->owner = this;

            if (state == ActorState::Init)
                comp->Init();
            else if (state == ActorState::Started)
            {
                comp->Init();
                comp->Start();
            }

            components.push_back(comp);
            return comp;
        };

        template <typename T>
        T *GetComponent()
        {
            for (Component *c : components)
            {
                T *typedComponent = dynamic_cast<T *>(c);
                if (typedComponent)
                {
                    return typedComponent;
                }
            }

            return nullptr;
        };

        template <typename T>
        bool HasComponent()
        {
            return GetComponent<T>() != nullptr;
        };

        template <typename T>
        void RemoveComponent()
        {
            for (auto it = components.begin(); it != components.end(); ++it)
            {
                T *typedComponent = dynamic_cast<T *>(*it);
                if (typedComponent)
                {
                    components.erase(it);
                    typedComponent->Destroy();
                    delete typedComponent;
                    break;
                }
            }
        };

        // -------------------------------

        inline Transform *GetTransform() { return &transform; };
        inline CeU32 GetID() { return id; };
    };
}