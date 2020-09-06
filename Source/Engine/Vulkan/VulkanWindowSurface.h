#pragma once

#define GLFW_INCLUDE_VULKAN
#include "Engine/Platform/Platform.h"

#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

#include <string>
#include <vector>

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
    VkSurfaceCapabilitiesKHR SurfaceProperties;

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
