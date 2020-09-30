#include "Engine/pch.h"

#include "Camera.h"
#include "Engine/Logger/Logger.h"

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
        Pitch = std::clamp(glm::degrees(asin(Front.y / glm::length(Front))), -89.0f, 89.0f);
        Yaw   = glm::degrees(acos(Front.x / glm::length(Front)));
        UpdateCameraVectors();
    }


    void Camera::CursorPosCallback(GLFWwindow* window, double x, double y)
    {
        Logger::Get()->debug("MousePos:{} {}", x, y);

        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
        {
            if (!PressingRightButton)
            {
                PressingRightButton = true;
            }
            else
            {
                ProcessMouseMovement((LastMousePos.x - static_cast<float>(x)) * 0.08f, (static_cast<float>(y) - LastMousePos.y) * 0.08f);
            }
            LastMousePos.x = static_cast<float>(x);
            LastMousePos.y = static_cast<float>(y);
        }
        else
        {
            PressingRightButton = false;
        }
    }

    void Camera::ProcessMouseMovement(float inYawOffset, float inPitchOffset)
    {
        Yaw += inYawOffset;
        Pitch += inPitchOffset;

        // Make sure that when pitch is out of bounds, screen doesn't get flipped
        Pitch = std::clamp(Pitch, -89.f, 89.f);

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
