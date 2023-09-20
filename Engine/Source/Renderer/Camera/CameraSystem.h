#pragma once

#include "Base.h"
#include "PerspectiveCamera.h"

#include <string>

namespace Core
{
    /// @brief Simple wrapper around things like active camera, camera map and other things.
    class CE_API CameraSystem
    {
    public:
        CameraSystem(){};
        ~CameraSystem(){};

        static void Init();
        static void Shutdown();

        /// @brief Returns the active the camera, nullptr if there isn't one.
        /// The active camera can be set using the "::Activate" function and passing the camera's name as the argument.
        /// @return A pointer to a perspective camera or nullptr.
        static PerspectiveCamera *GetActive();

        /// @brief Returns the camera under this name or nullptr if no camera under this name exists.
        /// @note The system doesn't log a message to console for a camera-not-found situation, thats the user responsibility.
        /// Check for nullptr to see if its found or not.
        /// @param name The name to look for.
        /// @return A pointer to a perspective camera or nullptr.
        static PerspectiveCamera *Get(const std::string &name);

        /// @brief Creates and saves a camera under the provided name, then it returns the newly formed camera.
        /// @param name The name of the camera to save under.
        /// @param fov The field-of-view of the camera IN RADIANS. Use Math::DegToRad(...) to convert from a degree amount to a radian amount.
        /// @param aspect The camera's aspect. Use Engine::GetWindowAspect() to get the window aspect.
        /// @param near The near frustum of the camera. Use 0.01 for default.
        /// @param far The far frustum of the camera.
        /// @return A pointer to a camera object.
        static PerspectiveCamera *Generate(const std::string &name, float fov, float aspect, float near, float far);

        /// @brief ACtivates a camera. Outputs a error message if the camera isn't found.
        /// @param name The name of the camera to search for.
        static void Activate(const std::string &name);

        /// @brief Updates to projection of the camera under this name. Outputs a error message if the camera isn't found.
        /// @param name The name of the camera to search for.
        /// @param aspect A float that represents the camera's new aspect. Use Engine::GetWindowAspect() to get the window aspect.
        static void UpdateProjection(const std::string &name, float aspect);

        /// @brief Updates the active camera's aspect if any active camera exists.
        /// @param aspect A float that represents the camera's new aspect. Use Engine::GetWindowAspect() to get the window aspect.
        static void UpdateProjectionForActiveCamera(float aspect);
    };
}