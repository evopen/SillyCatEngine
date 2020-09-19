#include "Engine/pch.h"

#include "VulkanDevice.h"
#include "VulkanFramebuffer.h"
#include "VulkanImageView.h"
#include "VulkanRenderPass.h"
#include "VulkanRenderTargetLayout.h"

VulkanFramebuffer::VulkanFramebuffer(VulkanDevice* InDevice, VulkanRenderPass* InRenderPass, std::vector<std::shared_ptr<VulkanImageView>> InImageViews, uint32_t InWidth, uint32_t InHeight)
    : Device(InDevice)
    , RenderPass(InRenderPass)
    , Framebuffer(VK_NULL_HANDLE)
    , Width(InWidth)
    , Height(InHeight)
    , ImageViews(InImageViews)
{
    std::vector<VkImageView> ImageViewTmp(ImageViews.size());
    for (uint32_t i = 0; i < ImageViews.size(); ++i)
    {
        ImageViewTmp[i] = ImageViews[i].get()->GetHandle();
    }

    VkFramebufferCreateInfo FramebufferInfo = {
        .sType           = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
        .renderPass      = RenderPass->GetRenderPassHandle(),
        .attachmentCount = static_cast<uint32_t>(ImageViewTmp.size()),
        .pAttachments    = ImageViewTmp.data(),
        .width           = Width,
        .height          = Height,
        .layers          = 1,
    };

    vkCreateFramebuffer(Device->GetDeviceHandle(), &FramebufferInfo, nullptr, &Framebuffer);
}
