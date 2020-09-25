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
    , Fence(std::make_unique<VulkanFence>(InDevice))
{
    VkCommandBufferAllocateInfo AllocateInfo = {
        .sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .commandPool        = Queue->GetCommandPoolHandle(),
        .level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        .commandBufferCount = 1,
    };
    vkAllocateCommandBuffers(Device->GetDeviceHandle(), &AllocateInfo, &CommandBuffer);
}

VulkanCommandBuffer::~VulkanCommandBuffer()
{
    Fence->Wait();
    vkFreeCommandBuffers(Device->GetDeviceHandle(), Queue->GetCommandPoolHandle(), 1, &CommandBuffer);
}

void VulkanCommandBuffer::Begin()
{
    Reset();
    VkCommandBufferBeginInfo BeginInfo = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
    };
    vkBeginCommandBuffer(CommandBuffer, &BeginInfo);

    bIsRecording = true;
}

void VulkanCommandBuffer::End()
{
    vkEndCommandBuffer(CommandBuffer);

    bIsRecording = false;
}

void VulkanCommandBuffer::BeginRenderPass(std::shared_ptr<VulkanRenderPass> inRenderPass, std::shared_ptr<VulkanFramebuffer> inFramebuffer)
{
    std::vector<VkClearValue> ClearValues(inRenderPass->GetRenderTargetLayout()->GetColorAttachmentReferenceCount(),
        {1, 1, 1, 1});

    VkRenderPassBeginInfo BeginInfo = {
        .sType       = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
        .renderPass  = inRenderPass->GetRenderPassHandle(),
        .framebuffer = inFramebuffer->GetHandle(),
        .renderArea  = {
            .offset = {
                .x = 0,
                .y = 0,
            },
            .extent = {
                .width  = inFramebuffer->GetWidth(),
                .height = inFramebuffer->GetHeight(),
            },
        },
        .clearValueCount = static_cast<uint32_t>(ClearValues.size()),
        .pClearValues    = ClearValues.data(),
    };
    vkCmdBeginRenderPass(CommandBuffer, &BeginInfo, VK_SUBPASS_CONTENTS_INLINE);
    OwnedFramebuffers.push_back(std::move(inFramebuffer));
    OwnedRenderPass.push_back(std::move(inRenderPass));
}

void VulkanCommandBuffer::EndRenderPass() const
{
    vkCmdEndRenderPass(CommandBuffer);
}

void VulkanCommandBuffer::Reset()
{
    vkResetCommandBuffer(CommandBuffer, 0);
    OwnedFramebuffers.clear();
    OwnedRenderPass.clear();
}

void VulkanCommandBuffer::Wait() const
{
    Fence->Wait();
}


void VulkanCommandBuffer::Submit(std::vector<VkSemaphore> InWaitSemaphores, std::vector<VkPipelineStageFlags> InWaitStages, std::vector<VkSemaphore> InSignalSemaphores) const
{
    VkSubmitInfo SubmitInfo = {
        .sType                = VK_STRUCTURE_TYPE_SUBMIT_INFO,
        .waitSemaphoreCount   = static_cast<uint32_t>(InWaitSemaphores.size()),
        .pWaitSemaphores      = InWaitSemaphores.data(),
        .pWaitDstStageMask    = InWaitStages.data(),
        .commandBufferCount   = 1,
        .pCommandBuffers      = &CommandBuffer,
        .signalSemaphoreCount = static_cast<uint32_t>(InSignalSemaphores.size()),
        .pSignalSemaphores    = InSignalSemaphores.data(),
    };
    Fence->Reset();
    vkQueueSubmit(Queue->GetHandle(), 1, &SubmitInfo, Fence->GetHandle());
}
