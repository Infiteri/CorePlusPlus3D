#include "SceneSerializer.h"
#include "Core/CeAssert.h"
#include "Core/Engine.h"
#include "Core/Logger.h"

#include "Components/Components.h"
#include "Renderer/Color.h"
#include "Renderer/Camera/CameraSystem.h"

#include "Renderer/Geometry/BoxGeometry.h"
#include "Renderer/Geometry/PlaneGeometry.h"
#include "Renderer/Light/SpotLight.h"
#include "Renderer/Light/PointLight.h"

#include "Scene/World.h"

#include <yaml-cpp/yaml.h>
#include <fstream>

#define CE_NO_PARENT_NAME "__NO_PARENT_NAME_CE__"
#define CE_SERIALIZE_FIELD(name, value) out << YAML::Key << name << YAML::Value << value
#define CE_CASE_SHADER_DATA_SERIALIZE(prefix, type) \
    case Data::prefix:                              \
    {                                               \
        type *v = (type *)d->Data;                  \
        out << YAML::Key << "Value" << v;           \
        break;                                      \
    }

namespace Core
{
    YAML::Emitter &operator<<(YAML::Emitter &out, Vector2 *v)
    {
        out << YAML::Flow;
        out << YAML::BeginSeq << v->x << v->y << YAML::EndSeq;
        return out;
    }

    YAML::Emitter &operator<<(YAML::Emitter &out, Vector3 *v)
    {
        out << YAML::Flow;
        out << YAML::BeginSeq << v->x << v->y << v->z << YAML::EndSeq;
        return out;
    }

    YAML::Emitter &operator<<(YAML::Emitter &out, Vector4 *v)
    {
        out << YAML::Flow;
        out << YAML::BeginSeq << v->x << v->y << v->z << v->w << YAML::EndSeq;
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

        case SkyMode::Shader:
            return "Shader";
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
        else if (value.compare("Shader") == 0)
            return SkyMode::Shader;
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
        if (!node)
            out->Set(1, 1, 1);
        else
            out->Set(node[0].as<float>(), node[1].as<float>(), node[2].as<float>());
    };

    void YAMLToColor(YAML::Node node, Color *out)
    {
        if (!node)
            out->Set(255, 255, 255, 255);
        else
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
        CE_SERIALIZE_FIELD("Position", &transform->position);
        CE_SERIALIZE_FIELD("Rotation", &transform->rotation);
        CE_SERIALIZE_FIELD("Scale", &transform->scale);
        out << YAML::EndMap;

        auto mesh = a->GetComponent<MeshComponent>();
        auto actorScript = a->GetComponent<ActorScriptComponent>();
        auto camera = a->GetComponent<PerspectiveCameraComponent>();
        auto aabb = a->GetComponent<AABBComponent>();
        auto pointLight = a->GetComponent<PointLightComponent>();
        auto spotLight = a->GetComponent<SpotLightComponent>();
        auto data = a->GetComponent<DataComponent>();
        auto physics = a->GetComponent<PhysicsComponent>();

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
                out << YAML::Key << "NormalTextureName" << YAML::Value << material->GetNormalTexture()->GetImagePath().c_str();
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
            CE_SERIALIZE_FIELD("Position", &pointLight->light->GetTransform()->position);
            CE_SERIALIZE_FIELD("Color", pointLight->light->GetColor());
            CE_SERIALIZE_FIELD("Specular", pointLight->light->GetSpecular());
            CE_SERIALIZE_FIELD("Quadratic", pointLight->light->GetQuadratic());
            CE_SERIALIZE_FIELD("Linear", pointLight->light->GetLinear());
            CE_SERIALIZE_FIELD("Constant", pointLight->light->GetConstant());
            CE_SERIALIZE_FIELD("Intensity", pointLight->light->GetIntensity());
            CE_SERIALIZE_FIELD("Radius", pointLight->light->GetRadius());
            out << YAML::EndMap;
        }

        if (spotLight)
        {
            out << YAML::Key << "SpotLight";
            out << YAML::BeginMap;
            out << YAML::Key << "Color" << YAML::Value << spotLight->light->GetColor();
            out << YAML::Key << "Specular" << YAML::Value << spotLight->light->GetSpecular();
            out << YAML::Key << "Quadratic" << YAML::Value << spotLight->light->GetQuadratic();
            out << YAML::Key << "Linear" << YAML::Value << spotLight->light->GetLinear();
            out << YAML::Key << "Constant" << YAML::Value << spotLight->light->GetConstant();
            out << YAML::Key << "Intensity" << YAML::Value << spotLight->light->GetIntensity();
            out << YAML::Key << "CutOff" << YAML::Value << spotLight->light->GetCutOff();
            out << YAML::Key << "OuterCutOff" << YAML::Value << spotLight->light->GetOuterCutOff();
            out << YAML::EndMap;
        }

