#include "Project.h"
#include "ProjectSerializer.h"

#include "Core/Logger.h"

namespace Core
{
    static Project *activeProject;

    Project::Project()
    {
    }

    Project::~Project()
    {
    }

    Project *Project::New()
    {
        CE_PROFILE_FUNCTION();

        if (activeProject)
        {
            delete activeProject;
        }

        activeProject = new Project();
        return activeProject;
    }
    
    Project* Project::GetInstance()
    {
        return activeProject;
    }

    Project *Project::Load(const std::string &path)
    {
        CE_PROFILE_FUNCTION();

        if (!activeProject)
            New();

        ProjectSerializer ser(activeProject);
        ser.Deserialize(path);

        return activeProject;
    }

    void Project::SaveActive(const std::string &path)
    {
        CE_PROFILE_FUNCTION();

        if (!activeProject)
        {
            CE_ERROR("Unable to save active project, no project loaded.");
            return;
        }

        ProjectSerializer ser(activeProject);
        ser.Serialize(path);
    }

    ProjectConfiguration *Project::GetConfig()
    {
        if (!activeProject)
            return nullptr;

        return &activeProject->config;
    }
}