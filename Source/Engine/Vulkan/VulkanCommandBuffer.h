#pragma once
#include "Engine/pch.h"

#include "Engine/Platform/Platform.h"

class VulkanFence;
class VulkanFramebuffer;
class VulkanRenderPass;
class VulkanQueue;
class VulkanDevice;

class VulkanCommandBuffer
{
public:
    API VulkanCommandBuffer(VulkanDevice* InDevice, VulkanQueue* InQueue);

    API void Begin() const;
    API void End() const;
    API void BeginRenderPass(VulkanRenderPass* InRenderPass, std::shared_ptr<VulkanFramebuffer> InFramebuffer) const;
    API void EndRenderPass() const;
    API void Reset() const;

    API VkCommandBuffer GetHandle() { return CommandBuffer; }

    API void Submit(std::vector<VkSemaphore> InWaitSemaphores = {}, std::vector<VkPipelineStageFlags> InWaitStages = {}, std::vector<VkSemaphore> InSignalSemaphores = {}, VulkanFence* Fence = nullptr) const;

private:
    VulkanDevice* Device;
    VulkanQueue* Queue;
    VkCommandBuffer CommandBuffer;
    bool bHasBegun;
};
