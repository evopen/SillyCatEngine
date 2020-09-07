#include "VulkanSwapchain.h"

#include "VulkanDevice.h"
#include "VulkanUtil.h"
#include "VulkanWindowSurface.h"

#include <vulkan/vulkan.h>

VulkanSwapchain::VulkanSwapchain(
    VulkanInstance* InInstance, VulkanDevice* InDevice, VulkanWindowSurface* InWindowSurface)
    : WindowSurface(InWindowSurface)
    , Instance(InInstance)
    , Device(InDevice)
{
    VkSwapchainCreateInfoKHR SwapchainInfo = {.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
        .surface                                     = WindowSurface->GetSurfaceHandle(),
        .minImageCount                               = 3,
        .imageFormat                                 = VK_FORMAT_B8G8R8A8_SRGB,
        .imageColorSpace                             = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR,
        .imageExtent                                 = WindowSurface->SurfaceProperties.currentExtent,
        .imageArrayLayers                            = 1,
        .imageUsage =
            VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT,
        .imageSharingMode = VK_SHARING_MODE_EXCLUSIVE,
        .preTransform     = WindowSurface->SurfaceProperties.currentTransform,
        .compositeAlpha   = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
        .presentMode      = VK_PRESENT_MODE_MAILBOX_KHR,
        .clipped          = VK_TRUE};
    CheckResult(vkCreateSwapchainKHR(Device->GetDeviceHandle(), &SwapchainInfo, nullptr, &Swapchain));
}
