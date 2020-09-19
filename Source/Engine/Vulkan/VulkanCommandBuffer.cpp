#include "Engine/pch.h"

#include "VulkanCommandBuffer.h"
#include "VulkanDevice.h"
#include "VulkanFence.h"
#include "VulkanFramebuffer.h"
#include "VulkanQueue.h"
#include "VulkanRenderPass.h"
#include "VulkanRenderTargetLayout.h"
#include "VulkanSemaphore.h"

VulkanCommandBuffer::VulkanCommandBuffer(VulkanDevice* InDevice, VulkanQueue* InQueue)
    : Device(InDevice)
    , Queue(InQueue)
    , CommandBuffer(VK_NULL_HANDLE)
{
    VkCommandBufferAllocateInfo AllocateInfo = {
        .sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .commandPool        = Queue->GetCommandPoolHandle(),
        .level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        .commandBufferCount = 1,
    };
    vkAllocateCommandBuffers(Device->GetDeviceHandle(), &AllocateInfo, &CommandBuffer);
}

void VulkanCommandBuffer::Begin() const
{
    VkCommandBufferBeginInfo BeginInfo = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
    };
    vkBeginCommandBuffer(CommandBuffer, &BeginInfo);
}

void VulkanCommandBuffer::End() const
{
    vkEndCommandBuffer(CommandBuffer);
}

void VulkanCommandBuffer::BeginRenderPass(VulkanRenderPass* InRenderPass, std::shared_ptr<VulkanFramebuffer> InFramebuffer) const
{
    std::vector<VkClearValue> ClearValues(InRenderPass->GetRenderTargetLayout()->GetColorAttachmentReferenceCount(),
        {1, 1, 1, 1});

    VkRenderPassBeginInfo BeginInfo = {
        .sType       = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
        .renderPass  = InRenderPass->GetRenderPassHandle(),
        .framebuffer = InFramebuffer->GetHandle(),
        .renderArea  = {
            .offset = {
                .x = 0,
                .y = 0,
            },
            .extent = {
                .width  = InFramebuffer->GetWidth(),
                .height = InFramebuffer->GetHeight(),
            },
        },
        .clearValueCount = static_cast<uint32_t>(ClearValues.size()),
        .pClearValues    = ClearValues.data(),
    };
    vkCmdBeginRenderPass(CommandBuffer, &BeginInfo, VK_SUBPASS_CONTENTS_INLINE);
}

void VulkanCommandBuffer::EndRenderPass() const
{
    vkCmdEndRenderPass(CommandBuffer);
}

void VulkanCommandBuffer::Reset() const
{
    vkResetCommandBuffer(CommandBuffer, 0);
}

void VulkanCommandBuffer::Submit(std::vector<VkSemaphore> InWaitSemaphores, std::vector<VkPipelineStageFlags> InWaitStages, std::vector<std::shared_ptr<VulkanSemaphore>> InSignalSemaphores, VulkanFence* Fence) const
{
    std::vector<VkSemaphore> SignalSemaphores(InSignalSemaphores.size());
    for (uint32_t i = 0; i < InSignalSemaphores.size(); i++)
    {
        SignalSemaphores[i] = InSignalSemaphores[i]->GetHandle();
    }
    VkSubmitInfo SubmitInfo = {
        .sType                = VK_STRUCTURE_TYPE_SUBMIT_INFO,
        .waitSemaphoreCount   = static_cast<uint32_t>(InWaitSemaphores.size()),
        .pWaitSemaphores      = InWaitSemaphores.data(),
        .pWaitDstStageMask    = InWaitStages.data(),
        .commandBufferCount   = 1,
        .pCommandBuffers      = &CommandBuffer,
        .signalSemaphoreCount = static_cast<uint32_t>(SignalSemaphores.size()),
        .pSignalSemaphores    = SignalSemaphores.data(),
    };
    vkQueueSubmit(Queue->GetHandle(), 1, &SubmitInfo, Fence ? Fence->GetHandle() : VK_NULL_HANDLE);
}
