#include "Engine/pch.h"

#include "VulkanCommandBuffer.h"
#include "VulkanDevice.h"
#include "VulkanSwapchain.h"
#include "VulkanUtil.h"
#include "VulkanWindowSurface.h"

VulkanSwapchain::VulkanSwapchain(
    std::shared_ptr<VulkanInstance> inInstance, VulkanDevice* InDevice, VulkanWindowSurface* InWindowSurface)
    : WindowSurface(InWindowSurface)
    , Instance(inInstance)
    , RenderIndex(0)
    , Device(InDevice)
{
    Create();

    WindowSurface->AddSwapchain(this);
}

VulkanSwapchain::~VulkanSwapchain()
{
    WindowSurface->RemoveSwapchain(this);
}

void VulkanSwapchain::AcquireNextImage(VkSemaphore InSignalSemaphore, VkFence InSignalFence)
{
    auto Result = vkAcquireNextImageKHR(Device->GetDeviceHandle(), Swapchain, std::numeric_limits<uint32_t>::max(), InSignalSemaphore, InSignalFence, &RenderIndex);
    if (Result == VK_ERROR_OUT_OF_DATE_KHR || VK_SUBOPTIMAL_KHR)
    {
    }
}

void VulkanSwapchain::CmdTransitImagePresentSrc(VulkanCommandBuffer* CmdBuffer)
{
    if (!CmdBuffer->IsRecording())
    {
        throw std::invalid_argument("CommandBuffer is not currently recording");
    }
    VkImageMemoryBarrier ImgMemBarrier = {
        .sType            = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
        .srcAccessMask    = 0,
        .dstAccessMask    = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT,
        .oldLayout        = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
        .newLayout        = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
        .image            = Images[RenderIndex],
        .subresourceRange = {
            .aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT,
            .baseMipLevel   = 0,
            .levelCount     = 1,
            .baseArrayLayer = 0,
            .layerCount     = 1,
        },
    };

    vkCmdPipelineBarrier(CmdBuffer->GetHandle(),
        VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
        0,
        0, nullptr,
        0, nullptr,
        1, &ImgMemBarrier);
}

void VulkanSwapchain::FramebufferResizeCallback()
{
    Destroy();
    Create();
}

void VulkanSwapchain::Create()
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
    vkCreateSwapchainKHR(Device->GetDeviceHandle(), &SwapchainInfo, nullptr, &Swapchain);

    uint32_t ImageCount;
    vkGetSwapchainImagesKHR(Device->GetDeviceHandle(), Swapchain, &ImageCount, nullptr);
    spdlog::info("Swapchain has {} images.", ImageCount);
    Images.resize(ImageCount);
    vkGetSwapchainImagesKHR(Device->GetDeviceHandle(), Swapchain, &ImageCount, Images.data());
}

void VulkanSwapchain::Destroy()
{
    vkDestroySwapchainKHR(Device->GetDeviceHandle(), Swapchain, nullptr);
}
