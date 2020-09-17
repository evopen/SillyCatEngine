#include "Engine/pch.h"

#include "VulkanDevice.h"
#include "VulkanFence.h"

VulkanFence::VulkanFence(VulkanDevice* InDevice)
    : Device(InDevice)
    , Fence(VK_NULL_HANDLE)
{
    VkFenceCreateInfo FenceInfo = {
        .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
    };
    vkCreateFence(Device->GetDeviceHandle(), &FenceInfo, nullptr, &Fence);
}
