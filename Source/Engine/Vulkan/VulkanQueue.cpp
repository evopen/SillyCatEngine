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
}
