#include "Engine/pch.h"

#include "VulkanDevice.h"
#include "VulkanFramebuffer.h"
#include "VulkanRenderPass.h"

VulkanFramebuffer::VulkanFramebuffer(VulkanDevice* InDevice, VulkanRenderPass* InRenderPass, uint32_t InWidth, uint32_t InHeight)
    : Device(InDevice)
    , RenderPass(InRenderPass)
    , Framebuffer(VK_NULL_HANDLE)
    , Width(InWidth)
    , Height(InHeight)
{
    VkFramebufferCreateInfo FramebufferInfo = {
        .sType      = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
        .renderPass = RenderPass->GetRenderPassHandle(),
        .width      = Width,
        .height     = Height,
        .layers     = 1,
    };

    vkCreateFramebuffer(Device->GetDeviceHandle(), &FramebufferInfo, nullptr, &Framebuffer);
}
