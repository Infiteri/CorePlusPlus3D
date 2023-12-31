#pragma once

#include "Base.h"
#include "Core/UUID.h"
#include "Math/Transform.h"
#include "Renderer/Objects/Mesh.h"

#include "Components/Components.h"

#include <vector>

namespace Core
{
    /// @brief Actor state representation.
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
        std::string name;
        Actor *parent;
        UUID uuid;

        // Lists
        std::vector<Component *> components;
        std::vector<Actor *> children;

        Matrix4 localMatrix;
        Matrix4 worldMatrix;

    public:
        Actor();
        ~Actor();

        static Actor *From(Actor *other);

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
        std::vector<Actor *> GetChildren() { return children; };

        UUID *GetUUID() { return &uuid; };
        void SetUUID(CeU64 uid);

        /// @brief Respects the rotation.
        void MoveBy(const Vector3 &vector);

        // -- ACTOR RELATIONS ---------------

        /// @brief If nullptr than there is no parent.
        /// @return Actor* || nullptr.
        Actor *GetParent() { return parent; };
        void SetParent(Actor *a);

        /// @brief Adds a child to the list and sets this as its parent.
        /// @param actor Pointer ot an actor.
        void AddChild(Actor *actor);

        /// @brief Searches and returns a actor by this name.
        /// @param name The name to check.
        /// @return Actor* || nullptr;
        Actor *GetChildByName(const std::string &name);

        /// @brief Removes a child with this name.
        /// @param name The name.
        void RemoveChildByName(const std::string &name);

        void RemoveChildUUID(UUID *uid);

        // ----------------------------------

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
        T *SearchChildrenForComponent()
        {
            for (Component *c : components)
            {
                T *typedComponent = dynamic_cast<T *>(c);
                if (typedComponent)
                {
                    return typedComponent;
                }
            }

            for (Actor *child : children)
            {
                T *componentInChildren = child->SearchChildrenForComponent<T>();
                if (componentInChildren)
                {
                    return componentInChildren;
                }
            }

            return nullptr;
        }

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

        void RemoveComponentAtIndex(int index);

        // -------------------------------

        void CalculateMatrices();

        inline Transform *GetTransform() { return &transform; };
        inline Matrix4 *GetWorldMatrix() { return &worldMatrix; };
        inline Matrix4 *GetLocalMatrix() { return &localMatrix; };
    };
}