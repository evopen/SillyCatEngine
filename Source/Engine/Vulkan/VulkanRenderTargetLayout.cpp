#include "Engine/pch.h"

#include "VulkanRenderTargetLayout.h"

VulkanRenderTargetLayout::VulkanRenderTargetLayout(uint32_t ColorRenderTargetCount, bool inUseDepthBuffer)
{
    ColorReferences.reserve(ColorRenderTargetCount);
    Descriptions.reserve(ColorRenderTargetCount);

    uint32_t attachmentIndex = 0;

    for (uint32_t i = 0; i < ColorRenderTargetCount; i++)
    {
        VkAttachmentReference ColorAttachmentRef = {
            .attachment = attachmentIndex++,
            .layout     = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
        };
        ColorReferences.push_back(ColorAttachmentRef);

        VkAttachmentDescription ColorAttachmentDesc = {
            .format        = VK_FORMAT_B8G8R8A8_SRGB,
            .samples       = VK_SAMPLE_COUNT_1_BIT,
            .loadOp        = VK_ATTACHMENT_LOAD_OP_CLEAR,
            .storeOp       = VK_ATTACHMENT_STORE_OP_STORE,
            .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
            .finalLayout   = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
        };
        Descriptions.push_back(ColorAttachmentDesc);
    }

    if (inUseDepthBuffer)
    {
        DepthStencilReference = {
            .attachment = attachmentIndex++,
            .layout     = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
        };
        VkAttachmentDescription DepthAttachmentDesc = {
            .format        = VK_FORMAT_D32_SFLOAT,
            .samples       = VK_SAMPLE_COUNT_1_BIT,
            .loadOp        = VK_ATTACHMENT_LOAD_OP_CLEAR,
            .storeOp       = VK_ATTACHMENT_STORE_OP_DONT_CARE,
            .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
            .finalLayout   = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
        };
        Descriptions.push_back(DepthAttachmentDesc);
    }
}
