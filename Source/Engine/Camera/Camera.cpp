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
        Yaw   = glm::degrees(asin(Front.z / glm::length(Front)));
        if (Front.z >= 0 && Front.x < 0)
        {
            Yaw = 180.f - Yaw;
        }
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
                ProcessMouseMovement((static_cast<float>(x) - LastMousePos.x) * 0.08f, (LastMousePos.y - static_cast<float>(y)) * 0.08f);
            }
            LastMousePos.x = static_cast<float>(x);
            LastMousePos.y = static_cast<float>(y);
        }
        else
        {
            PressingRightButton = false;
        }
    }

    void Camera::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        if (key == GLFW_KEY_W && action == GLFW_PRESS)
        {
            Position = Position + glm::normalize(Front);
        }
        else if (key == GLFW_KEY_S && action == GLFW_PRESS)
        {
            Position = Position - glm::normalize(Front);
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

    void Camera::ProcessKeyboard(Direction direction, float distance)
    {
        switch (direction)
        {
        case Direction::Forward:
            Position += Front * distance;
            break;
        case Direction::Backward:
            Position -= Front * distance;
            break;
        case Direction::Left:
            Position -= Right * distance;
            break;
        case Direction::Right:
            Position += Right * distance;
            break;
        case Direction::Up:
            Position += Up * distance;
            break;
        case Direction::Down:
            Position -= Up * distance;
            break;
        }
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
