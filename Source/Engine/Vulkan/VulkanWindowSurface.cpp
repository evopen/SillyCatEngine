#include "Engine/pch.h"

#include "VulkanDevice.h"
#include "VulkanInstance.h"
#include "VulkanQueue.h"
#include "VulkanSwapchain.h"
#include "VulkanUtil.h"
#include "VulkanWindowSurface.h"

#include <GLFW/glfw3.h>

#include <stdexcept>

VulkanWindowSurface::VulkanWindowSurface(
    VulkanInstance* InInstance, VulkanDevice* InDevice, std::string InWindowName, uint32_t InWidth, uint32_t InHeight)
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

    glfwSetWindowUserPointer(Window, this);
    glfwSetFramebufferSizeCallback(Window, StaticFramebufferResizeCallback);
}

void VulkanWindowSurface::CreateSurface()
{
    CheckResult(glfwCreateWindowSurface(Instance->GetInstanceHandle(), Window, nullptr, &Surface));
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(Device->GetPhysicalDeviceHandle(), Surface, &SurfaceProperties);
    vkGetPhysicalDeviceSurfaceSupportKHR(
        Device->GetPhysicalDeviceHandle(), Device->GetGraphicsQueue()->GetFamilyIndex(), Surface, &SupportPresent);
}


void VulkanWindowSurface::FramebufferResizeCallback(int InWidth, int InHeight)
{
    Width = InWidth;
    Height = InHeight;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(Device->GetPhysicalDeviceHandle(), Surface, &SurfaceProperties);
    for (VulkanSwapchain* Swapchain : static_cast<VulkanWindowSurface*>(glfwGetWindowUserPointer(Window))->GetSwapchains())
    {
        Swapchain->FramebufferResizeCallback();
    }
}

void VulkanWindowSurface::StaticFramebufferResizeCallback(GLFWwindow* Window, int InWidth, int InHeight)
{
    spdlog::info("new framebuffer width: {}, height: {}", InWidth, InHeight);
    VulkanWindowSurface* WindowSurface = static_cast<VulkanWindowSurface*>(glfwGetWindowUserPointer(Window));
    WindowSurface->FramebufferResizeCallback(InWidth, InHeight);
}
