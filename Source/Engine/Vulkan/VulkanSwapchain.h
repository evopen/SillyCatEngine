#pragma once
#include "Engine/Platform/Platform.h"

#include <vulkan/vulkan.h>

class VulkanDevice;
class VulkanInstance;
class VulkanWindowSurface;

class VulkanSwapchain
{
public:
    API VulkanSwapchain(VulkanInstance* InInstance, VulkanDevice* InDevice, VulkanWindowSurface* InWindowSurface);

    API VkSwapchainKHR GetSwapchainHandle() { return Swapchain; }
    API VkImage GetImage(uint32_t Index) { return Images[Index]; }
    API uint32_t GetRenderIndex() const { return RenderIndex; }
    API uint32_t GetImageCount() const { return static_cast<uint32_t>(Images.size()); }
    API void AcquireNextImage(VkSemaphore InSignalSemaphore, VkFence InSignalFence);

private:
    VkSwapchainKHR Swapchain;
    VulkanDevice* Device;
    VulkanWindowSurface* WindowSurface;
    VulkanInstance* Instance;
    std::vector<VkImage> Images;

    uint32_t RenderIndex;
};
