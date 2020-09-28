#include "Engine/pch.h"

#include "Camera.h"

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
}
