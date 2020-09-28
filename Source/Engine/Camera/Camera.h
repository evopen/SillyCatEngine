#pragma once
#include "Engine/pch.h"

#include "Engine/Platform/Platform.h"

namespace Sce
{
    class Camera
    {
    public:
        API Camera(glm::vec3 inPosition, float inYaw, float inPitch, glm::vec3 inWorldUp = {0, 1, 0}, float inAspect = 4.0 / 3.0, float inFov = 60);
        API Camera(glm::vec3 inPosition, glm::vec3 inLookAt, glm::vec3 inWorldUp = {0, 1, 0}, float inAspect = 4.0 / 3.0, float inFov = 60);
        API glm::mat4 GetViewMatrix() const { return glm::lookAt(Position, Position + Front, Up); }
        API glm::mat4 GetProjectionMatrix() const { return ProjectionMatrix; }

        void ProcessMouseMovement(float inYawOffset, float inPitchOffset);

    private:
        glm::vec3 Position;
        glm::vec3 Up;
        glm::vec3 Front;
        glm::vec3 Right;
        glm::vec3 WorldUp;

        float Yaw;
        float Pitch;

        float Fov;  //in degree
        float Aspect;

        glm::mat4 ProjectionMatrix;

        void UpdateCameraVectors()
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
    };
}
