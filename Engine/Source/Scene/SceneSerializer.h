#pragma once

#include "Base.h"
#include "Scene.h"

#include <string>

namespace YAML
{
    class Emitter;
}

namespace Core
{
    class CE_API SceneSerializer
    {
    private:
        Scene *scene;

    public:
        SceneSerializer(Scene *scene);
        ~SceneSerializer();

        void Set(Scene *scene);

        void SerActor(Actor *ac, Actor *parent, YAML::Emitter &out);

        void Serialize(const std::string &name);
        void Deserialize(const std::string &name);
        void DeserializeAndCreateNewScene(const std::string &name);
    };
}