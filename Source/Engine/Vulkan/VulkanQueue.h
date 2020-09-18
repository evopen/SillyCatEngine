#pragma once

#include <vulkan/vulkan.h>

class VulkanDevice;

class VulkanQueue
{
public:
    VulkanQueue(VulkanDevice* inDevice, uint32_t inFamilyIndex);
    uint32_t GetFamilyIndex() const { return FamilyIndex; }
    VkCommandPool GetCommandPoolHandle() { return CommandPool; }
    VkQueue GetHandle() { return Queue; }

private:
    VkQueue Queue;
    VulkanDevice* Device;
    uint32_t FamilyIndex;
    uint32_t QueueIndex;
    VkCommandPool CommandPool;
};