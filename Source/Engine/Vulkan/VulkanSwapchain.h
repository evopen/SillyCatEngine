#pragma once
#include "Engine/Platform/Platform.h"

class VulkanCommandBuffer;

class VulkanDevice;
class VulkanInstance;
class VulkanWindowSurface;

class VulkanSwapchain
{
public:
    API VulkanSwapchain(std::shared_ptr<VulkanInstance> inInstance, VulkanDevice* InDevice, std::shared_ptr<VulkanWindowSurface> InWindowSurface);
    API ~VulkanSwapchain();

    API VkSwapchainKHR GetSwapchainHandle() { return Swapchain; }
    API VkImage GetImage(uint32_t Index) { return Images[Index]; }
    API uint32_t GetRenderIndex() const { return RenderIndex; }
    API uint32_t GetImageCount() const { return static_cast<uint32_t>(Images.size()); }
    API void AcquireNextImage(VkSemaphore InSignalSemaphore, VkFence InSignalFence);

    API void CmdTransitImagePresentSrc(VulkanCommandBuffer* CmdBuffer);

    void FramebufferResizeCallback();

private:
    VkSwapchainKHR Swapchain;
    VulkanDevice* Device;
    std::shared_ptr<VulkanWindowSurface> WindowSurface;
    std::shared_ptr<VulkanInstance> Instance;
    std::vector<VkImage> Images;

    uint32_t RenderIndex;

    void Create();
    void Destroy();
};
