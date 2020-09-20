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

void VulkanCommandBuffer::BeginRenderPass(VulkanRenderPass* InRenderPass, std::shared_ptr<VulkanFramebuffer> InFramebuffer)
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
    Framebuffers.push_back(InFramebuffer);
}

void VulkanCommandBuffer::EndRenderPass() const
{
    vkCmdEndRenderPass(CommandBuffer);
}

void VulkanCommandBuffer::Reset()
{
    vkResetCommandBuffer(CommandBuffer, 0);
    Framebuffers.clear();
}

void VulkanCommandBuffer::Wait() const
{
    Fence->Wait();
}


void VulkanCommandBuffer::Submit(std::vector<VkSemaphore> InWaitSemaphores, std::vector<VkPipelineStageFlags> InWaitStages, std::vector<std::shared_ptr<VulkanSemaphore>> InSignalSemaphores) const
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
    Fence->Reset();
    vkQueueSubmit(Queue->GetHandle(), 1, &SubmitInfo, Fence->GetHandle());
}
