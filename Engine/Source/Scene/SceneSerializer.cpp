#include "SceneSerializer.h"
#include "Core/CeAssert.h"
#include "Core/Logger.h"

#include "Components/Components.h"
#include "Renderer/Color.h"
#include "Renderer/Camera/CameraSystem.h"

#include "Renderer/Geometry/BoxGeometry.h"
#include "Renderer/Geometry/PlaneGeometry.h"

#include "Scene/World.h"

#include <yaml-cpp/yaml.h>
#include <fstream>

namespace Core
{
    YAML::Emitter &operator<<(YAML::Emitter &out, Vector3 *v)
    {
        out << YAML::Flow;
        out << YAML::BeginSeq << v->x << v->y << v->z << YAML::EndSeq;
        return out;
    }

    YAML::Emitter &operator<<(YAML::Emitter &out, Color *color)
    {
        out << YAML::Flow;
        out << YAML::BeginSeq << color->r << color->g << color->b << color->a << YAML::EndSeq;
        return out;
    }

    std::string SkyModeToString(SkyMode mode)
    {
        switch (mode)
        {
        case SkyMode::CubeMap:
            return "CubeMap";
            break;

        case SkyMode::Color:
            return "Color";
            break;

        default:
            return "Color";
            break;
        }
    }

    SkyMode StringToSkyMode(const std::string &value)
    {
        if (value.compare("CubeMap") == 0)
            return SkyMode::CubeMap;
        else if (value.compare("Color") == 0)
            return SkyMode::Color;
        else
            return SkyMode::Color;
    }

    std::string GeometryTypeToString(GeometryType mode)
    {
        switch (mode)
        {
        case GeometryType::Box:
            return "Box";
            break;

        case GeometryType::Plane:
            return "Plane";
            break;

        default:
            return "Box";
            break;
        }
    }

    GeometryType StringToGeometryType(const std::string &value)
    {
        if (value.compare("Box") == 0)
            return GeometryType::Box;
        else if (value.compare("Plane") == 0)
            return GeometryType::Plane;
        else
            return GeometryType::Box;
    }

    SceneSerializer::SceneSerializer(Scene *scene)
    {
        Set(scene);
    }

    SceneSerializer::~SceneSerializer()
    {
    }

    void SceneSerializer::Set(Scene *scene)
    {
        this->scene = scene;
    }

    void SceneSerializer::SerActor(Actor *a, Actor *parent, YAML::Emitter &out)
    {
        out << YAML::BeginMap;
        out << YAML::Key << "Actor";
        out << YAML::Value << a->GetID();

        out << YAML::Key << "Name";
        out << YAML::Value << a->GetName().c_str();

        // Ser. Transform
        Transform *transform = a->GetTransform();
        out << YAML::Key << "Transform";
        out << YAML::BeginMap;
        out << YAML::Key << "Position" << YAML::Value << transform->GetPosition();
        out << YAML::Key << "Rotation" << YAML::Value << transform->GetRotation();
        out << YAML::Key << "Scale" << YAML::Value << transform->GetScale();
        out << YAML::EndMap;

        auto mesh = a->GetComponent<MeshComponent>();
        auto actorScript = a->GetComponent<ActorScriptComponent>();

        if (mesh)
        {
            auto material = mesh->mesh->GetMaterial();
            auto geometry = mesh->mesh->GetGeometry();

            out << YAML::Key << "MeshComponent";
            out << YAML::BeginMap;

            out << YAML::Key << "Name" << YAML::Value << material->GetName().c_str();
            out << YAML::Key << "Unique" << YAML::Value << (mesh->mesh->IsMaterialUnique() == true ? "Yes" : "No");

            if (!mesh->mesh->IsMaterialUnique())
            {
                out << YAML::Key << "Color" << YAML::Value << material->GetColor();
                out << YAML::Key << "ColorTextureName" << YAML::Value << material->GetColorTexture()->GetImagePath().c_str();
            }

            out << YAML::Key << "GeometryType" << YAML::Value << GeometryTypeToString(geometry->GetType()).c_str();

            if (geometry->GetType() == GeometryType::Box)
            {
                BoxGeometry *g = (BoxGeometry *)geometry;
                out << YAML::Key << "W" << YAML::Value << g->width;
                out << YAML::Key << "H" << YAML::Value << g->height;
                out << YAML::Key << "D" << YAML::Value << g->depth;
            }
            else if (geometry->GetType() == GeometryType::Plane)
            {
                PlaneGeometry *g = (PlaneGeometry *)geometry;
                out << YAML::Key << "W" << YAML::Value << g->width;
                out << YAML::Key << "H" << YAML::Value << g->height;
            }

            out << YAML::EndMap;
        }

        if (actorScript)
        {
            CE_ASSERT(actorScript->owner != nullptr);
            CE_ASSERT(!actorScript->owner->GetName().empty());
            CE_ASSERT(!actorScript->className.empty());

            out << YAML::Key << "ActorScriptComponent";
            out << YAML::BeginMap;
            out << YAML::Key << "ClassName" << YAML::Value << actorScript->className.c_str();
            out << YAML::Key << "OwnerName" << YAML::Value << actorScript->owner->GetName().c_str();
            out << YAML::EndMap;
        }

        out << YAML::EndMap;
    }

