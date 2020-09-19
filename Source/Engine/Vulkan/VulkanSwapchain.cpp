#include "Engine/pch.h"

#include "VulkanDevice.h"
#include "VulkanSwapchain.h"
#include "VulkanUtil.h"
#include "VulkanWindowSurface.h"

#include <vulkan/vulkan.h>

VulkanSwapchain::VulkanSwapchain(
    VulkanInstance* InInstance, VulkanDevice* InDevice, VulkanWindowSurface* InWindowSurface)
    : WindowSurface(InWindowSurface)
    , Instance(InInstance)
    , RenderIndex(0)
    , Device(InDevice)
{
    VkSwapchainCreateInfoKHR SwapchainInfo = {
        .sType            = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
        .surface          = WindowSurface->GetSurfaceHandle(),
        .minImageCount    = 2,
        .imageFormat      = VK_FORMAT_B8G8R8A8_SRGB,
        .imageColorSpace  = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR,
        .imageExtent      = WindowSurface->SurfaceProperties.currentExtent,
        .imageArrayLayers = 1,
        .imageUsage =
            VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT,
        .imageSharingMode = VK_SHARING_MODE_EXCLUSIVE,
        .preTransform     = WindowSurface->SurfaceProperties.currentTransform,
        .compositeAlpha   = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
        .presentMode      = VK_PRESENT_MODE_FIFO_KHR,
        .clipped          = VK_TRUE,
    };
    CheckResult(vkCreateSwapchainKHR(Device->GetDeviceHandle(), &SwapchainInfo, nullptr, &Swapchain));

    uint32_t ImageCount;
    vkGetSwapchainImagesKHR(Device->GetDeviceHandle(), Swapchain, &ImageCount, nullptr);
    spdlog::info("Swapchain has {} images.", ImageCount);
    Images.resize(ImageCount);
    vkGetSwapchainImagesKHR(Device->GetDeviceHandle(), Swapchain, &ImageCount, Images.data());
}

void VulkanSwapchain::AcquireNextImage(VkSemaphore InSignalSemaphore, VkFence InSignalFence)
{
    vkAcquireNextImageKHR(Device->GetDeviceHandle(), Swapchain, std::numeric_limits<uint32_t>::max(), InSignalSemaphore, InSignalFence, &RenderIndex);
}
