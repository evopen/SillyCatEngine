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
    API void BeginRenderPass(std::shared_ptr<VulkanRenderPass> inRenderPass, std::shared_ptr<VulkanFramebuffer> inFramebuffer);
    API void EndRenderPass() const;
    API void Reset();
    API bool IsRecording() const { return bIsRecording; }
    API void Submit(std::vector<VkSemaphore> InWaitSemaphores = {}, std::vector<VkPipelineStageFlags> InWaitStages = {}, std::vector<VkSemaphore> InSignalSemaphores = {}) const;
    API void Wait() const;

    API VkCommandBuffer GetHandle() { return CommandBuffer; }


private:
    VulkanDevice* Device;
    VulkanQueue* Queue;
    VkCommandBuffer CommandBuffer;
    bool bIsRecording;
    std::unique_ptr<VulkanFence> Fence;

    std::vector<std::shared_ptr<VulkanFramebuffer>> OwnedFramebuffers;
    std::vector<std::shared_ptr<VulkanRenderPass>> OwnedRenderPass;
};
