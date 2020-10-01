#include "Engine/pch.h"

#include "Engine/Camera/Camera.h"
#include "Engine/Vulkan/VulkanWindowSurface.h"
#include "Input.h"

namespace Sce
{
    Input::Input(std::shared_ptr<VulkanWindowSurface> WindowSurface)
        : WindowSurface(WindowSurface)
        , Camera(nullptr)
        , KeyboardMovementSpeed(0.5)
    {
    }

    void Input::ProcessKeyboard()
    {
        if (Camera)
        {
            if (glfwGetKey(WindowSurface->GetWindowHandle(), GLFW_KEY_W) == GLFW_PRESS)
            {
                Camera->ProcessKeyboard(Sce::Camera::Direction::Forward, KeyboardMovementSpeed);
            }
            else if (glfwGetKey(WindowSurface->GetWindowHandle(), GLFW_KEY_S) == GLFW_PRESS)
            {
                Camera->ProcessKeyboard(Sce::Camera::Direction::Backward, KeyboardMovementSpeed);
            }
            else if (glfwGetKey(WindowSurface->GetWindowHandle(), GLFW_KEY_A) == GLFW_PRESS)
            {
                Camera->ProcessKeyboard(Sce::Camera::Direction::Left, KeyboardMovementSpeed);
            }
            else if (glfwGetKey(WindowSurface->GetWindowHandle(), GLFW_KEY_D) == GLFW_PRESS)
            {
                Camera->ProcessKeyboard(Sce::Camera::Direction::Right, KeyboardMovementSpeed);
            }
            else if (glfwGetKey(WindowSurface->GetWindowHandle(), GLFW_KEY_Q) == GLFW_PRESS)
            {
                Camera->ProcessKeyboard(Camera::Direction::Down, KeyboardMovementSpeed);
            }
            else if (glfwGetKey(WindowSurface->GetWindowHandle(), GLFW_KEY_E) == GLFW_PRESS)
            {
                Camera->ProcessKeyboard(Camera::Direction::Up, KeyboardMovementSpeed);
            }
        }

        if (glfwGetKey(WindowSurface->GetWindowHandle(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
            std::exit(0);
    }

    void Input::LinkKeyboardCameraMovement(Sce::Camera* inCamera, float inKeyboardMovementSpeed)
    {
        KeyboardMovementSpeed = inKeyboardMovementSpeed;
        Camera                = inCamera;
    }
}
