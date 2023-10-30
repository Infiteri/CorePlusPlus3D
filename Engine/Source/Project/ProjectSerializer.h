#pragma once

#include "Base.h"
#include "Project.h"

namespace Core
{
    class CE_API ProjectSerializer
    {
    public:
        Project *project;

        ProjectSerializer(Project *proj);
        ~ProjectSerializer();

        void Serialize(const std::string &path);
        void Deserialize(const std::string &path);
    };

}