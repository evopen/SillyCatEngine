#include "Engine/pch.h"

#include "VulkanDevice.h"
#include "VulkanQueue.h"

VulkanQueue::VulkanQueue(VulkanDevice* inDevice, uint32_t inFamilyIndex)
    : Queue(VK_NULL_HANDLE)
    , Device(inDevice)
    , FamilyIndex(inFamilyIndex)
    , QueueIndex(0)

{
    vkGetDeviceQueue(Device->GetDeviceHandle(), FamilyIndex, QueueIndex, &Queue);
    VkCommandPoolCreateInfo CommandPoolInfo = {
        .sType            = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
        .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
        .queueFamilyIndex = FamilyIndex,
    };
    vkCreateCommandPool(Device->GetDeviceHandle(), &CommandPoolInfo, nullptr, &CommandPool);
}
