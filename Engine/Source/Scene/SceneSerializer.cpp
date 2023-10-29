#include "SceneSerializer.h"
#include "Core/CeAssert.h"
#include "Core/Engine.h"
#include "Core/Logger.h"

#include "Components/Components.h"
#include "Renderer/Color.h"
#include "Renderer/Camera/CameraSystem.h"

#include "Renderer/Geometry/BoxGeometry.h"
#include "Renderer/Geometry/PlaneGeometry.h"

#include "Scene/World.h"

#include <yaml-cpp/yaml.h>
#include <fstream>

#define CE_NO_PARENT_NAME "__NO_PARENT_NAME_CE__"

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

    void YAMLToVcc3(YAML::Node node, Vector3 *out)
    {
        out->Set(node[0].as<float>(), node[1].as<float>(), node[2].as<float>());
    };

    void YAMLToColor(YAML::Node node, Color *out)
    {
        out->Set(node[0].as<float>(), node[1].as<float>(), node[2].as<float>(), node[3].as<float>());
    };

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
        out << YAML::Value << "Filed";

        out << YAML::Key << "UUID";
        out << YAML::Value << a->GetUUID()->Get();

        out << YAML::Key << "Name";
        out << YAML::Value << a->GetName().c_str();

        out << YAML::Key << "ParentName";

        if (parent != nullptr)
            out << YAML::Value << parent->GetName().c_str();
        else
            out << YAML::Value << CE_NO_PARENT_NAME;

        // Ser. Transform
        Transform *transform = a->GetTransform();
        out << YAML::Key << "Transform";
        out << YAML::BeginMap;
        out << YAML::Key << "Position" << YAML::Value << &transform->position;
        out << YAML::Key << "Rotation" << YAML::Value << &transform->rotation;
        out << YAML::Key << "Scale" << YAML::Value << &transform->scale;
        out << YAML::EndMap;

        auto mesh = a->GetComponent<MeshComponent>();
        auto actorScript = a->GetComponent<ActorScriptComponent>();
        auto camera = a->GetComponent<PerspectiveCameraComponent>();
        auto aabb = a->GetComponent<AABBComponent>();
        auto pointLight = a->GetComponent<PointLightComponent>();

        if (mesh)
        {
            auto material = mesh->mesh->GetMaterial();
            auto geometry = mesh->mesh->GetGeometry();

            out << YAML::Key << "MeshComponent";
            out << YAML::BeginMap;

            out << YAML::Key << "Name" << YAML::Value << material->GetName().c_str();
            out << YAML::Key << "Unique" << YAML::Value << (mesh->mesh->IsMaterialUnique() == true ? "Yes" : "No");

            if (mesh->mesh->IsMaterialUnique())
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
            CE_ASSERT_IF(actorScript->owner == nullptr);
            CE_ASSERT_IF(actorScript->owner->GetName().empty());
            CE_ASSERT_IF(actorScript->className.empty());

            out << YAML::Key << "ActorScriptComponent";
            out << YAML::BeginMap;
            out << YAML::Key << "ClassName" << YAML::Value << actorScript->className.c_str();
            out << YAML::Key << "OwnerName" << YAML::Value << actorScript->owner->GetName().c_str();
            out << YAML::EndMap;
        }

        if (camera)
        {
            out << YAML::Key << "PerspectiveCameraComponent";
            out << YAML::BeginMap;
            SerCamera("", camera->camera, out);
            out << YAML::EndMap;
        }

        if (aabb)
        {
            out << YAML::Key << "AABBComponent";
            out << YAML::BeginMap;
            out << YAML::Key << "W" << YAML::Value << aabb->width;
            out << YAML::Key << "H" << YAML::Value << aabb->height;
            out << YAML::Key << "D" << YAML::Value << aabb->depth;
            out << YAML::Key << "PX" << YAML::Value << aabb->padding.x;
            out << YAML::Key << "PY" << YAML::Value << aabb->padding.y;
            out << YAML::EndMap;
        }

        if (pointLight)
        {
            out << YAML::Key << "PointLight";
            out << YAML::BeginMap;
            out << YAML::Key << "Position" << YAML::Value << &pointLight->light->GetTransform()->position;
            out << YAML::Key << "Color" << YAML::Value << pointLight->light->GetColor();
            out << YAML::Key << "Specular" << YAML::Value << pointLight->light->GetSpecular();
            out << YAML::Key << "Constant" << YAML::Value << pointLight->light->GetConstant();
            out << YAML::Key << "Linear" << YAML::Value << pointLight->light->GetLinear();
            out << YAML::Key << "Intensity" << YAML::Value << pointLight->light->GetIntensity();
            out << YAML::Key << "Radius" << YAML::Value << pointLight->light->GetRadius();
            out << YAML::EndMap;
        }

        out << YAML::EndMap;

        // NOTE: Serialize the children
        for (Actor *child : a->GetChildren())
            SerActor(child, a, out);
    }

    void SceneSerializer::SerCamera(std::string name, PerspectiveCamera *camera, YAML::Emitter &out)
    {
        out << YAML::Key << "Fov" << YAML::Value << camera->GetFOV();
        out << YAML::Key << "Near" << YAML::Value << camera->GetNear();
        out << YAML::Key << "Far" << YAML::Value << camera->GetFar();
        out << YAML::Key << "Position" << YAML::Value << camera->GetPosition();
        out << YAML::Key << "Rotation" << YAML::Value << camera->GetRotation();
    }

    void SceneSerializer::Serialize(const std::string &name)
    {
        auto camera = scene->GetActorCameraComponent();
        auto env = scene->GetEnvironment();

        CE_ASSERT_IF(scene == nullptr);
        CE_ASSERT_IF(name.empty());

        // Start serializing
        YAML::Emitter out;
        out << YAML::BeginMap;
        out << YAML::Key << "Scene";
        out << YAML::Value << scene->GetName().c_str();

        // -- SERIALIZE SKY --------
        out << YAML::Key << "Sky" << YAML::BeginMap;
        auto sky = scene->GetEnvironment()->sky;
        switch (sky->GetMode())
        {
        case SkyMode::Color:
            out << YAML::Key << "Mode" << YAML::Value << "Color";
            out << YAML::Key << "Color" << YAML::Value << sky->GetColor();
            break;

        case SkyMode::CubeMap:
            out << YAML::Key << "Mode" << YAML::Value << "CubeMap";
            out << YAML::Key << "CubeMapPath" << YAML::Value << sky->GetCubeTexturePath();
            break;

        default:
            CE_WARN("Unknown sky type when serializing '%s' scene.", name.c_str());
            break;
        }
        out << YAML::EndMap;
        // -------------------------

        // -- SERIALIZE LIGHT ------
        out << YAML::Key << "DirectionalLight" << YAML::BeginMap;
        auto light = scene->GetEnvironment()->directionalLight;
        out << YAML::Key << "Color" << YAML::Value << light->GetColor();
        out << YAML::Key << "Direction" << YAML::Value << light->GetDirection();
        out << YAML::EndMap;
        // -------------------------

        out << YAML::Key << "Actors";
        out << YAML::Value << YAML::BeginSeq;

        //
        for (Actor *a : scene->GetActors())
            SerActor(a, a->GetParent(), out);

        out << YAML::EndSeq;
        out << YAML::EndMap;

        std::ofstream fout(name);
        fout << out.c_str();
    }

    void SceneSerializer::Deserialize(const std::string &name)
    {
        CE_ASSERT_IF(name.empty() && "SceneSerializer::Deserialize: Name cannot be empty.");

        std::ifstream stream(name);
        std::stringstream strStream(name);

        strStream << stream.rdbuf();

        YAML::Node data = YAML::Load(strStream.str());

        if (!data["Scene"])
            return;

        std::string sceneName = data["Scene"].as<std::string>();
        scene->SetName(sceneName);

        auto env = scene->GetEnvironment();
        env->sky->SetMode(StringToSkyMode(data["Sky"]["Mode"].as<std::string>()));

        switch (env->sky->GetMode())
        {
        case SkyMode::Color:
        {
            auto c = data["Sky"]["Color"];
            env->sky->GetColor()->Set(c[0].as<float>(), c[1].as<float>(), c[2].as<float>(), c[3].as<float>());
        }
        break;

        case SkyMode::CubeMap:
        {
            env->sky->CreateCubeTexture(data["Sky"]["CubeMapPath"].as<std::string>());
        }
        break;

        default:
            CE_WARN("Unknown sky type when deserializing '%s' scene.", name.c_str());
            break;
        }

        //? Deserialize the directional light.
        {
            // NOTE: Otherwise default values will be set.
            if (data["DirectionalLight"])
            {
                auto c = data["DirectionalLight"]["Color"];
                auto dir = data["DirectionalLight"]["Direction"];
                env->directionalLight->GetColor()->Set(c[0].as<float>(), c[1].as<float>(), c[2].as<float>(), c[3].as<float>());
                env->directionalLight->GetDirection()->Set(dir[0].as<float>(), dir[1].as<float>(), dir[2].as<float>());
            }
        }

        auto actors = data["Actors"];
        if (actors)
        {
            for (auto actor : actors)
            {
                auto transform = actor["Transform"];
                auto mesh = actor["MeshComponent"];
                auto actorScript = actor["ActorScriptComponent"];
                auto camera = actor["PerspectiveCameraComponent"];
                auto aabb = actor["AABBComponent"];
                auto pLight = actor["PointLight"];

                Actor *a = new Actor();
                a->SetUUID(actor["UUID"].as<CeU64>());

                a->SetName(actor["Name"].as<std::string>());
                a->GetTransform()->position.Set(transform["Position"][0].as<float>(), transform["Position"][1].as<float>(), transform["Position"][2].as<float>());
                a->GetTransform()->rotation.Set(transform["Rotation"][0].as<float>(), transform["Rotation"][1].as<float>(), transform["Rotation"][2].as<float>());
                a->GetTransform()->scale.Set(transform["Scale"][0].as<float>(), transform["Scale"][1].as<float>(), transform["Scale"][2].as<float>());

                if (mesh)
                {
                    auto c = a->AddComponent<MeshComponent>();

                    if (mesh["Unique"].as<std::string>().compare("Yes") == 0)
                    {
                        auto col = mesh["Color"];
                        c->mesh->GetMaterial()->GetColor()->Set(col[0].as<float>(), col[1].as<float>(), col[2].as<float>(), col[3].as<float>());

                        std::string colorTexture = mesh["ColorTextureName"] ? mesh["ColorTextureName"].as<std::string>() : "";
                        if (!colorTexture.empty())
                        {
                            c->mesh->GetMaterial()->SetColorTexture(colorTexture);
                        }
                    }
                    else
                    {
                        auto m_name = mesh["Name"].as<std::string>();
                        c->SetMaterialFromName(m_name);
                    }

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
                    auto script = a->AddComponent<ActorScriptComponent>();
                    script->className = actorScript["ClassName"].as<std::string>();
                    script->owner = a;
                }

                if (camera)
                {
                    auto cc = a->AddComponent<PerspectiveCameraComponent>();
                    cc->camera->SetFOV(camera["Fov"].as<float>());
                    cc->camera->SetNear(camera["Near"].as<float>());
                    cc->camera->SetFar(camera["Far"].as<float>());
                    cc->camera->UpdateProjection(Engine::GetWindowAspect());
                }

                if (aabb)
                {
                    auto abc = a->AddComponent<AABBComponent>();
                    abc->width = aabb["W"].as<float>();
                    abc->height = aabb["H"].as<float>();
                    abc->depth = aabb["D"].as<float>();
                    abc->padding.x = aabb["PX"].as<float>();
                    abc->padding.y = aabb["PY"].as<float>();
                }

                if (pLight)
                {
                    auto c = a->AddComponent<PointLightComponent>();
                    YAMLToVcc3(pLight["Position"], &c->light->GetTransform()->position);
                    YAMLToColor(pLight["Color"], c->light->GetColor());
                    YAMLToVcc3(pLight["Specular"], c->light->GetSpecular());
                    c->light->SetConstant(pLight["Constant"].as<float>());
                    c->light->SetLinear(pLight["Linear"].as<float>());
                    c->light->SetIntensity(pLight["Intensity"].as<float>());
                    c->light->SetRadius(pLight["Radius"].as<float>());
                }

                // Deserialize the actor parent
                if (actor["ParentName"])
                {
                    std::string parentName = actor["ParentName"].as<std::string>();
                    if (parentName != CE_NO_PARENT_NAME)
                    {
                        Actor *par = scene->GetActorByName(parentName);
                        par->AddChild(a);
                    }
                    else
                    {
                        scene->AddActor(a);
                    }
                }
                else
                {
                    scene->AddActor(a);
                }
            }
        }
    }

    void SceneSerializer::DeserializeAndCreateNewScene(const std::string &name)
    {
        World::StopActive();

        CE_ASSERT_IF(name.empty() && "SceneSerializer::Deserialize: Name cannot be empty.");
        std::ifstream stream(name);
        std::stringstream strStream(name);
        strStream << stream.rdbuf();
        YAML::Node data = YAML::Load(strStream.str());
        if (!data["Scene"])
            return;

        std::string sceneName = data["Scene"].as<std::string>();

        if (World::Get(sceneName) != nullptr)
        {
            World::Delete(sceneName);
        }

        World::Create(sceneName);
        World::Activate(sceneName);

        Set(World::GetActive());
        Deserialize(name);
    }
}