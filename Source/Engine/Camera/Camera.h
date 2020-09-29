#pragma once
#include "Engine/pch.h"

#include "Engine/Platform/Platform.h"

class VulkanWindowSurface;

namespace Sce
{
    class Camera
    {
    public:
        API Camera(glm::vec3 inPosition, float inYaw, float inPitch, glm::vec3 inWorldUp = {0, 1, 0}, float inAspect = 4.0 / 3.0, float inFov = 60);
        API Camera(glm::vec3 inPosition, glm::vec3 inLookAt, glm::vec3 inWorldUp = {0, 1, 0}, float inAspect = 4.0 / 3.0, float inFov = 60);
        API glm::mat4 GetViewMatrix() const { return glm::lookAt(Position, Position + Front, Up); }
        API glm::mat4 GetProjectionMatrix() const { return ProjectionMatrix; }

        API void CursorPosCallback(GLFWwindow* window, double x, double y);

        void ProcessMouseMovement(float inYawOffset, float inPitchOffset);

    private:
        glm::vec3 Position;
        glm::vec3 Up;
        glm::vec3 Front;
        glm::vec3 Right;
        glm::vec3 WorldUp;

        std::optional<float> MouseSensitivity;

        float Yaw;
        float Pitch;

        float Fov;  //in degree
        float Aspect;

        glm::mat4 ProjectionMatrix;

        void UpdateCameraVectors();
    };
}
