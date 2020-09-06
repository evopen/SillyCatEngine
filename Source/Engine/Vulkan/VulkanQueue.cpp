#include "VulkanQueue.h"

#include "VulkanDevice.h"

VulkanQueue::VulkanQueue(VulkanDevice* inDevice, uint32_t inFamilyIndex)
    : Queue(VK_NULL_HANDLE)
    , Device(inDevice)
    , FamilyIndex(inFamilyIndex)
    , QueueIndex(0)

{
    vkGetDeviceQueue(Device->GetDeviceHandle(), FamilyIndex, QueueIndex, &Queue);
}
