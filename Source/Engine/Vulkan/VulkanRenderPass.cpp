#include "Engine/pch.h"

#include "Engine/Concurrent/Thread.h"
#include "VulkanCommandBuffer.h"
#include "VulkanDevice.h"
#include "VulkanFramebuffer.h"
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

void VulkanRenderPass::Begin(VulkanCommandBuffer* inCmdBuffer, std::shared_ptr<VulkanFramebuffer> Framebuffer, std::vector<VkClearValue> inClearValues)
{
    VkRenderPassBeginInfo BeginInfo = {
        .sType       = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
        .renderPass  = RenderPass,
        .framebuffer = Framebuffer->GetHandle(),
        .renderArea  = {
            .offset = {
                .x = 0,
                .y = 0,
            },
            .extent = {
                .width  = Framebuffer->GetWidth(),
                .height = Framebuffer->GetHeight(),
            },
        },
        .clearValueCount = static_cast<uint32_t>(inClearValues.size()),
        .pClearValues    = inClearValues.data(),
    };
    vkCmdBeginRenderPass(inCmdBuffer->GetHandle(), &BeginInfo, VK_SUBPASS_CONTENTS_INLINE);
    inCmdBuffer->PossessObject(shared_from_this());
    inCmdBuffer->PossessObject(Framebuffer);
    BeganInCmdBuf = inCmdBuffer;
}

void VulkanRenderPass::End()
{
    if (BeganInCmdBuf.has_value())
    {
        vkCmdEndRenderPass(BeganInCmdBuf.value()->GetHandle());
    }
}

bool VulkanRenderPass::HasDepthBuffer()
{
    return RTLayout->GetDepthStencilAttachmentReference() ? true : false;
}
