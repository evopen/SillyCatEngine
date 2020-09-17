#include "Engine/pch.h"

#include "VulkanDevice.h"
#include "VulkanRenderPass.h"
#include "VulkanRenderTargetLayout.h"

VulkanRenderPass::VulkanRenderPass(VulkanDevice* InDevice, VulkanRenderTargetLayout* InRTLayout)
    : Device(InDevice)
    , RTLayout(InRTLayout)
{
    VkSubpassDescription MainSubpass = {
        .pipelineBindPoint       = VK_PIPELINE_BIND_POINT_GRAPHICS,
        .inputAttachmentCount    = RTLayout->GetInputAttachmentReferenceCount(),
        .pInputAttachments       = RTLayout->GetInputAttachmentReferences(),
        .colorAttachmentCount    = RTLayout->GetColorAttachmentReferenceCount(),
        .pColorAttachments       = RTLayout->GetColorAttachmentReferences(),
        .pResolveAttachments     = RTLayout->GetResolveAttachmentReferences(),
        .pDepthStencilAttachment = RTLayout->GetDepthStencilAttachmentReference(),
    };

    VkRenderPassCreateInfo RenderPassInfo = {
        .sType           = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
        .attachmentCount = RTLayout->GetAttachmentDescriptionCount(),
        .pAttachments    = RTLayout->GetAttachmentDescriptions(),
        .subpassCount    = 1,
        .pSubpasses      = &MainSubpass,
    };
    vkCreateRenderPass(Device->GetDeviceHandle(), &RenderPassInfo, nullptr, &RenderPass);
}
