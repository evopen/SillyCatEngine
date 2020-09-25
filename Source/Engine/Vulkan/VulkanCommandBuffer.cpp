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


void VulkanCommandBuffer::Reset()
{
    vkResetCommandBuffer(CommandBuffer, 0);
    OwnedObjects.clear();
}

void VulkanCommandBuffer::Wait() const
{
    Fence->Wait();
}

void VulkanCommandBuffer::PossessObject(std::shared_ptr<void> inObject)
{
    OwnedObjects.emplace_back(std::move(inObject));
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
