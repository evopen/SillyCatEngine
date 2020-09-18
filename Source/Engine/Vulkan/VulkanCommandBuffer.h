#pragma once
#include "Engine/pch.h"

#include "Engine/Platform/Platform.h"

class VulkanFramebuffer;
class VulkanRenderPass;
class VulkanQueue;
class VulkanDevice;

class VulkanCommandBuffer
{
public:
    API VulkanCommandBuffer(VulkanDevice* InDevice, VulkanQueue* InQueue);

    void Begin() const;
    void End() const;
    void BeginRenderPass(VulkanRenderPass* InRenderPass, VulkanFramebuffer* InFramebuffer) const;
    void EndRenderPass() const;

private:
    VulkanDevice* Device;
    VulkanQueue* Queue;
    VkCommandBuffer CommandBuffer;
    bool bHasBegun;
};
