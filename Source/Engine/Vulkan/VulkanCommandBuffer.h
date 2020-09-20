#pragma once
#include "Engine/pch.h"

#include "Engine/Platform/Platform.h"

class VulkanSemaphore;
class VulkanFence;
class VulkanFramebuffer;
class VulkanRenderPass;
class VulkanQueue;
class VulkanDevice;

class VulkanCommandBuffer
{
public:
    API VulkanCommandBuffer(VulkanDevice* InDevice, VulkanQueue* InQueue);
    API ~VulkanCommandBuffer();

    API void Begin();
    API void End();
    API void BeginRenderPass(VulkanRenderPass* InRenderPass, std::shared_ptr<VulkanFramebuffer> InFramebuffer) const;
    API void EndRenderPass() const;
    API void Reset() const;
    API bool IsRecording() const { return bIsRecording; }
    API void Wait() const;

    API VkCommandBuffer GetHandle() { return CommandBuffer; }

    API void Submit(std::vector<VkSemaphore> InWaitSemaphores = {}, std::vector<VkPipelineStageFlags> InWaitStages = {}, std::vector<std::shared_ptr<VulkanSemaphore>> InSignalSemaphores = {}) const;

private:
    VulkanDevice* Device;
    VulkanQueue* Queue;
    VkCommandBuffer CommandBuffer;
    bool bIsRecording;
    std::unique_ptr<VulkanFence> Fence;
};
