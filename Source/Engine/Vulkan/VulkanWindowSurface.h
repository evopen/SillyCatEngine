#pragma once

#include "Engine/pch.h"

#include "Engine/Platform/Platform.h"

#undef CreateWindowW
#undef CreateWindow


class VulkanDevice;
class VulkanInstance;

class VulkanWindowSurface
{
public:
    API VulkanWindowSurface(VulkanInstance* InInstance, VulkanDevice* Device, std::string InWindowName,
        uint32_t InWidth, uint32_t InHeight);
    void CreateWindow();
    void CreateSurface();
    VkSurfaceKHR GetSurfaceHandle() { return Surface; }
    API GLFWwindow* GetWindowHandle() { return Window; }
    VkSurfaceCapabilitiesKHR SurfaceProperties;

    API uint32_t GetWidth() const { return Width; }
    API uint32_t GetHeight() const { return Height; }

private:
    VkSurfaceKHR Surface;
    VulkanInstance* Instance;
    VulkanDevice* Device;
    GLFWwindow* Window;
    uint32_t Width;
    uint32_t Height;
    std::string WindowName;
    VkBool32 SupportPresent;
};
