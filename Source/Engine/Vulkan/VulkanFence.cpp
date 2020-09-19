#include "Engine/pch.h"

#include "VulkanDevice.h"
#include "VulkanFence.h"

VulkanFence::VulkanFence(VulkanDevice* InDevice)
    : Device(InDevice)
    , Fence(VK_NULL_HANDLE)
{
    VkFenceCreateInfo FenceInfo = {
        .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
        .flags = VK_FENCE_CREATE_SIGNALED_BIT,
    };
    vkCreateFence(Device->GetDeviceHandle(), &FenceInfo, nullptr, &Fence);
}

void VulkanFence::Wait() const
{
    vkWaitForFences(Device->GetDeviceHandle(), 1, &Fence, VK_TRUE, std::numeric_limits<uint64_t>::max());
}

void VulkanFence::Reset() const
{
    vkResetFences(Device->GetDeviceHandle(), 1, &Fence);
}