        if (data)
        {
            out << YAML::Key << "DataComponent";
            out << YAML::BeginSeq;

            for (auto d : data->Sets)
            {
                out << YAML::BeginMap;
                out << YAML::Key << "Name" << YAML::Value << d->Name;
                out << YAML::Key << "Type" << YAML::Value << d->TypeToString();

                switch (d->Type)
                {
                    CE_CASE_SHADER_DATA_SERIALIZE(DataVec2, Vector2);
                    CE_CASE_SHADER_DATA_SERIALIZE(DataVec3, Vector3);
                    CE_CASE_SHADER_DATA_SERIALIZE(DataVec4, Vector4);
                    CE_CASE_SHADER_DATA_SERIALIZE(DataColor, Color);
                    CE_CASE_SHADER_DATA_SERIALIZE(DataFloat, Data::FloatContainer);
                }

                out << YAML::EndMap;
            }

            out << YAML::EndSeq;
        }

        if (physics)
        {
            out << YAML::Key << "PhysicsComponent";
            out << YAML::BeginMap;
            out << YAML::Key << "Velocity" << YAML::Value << &physics->Configuration.Velocity;
            out << YAML::Key << "Acceleration" << YAML::Value << &physics->Configuration.Acceleration;
            out << YAML::Key << "Size" << YAML::Value << &physics->Configuration.Size;
            out << YAML::Key << "Mass" << YAML::Value << physics->Configuration.Mass;
            out << YAML::Key << "Damping" << YAML::Value << physics->Configuration.Damping;
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
        CE_PROFILE_FUNCTION();

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

        case SkyMode::Shader:
            out << YAML::Key << "Mode" << YAML::Value << "Shader";
            out << YAML::Key << "ShaderName" << YAML::Value << sky->GetShaderName();

            // Serialize the shader data
            out << YAML::Key << "SkyShaderData";
            out << YAML::Value << YAML::BeginSeq;

            for (auto d : sky->GetSkyShaderData())
            {
                out << YAML::BeginMap;
                out << YAML::Key << "Name" << YAML::Value << d->Name;
                out << YAML::Key << "Type" << YAML::Value << d->TypeToString();

                switch (d->Type)
                {
                case Data::DataVec2:
                {
                    Vector2 *v = (Vector2 *)d->Data;
                    out << YAML::Key << "Value" << v;
                    break;
                }

                case Data::DataVec3:
                {
                    Vector3 *v = (Vector3 *)d->Data;
                    out << YAML::Key << "Value" << v;
                    break;
                }

                case Data::DataVec4:
                {
                    Vector4 *v = (Vector4 *)d->Data;
                    out << YAML::Key << "Value" << v;
                    break;
                }

                case Data::DataColor:
                {
                    Color *v = (Color *)d->Data;
                    out << YAML::Key << "Value" << v;
                    break;
                }
                case Data::DataFloat:
                {
                    Data::FloatContainer *v = (Data::FloatContainer *)d->Data;
                    out << YAML::Key << "Value" << v->Value;
                    break;
                }
                }

                out << YAML::EndMap;
            }

            out << YAML::EndSeq;

            break;

        default:
            CE_CORE_WARN("Unknown sky type when serializing '%s' scene.", name.c_str());
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
        CE_PROFILE_FUNCTION();

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

        case SkyMode::Shader:
        {
            env->sky->SetShaderName(data["Sky"]["ShaderName"].as<std::string>());

            for (auto skyShaderData : data["Sky"]["SkyShaderData"])
            {
                std::string name = skyShaderData["Name"].as<std::string>();
                Data::DataType type = Data::StringToDataType(skyShaderData["Type"].as<std::string>());

                switch (type)
                {
                case Data::DataVec2:
                {
                    Vector2 *data = new Vector2(skyShaderData["Value"][0].as<float>(), skyShaderData["Value"][1].as<float>());
                    env->sky->AddShaderData(sizeof(data), data, type, name);
                    delete data;
                }
                break;

                case Data::DataVec3:
                {
                    Vector3 *data = new Vector3(skyShaderData["Value"][0].as<float>(), skyShaderData["Value"][1].as<float>(), skyShaderData["Value"][2].as<float>());
                    env->sky->AddShaderData(sizeof(data), data, type, name);
                    delete data;
                }
                break;

                case Data::DataVec4:
                {
                    Vector4 *data = new Vector4(skyShaderData["Value"][0].as<float>(), skyShaderData["Value"][1].as<float>(), skyShaderData["Value"][2].as<float>(), skyShaderData["Value"][3].as<float>());
                    env->sky->AddShaderData(sizeof(data), data, type, name);
                    delete data;
                }
                break;

                case Data::DataColor:
                {
                    Color *data = new Color(skyShaderData["Value"][0].as<float>(), skyShaderData["Value"][1].as<float>(), skyShaderData["Value"][2].as<float>(), skyShaderData["Value"][3].as<float>());
                    env->sky->AddShaderData(sizeof(data), data, type, name);
                    delete data;
                }
                break;

                case Data::DataFloat:
                {
                    Data::FloatContainer *data = new Data::FloatContainer(skyShaderData["Value"].as<float>());
                    env->sky->AddShaderData(sizeof(data), data, type, name);
                    delete data;
                }
                break;

                case Data::DataNone:
                default:
                    break;
                }
            }
        }
        break;

        default:
            CE_CORE_WARN("Unknown sky type when deserializing '%s' scene.", name.c_str());
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
                auto sLight = actor["SpotLight"];
                auto physics = actor["PhysicsComponent"];
                auto data = actor["DataComponent"];

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

                        std::string normalTexture = mesh["NormalTextureName"] ? mesh["NormalTextureName"].as<std::string>() : "";
                        if (!normalTexture.empty())
                        {
                            c->mesh->GetMaterial()->SetNormalTexture(normalTexture);
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
                    c->light->SetQuadratic(pLight["Quadratic"].as<float>());
                    c->light->SetIntensity(pLight["Intensity"].as<float>());
                    c->light->SetRadius(pLight["Radius"].as<float>());
                }

                if (sLight)
                {
                    auto c = a->AddComponent<SpotLightComponent>();
                    YAMLToColor(sLight["Color"], c->light->GetColor());
                    YAMLToVcc3(sLight["Specular"], c->light->GetSpecular());
                    c->light->SetConstant(sLight["Constant"].as<float>());
                    c->light->SetLinear(sLight["Linear"].as<float>());
                    c->light->SetQuadratic(sLight["Quadratic"].as<float>());
                    c->light->SetIntensity(sLight["Intensity"].as<float>());
                    c->light->SetCutOff(sLight["CutOff"].as<float>());
                    c->light->SetOuterCutOff(sLight["OuterCutOff"].as<float>());
                }

                if (data)
                {
                    auto c = a->AddComponent<DataComponent>();
                    for (auto yData : data)
                    {
                        std::string name = yData["Name"].as<std::string>();
                        CE_TRACE(name.c_str());
                        Data::DataType type = Data::StringToDataType(yData["Type"].as<std::string>());
                        auto v = yData["Value"];

                        switch (type)
                        {
                        case Data::DataVec2:
                        {
                            Vector2 *data = new Vector2(v[0].as<float>(), v[1].as<float>());
                            c->Sets.push_back(new Data::Set(sizeof(Vector2), data, Data::DataVec2, name));
                            delete data;
                        }
                        break;

                        case Data::DataVec3:
                        {
                            Vector3 *data = new Vector3(v[0].as<float>(), v[1].as<float>(), v[2].as<float>());
                            c->Sets.push_back(new Data::Set(sizeof(Vector3), data, Data::DataVec3, name));
                            delete data;
                        }
                        break;

                        case Data::DataVec4:
                        {
                            Vector4 *data = new Vector4(v[0].as<float>(), v[1].as<float>(), v[2].as<float>(), v[3].as<float>());
                            c->Sets.push_back(new Data::Set(sizeof(Vector4), data, Data::DataVec4, name));
                            delete data;
                        }
                        break;

                        case Data::DataFloat:
                        {
                            Data::FloatContainer *data = new Data::FloatContainer(v.as<float>());
                            c->Sets.push_back(new Data::Set(sizeof(Data::FloatContainer), data, Data::DataFloat, name));
                            delete data;
                        }
                        break;

                        case Data::DataColor:
                        {
                            Color *data = new Color(v[0].as<float>(), v[1].as<float>(), v[2].as<float>(), v[3].as<float>());
                            c->Sets.push_back(new Data::Set(sizeof(Color), data, Data::DataColor, name));
                            delete data;
                        }
                        break;

                        case Data::DataNone:
                        default:
                            break;
                        }
                    }
                }

                if (physics)
                {
                    auto c = a->AddComponent<PhysicsComponent>();
                    YAMLToVcc3(physics["Velocity"], &c->Configuration.Velocity);
                    YAMLToVcc3(physics["Acceleration"], &c->Configuration.Acceleration);
                    YAMLToVcc3(physics["Size"], &c->Configuration.Size);
                    c->Configuration.Mass = physics["Mass"] ? physics["Mass"].as<float>() : 1.0f;
                    c->Configuration.Damping = physics["Damping"] ? physics["Damping"].as<float>() : 0.9f;
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