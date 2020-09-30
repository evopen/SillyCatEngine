#include "Engine/pch.h"

#include "VulkanDevice.h"
#include "VulkanInstance.h"
#include "VulkanQueue.h"
#include "VulkanSwapchain.h"
#include "VulkanUtil.h"
#include "VulkanWindowSurface.h"


VulkanWindowSurface::VulkanWindowSurface(
    std::shared_ptr<VulkanInstance> InInstance, VulkanDevice* InDevice, std::string InWindowName, uint32_t InWidth, uint32_t InHeight)
    : Surface(VK_NULL_HANDLE)
    , Instance(InInstance)
    , Window(nullptr)
    , Width(InWidth)
    , Height(InHeight)
    , Device(InDevice)
    , WindowName(InWindowName)
    , SurfaceProperties()
{
    CreateWindow();
    CreateSurface();
}

void VulkanWindowSurface::CreateWindow()
{
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    Window = glfwCreateWindow(Width, Height, WindowName.c_str(), nullptr, nullptr);
    if (!Window)
        throw std::runtime_error("Failed to create window");
    CenterWindow();
    glfwSetWindowUserPointer(Window, this);
    glfwSetFramebufferSizeCallback(Window, StaticFramebufferResizeCallback);
    glfwSetCursorPosCallback(Window, StaticCursorPosCallback);
    glfwSetKeyCallback(Window, StaticKeyCallback);
}

void VulkanWindowSurface::CreateSurface()
{
    CheckResult(glfwCreateWindowSurface(Instance->GetInstanceHandle(), Window, nullptr, &Surface));
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(Device->GetPhysicalDeviceHandle(), Surface, &SurfaceProperties);
    vkGetPhysicalDeviceSurfaceSupportKHR(
        Device->GetPhysicalDeviceHandle(), Device->GetGraphicsQueue()->GetFamilyIndex(), Surface, &SupportPresent);
}

void VulkanWindowSurface::CenterWindow()
{
    GLFWmonitor* Monitor = glfwGetPrimaryMonitor();
    auto Mode            = glfwGetVideoMode(Monitor);
    int monitorX, monitorY;
    glfwGetMonitorPos(Monitor, &monitorX, &monitorY);
    glfwSetWindowPos(Window,
        monitorX + (Mode->width - Width) / 2,
        monitorY + (Mode->height - Height) / 2);
}

VkViewport VulkanWindowSurface::GetSurfaceViewport()
{
    return VkViewport{
        .x        = 0,
        .y        = static_cast<float>(Height),
        .width    = static_cast<float>(Width),
        .height   = -static_cast<float>(Height),
        .minDepth = 0.f,
        .maxDepth = 1.f,
    };
}

VkRect2D VulkanWindowSurface::GetSurfaceScissor()
{
    return VkRect2D{
        .offset = {
            .x = 0,
            .y = 0,
        },
        .extent = {
            .width  = Width,
            .height = Height,
        },
    };
}

void VulkanWindowSurface::InstallCursorCallback(std::function<void(GLFWwindow* window, double x, double y)> inCallback)
{
    CursorPosCallbackList.push_back(inCallback);
}

void VulkanWindowSurface::InstallMouseButtonCallback(std::function<void(GLFWwindow* window, int button, int action, int mods)> inCallback)
{
    MouseButtonCallbackList.push_back(inCallback);
}

void VulkanWindowSurface::InstallKeyCallback(std::function<void(GLFWwindow* window, int key, int scancode, int action, int mods)> inCallback)
{
    KeyCallbackList.push_back(inCallback);
}


void VulkanWindowSurface::FramebufferResizeCallback(int InWidth, int InHeight)
{
    Width  = InWidth;
    Height = InHeight;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(Device->GetPhysicalDeviceHandle(), Surface, &SurfaceProperties);
    for (VulkanSwapchain* Swapchain : static_cast<VulkanWindowSurface*>(glfwGetWindowUserPointer(Window))->GetSwapchains())
    {
        Swapchain->FramebufferResizeCallback();
    }
}

void VulkanWindowSurface::CursorPosCallback(GLFWwindow* window, double x, double y)
{
    for (auto& f : CursorPosCallbackList)
    {
        f(window, x, y);
    }
}

void VulkanWindowSurface::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    for (auto& f : MouseButtonCallbackList)
    {
        f(window, button, action, mods);
    }
}

void VulkanWindowSurface::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    for (auto& f : KeyCallbackList)
    {
        f(window, key, scancode, action, mods);
    }
}

void VulkanWindowSurface::StaticFramebufferResizeCallback(GLFWwindow* Window, int InWidth, int InHeight)
{
    spdlog::info("new framebuffer width: {}, height: {}", InWidth, InHeight);
    VulkanWindowSurface* WindowSurface = static_cast<VulkanWindowSurface*>(glfwGetWindowUserPointer(Window));
    WindowSurface->FramebufferResizeCallback(InWidth, InHeight);
}

void VulkanWindowSurface::StaticCursorPosCallback(GLFWwindow* window, double x, double y)
{
    VulkanWindowSurface* WindowSurface = static_cast<VulkanWindowSurface*>(glfwGetWindowUserPointer(window));
    WindowSurface->CursorPosCallback(window, x, y);
}

void VulkanWindowSurface::StaticMouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    VulkanWindowSurface* WindowSurface = static_cast<VulkanWindowSurface*>(glfwGetWindowUserPointer(window));
    WindowSurface->MouseButtonCallback(window, button, action, mods);
}

void VulkanWindowSurface::StaticKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    VulkanWindowSurface* WindowSurface = static_cast<VulkanWindowSurface*>(glfwGetWindowUserPointer(window));
    WindowSurface->KeyCallback(window, key, scancode, action, mods);
}
