#pragma once

#include "Engine/pch.h"

#include "Engine/Platform/Platform.h"

#undef CreateWindowW
#undef CreateWindow


class VulkanDevice;
class VulkanInstance;
class VulkanSwapchain;

class VulkanWindowSurface
{
public:
    API VulkanWindowSurface(std::shared_ptr<VulkanInstance> InInstance, VulkanDevice* Device, std::string InWindowName,
        uint32_t InWidth, uint32_t InHeight);
    void CreateWindow();
    void CreateSurface();
    VkSurfaceKHR GetSurfaceHandle() { return Surface; }
    API GLFWwindow* GetWindowHandle() { return Window; }
    VkSurfaceCapabilitiesKHR SurfaceProperties;

    API uint32_t GetWidth() const { return Width; }
    API uint32_t GetHeight() const { return Height; }

    API void CenterWindow();

    void AddSwapchain(VulkanSwapchain* InSwapchain) { Swapchains.insert(InSwapchain); }
    void RemoveSwapchain(VulkanSwapchain* InSwapchain) { Swapchains.erase(InSwapchain); }

    std::set<VulkanSwapchain*> GetSwapchains() { return Swapchains; }

    API VkViewport GetSurfaceViewport();
    API VkRect2D GetSurfaceScissor();


private:
    VkSurfaceKHR Surface;
    std::shared_ptr<VulkanInstance> Instance;
    VulkanDevice* Device;
    GLFWwindow* Window;
    uint32_t Width;
    uint32_t Height;
    std::string WindowName;
    VkBool32 SupportPresent;

    std::set<VulkanSwapchain*> Swapchains;

    void FramebufferResizeCallback(int InWidth, int InHeight);
    static void StaticFramebufferResizeCallback(GLFWwindow* Window, int InWidth, int InHeight);
};
