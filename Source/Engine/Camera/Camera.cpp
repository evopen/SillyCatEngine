#include "Engine/pch.h"

#include "Camera.h"
#include "Engine/Vulkan/VulkanWindowSurface.h"

namespace Sce
{
    Camera::Camera(glm::vec3 inPosition, float inYaw, float inPitch, glm::vec3 inWorldUp, float inAspect, float inFov)
        : Position(inPosition)
        , WorldUp(inWorldUp)
        , Yaw(inYaw)
        , Pitch(inPitch)
        , Fov(inFov)
        , Aspect(inAspect)
        , ProjectionMatrix(glm::perspective(glm::radians(Fov), Aspect, 0.1f, 10000.f))
    {
        UpdateCameraVectors();
    }

    Camera::Camera(glm::vec3 inPosition, glm::vec3 inLookAt, glm::vec3 inWorldUp, float inAspect, float inFov)
        : Position(inPosition)
        , WorldUp(inWorldUp)
        , Fov(inFov)
        , Aspect(inAspect)
        , ProjectionMatrix(glm::perspective(glm::radians(Fov), Aspect, 0.1f, 10000.f))
    {
        Front = inLookAt - Position;
        Pitch = std::clamp(glm::degrees(acos(Front.z / glm::length(Front))), -89.0f, 89.0f);
        Yaw   = glm::degrees(acos(Front.x / glm::length(Front)));
        UpdateCameraVectors();
    }


    void Camera::CursorPosCallback(GLFWwindow* window, double x, double y)
    {
        spdlog::warn("numerous{} {}", x, y);
    }

    void Camera::ProcessMouseMovement(float inYawOffset, float inPitchOffset)
    {
        Yaw += inYawOffset;
        Pitch += inPitchOffset;

        // Make sure that when pitch is out of bounds, screen doesn't get flipped
        if (Pitch > 89.0f)
            Pitch = 89.0f;
        else if (Pitch < -89.0f)
            Pitch = -89.0f;

        // Update Front, Right and Up Vectors using the updated Euler angles
        UpdateCameraVectors();
    }

    void Camera::UpdateCameraVectors()
    {
        // Calculate the new Front vector
        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        Front   = glm::normalize(front);
        // Also re-calculate the Right and Up vector
        Right = glm::normalize(glm::cross(Front, WorldUp));
        // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        Up = glm::normalize(glm::cross(Right, Front));
    }
}
