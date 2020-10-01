#pragma once

#include "Engine/pch.h"

#include "Engine/Platform/Platform.h"

namespace Sce
{
    class Camera;


    class Input
    {
    public:
        API Input(std::shared_ptr<VulkanWindowSurface> inWindowSurface);


        // continuously poll every frame
        API void ProcessKeyboard();

        API void LinkKeyboardCameraMovement(Sce::Camera* inCamera, float inKeyboardMovementSpeed);

    private:
        std::shared_ptr<VulkanWindowSurface> WindowSurface;

        Sce::Camera* Camera;
        float KeyboardMovementSpeed;
    };
}