    void SceneSerializer::Serialize(const std::string &name)
    {
        auto camera = CameraSystem::Get(scene->GetSceneCameraName());

        CE_ASSERT(scene != nullptr);
        CE_ASSERT(camera != nullptr);
        CE_ASSERT(!name.empty());

        auto env = scene->GetEnvironment();

        // Start serializing
        YAML::Emitter out;
        out << YAML::BeginMap;
        out << YAML::Key << "Scene";
        out << YAML::Value << scene->GetName().c_str();

        out << YAML::Key << "SceneCamera" << YAML::BeginMap;
        out << YAML::Key << "Name" << YAML::Value << scene->GetSceneCameraName().c_str();
        out << YAML::Key << "Fov" << YAML::Value << camera->GetFOV();
        out << YAML::Key << "Near" << YAML::Value << camera->GetNear();
        out << YAML::Key << "Far" << YAML::Value << camera->GetFar();
        out << YAML::Key << "Position" << YAML::Value << camera->GetPosition();
        out << YAML::Key << "Rotation" << YAML::Value << camera->GetRotation();
        out << YAML::EndMap;

        out << YAML::Key << "Actors";
        out << YAML::Value << YAML::BeginSeq;

        //

        for (Actor *a : scene->GetActors())
            SerActor(a, nullptr, out);

        out << YAML::EndSeq;
        out << YAML::EndMap;

        std::ofstream fout(name);
        fout << out.c_str();
    }

    void SceneSerializer::Deserialize(const std::string &name)
    {
        CE_ASSERT(scene != nullptr);
        CE_ASSERT(!name.empty());

        std::ifstream stream(name);
        std::stringstream strStream(name);

        strStream << stream.rdbuf();

        YAML::Node data = YAML::Load(strStream.str());

        if (!data["Scene"])
            return;

        std::string sceneName = data["Scene"].as<std::string>();

        auto actors = data["Actors"];
        if (actors)
        {
            for (auto actor : actors)
            {
                auto transform = actor["Transform"];
                auto mesh = actor["MeshComponent"];
                auto actorScript = actor["ActorScriptComponent"];

                Actor *a = new Actor();

                a->SetName(actor["Name"].as<std::string>());
                a->GetTransform()->GetPosition()->Set(transform["Position"][0].as<float>(), transform["Position"][1].as<float>(), transform["Position"][2].as<float>());
                a->GetTransform()->GetRotation()->Set(transform["Rotation"][0].as<float>(), transform["Rotation"][1].as<float>(), transform["Rotation"][2].as<float>());
                a->GetTransform()->GetScale()->Set(transform["Scale"][0].as<float>(), transform["Scale"][1].as<float>(), transform["Scale"][2].as<float>());

                if (mesh)
                {
                    auto c = a->AddComponent<MeshComponent>();

                    if (mesh["Unique"].as<std::string>().compare("Yes"))
                    {
                        auto col = mesh["Color"];
                        std::string colorTexture = mesh["ColorTextureName"].as<std::string>();

                        c->mesh->GetMaterial()->GetColor()->Set(col[0].as<float>(), col[1].as<float>(), col[2].as<float>(), col[3].as<float>());

                        if (!colorTexture.empty())
                        {
                            c->mesh->GetMaterial()->SetColorTexture(colorTexture);
                        }
                    }
                    else
                        c->SetMaterialFromName(mesh["Name"].as<std::string>());

                    if (mesh["GeometryType"].as<std::string>().compare("Box") == 0)
                    {
                        c->SetGeometry(new BoxGeometry(mesh["W"].as<float>(), mesh["H"].as<float>(), mesh["D"].as<float>()));
                    }
                    else if (mesh["GeometryType"].as<std::string>().compare("Plane") == 0)
                    {
                        c->SetGeometry(new PlaneGeometry(mesh["W"].as<float>(), mesh["H"].as<float>()));
                    }
                }

                if (actorScript)
                {
                }

                scene->AddActor(a);
            }
        }
    }

    void SceneSerializer::DeserializeAndCreateNewScene(const std::string &name)
    {
        World::Create("NewScene");
        World::Activate("NewScene");
        Set(World::GetActive());
        Deserialize(name);
    }
}