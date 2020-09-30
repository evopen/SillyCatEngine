#pragma once
#include "Engine/pch.h"

#include "Engine/Platform/Platform.h"

#include <iostream>

class VulkanWindowSurface;

namespace Sce
{
    class Camera
    {
    public:
        enum class Direction
        {
            Forward,
            Backward,
            Left,
            Right,
            Up,
            Down
        };

        API Camera(glm::vec3 inPosition, float inYaw, float inPitch, glm::vec3 inWorldUp = {0, 1, 0}, float inAspect = 4.0 / 3.0, float inFov = 60);
        API Camera(glm::vec3 inPosition, glm::vec3 inLookAt, glm::vec3 inWorldUp = {0, 1, 0}, float inAspect = 4.0 / 3.0, float inFov = 60);
        API glm::mat4 GetViewMatrix() const { return glm::lookAt(Position, Position + Front, Up); }
        API glm::mat4 GetProjectionMatrix() const { return ProjectionMatrix; }
        API glm::vec3 GetPosition() const { return Position; }
        API glm::vec3 GetFront() const { return Front; }
        API glm::vec3 GetRight() const { return Right; }
        API float GetYaw() const { return Yaw; }
        API float GetPitch() const { return Pitch; }
        API float GetFov() const { return Fov; }
        API float GetAspect() const { return Aspect; }

        API void CursorPosCallback(GLFWwindow* window, double x, double y);
        API void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

        void ProcessMouseMovement(float inYawOffset, float inPitchOffset);

        API void ProcessKeyboard(Direction direction);

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

        bool PressingRightButton;
        glm::vec2 LastMousePos;
        void UpdateCameraVectors();
    };
}
