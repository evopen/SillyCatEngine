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

    API void InstallCursorCallback(std::function<void(GLFWwindow* window, double x, double y)> inCallback);
    API void InstallMouseButtonCallback(std::function<void(GLFWwindow* window, int button, int action, int mods)> inCallback);
    API void InstallKeyCallback(std::function<void(GLFWwindow* window, int key, int scancode, int action, int mods)> inCallback);


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

    std::vector<std::function<void(GLFWwindow* window, double x, double y)>> CursorPosCallbackList;
    std::vector<std::function<void(GLFWwindow* window, int button, int action, int mods)>> MouseButtonCallbackList;
    std::vector<std::function<void(GLFWwindow* window, int key, int scancode, int action, int mods)>> KeyCallbackList;

    void FramebufferResizeCallback(int InWidth, int InHeight);
    void CursorPosCallback(GLFWwindow* window, double x, double y);
    void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void StaticFramebufferResizeCallback(GLFWwindow* Window, int InWidth, int InHeight);
    static void StaticCursorPosCallback(GLFWwindow* window, double x, double y);
    static void StaticMouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    static void StaticKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
};
