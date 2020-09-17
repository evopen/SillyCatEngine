#include "Engine/pch.h"

#include "VulkanDevice.h"
#include "VulkanSemaphore.h"

VulkanSemaphore::VulkanSemaphore(VulkanDevice* InDevice)
    : Device(InDevice)
    , Semaphore(VK_NULL_HANDLE)
{
    VkSemaphoreCreateInfo SemaphoreInfo = {
        .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
    };
    vkCreateSemaphore(Device->GetDeviceHandle(), &SemaphoreInfo, nullptr, &Semaphore);
}
