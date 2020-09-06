#pragma once

#include "VulkanDevice.h"

#include "VulkanInstance.h"
#include "VulkanUtil.h"

VulkanDevice::VulkanDevice()
    : Instance(nullptr)
    , Device(VK_NULL_HANDLE)
    , PhysicalDevice(VK_NULL_HANDLE)
    , OffscreenRender(false)
{
}


void VulkanDevice::CreateDevice()
{
    VkDeviceCreateInfo deviceInfo{.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO, .};

    CheckResult(vkCreateDevice(PhysicalDevice, &deviceInfo, nullptr, &Device));
}

void VulkanDevice::Init()
{
    GetDeviceExtensions(DeviceExtension, OffscreenRender);
    CreateDevice();
}

void VulkanDevice::GetDeviceExtensions(std::vector<const char*>& deviceExtensions, bool OffscreenRender)
{
    if (!OffscreenRender)
        deviceExtensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
}

VkPhysicalDevice VulkanDevice::SelectPhysicalDevice(VulkanInstance instance)
{
    uint32_t numPhysicalDevices;
    vkEnumeratePhysicalDevices(instance.GetInstanceHandle(), &numPhysicalDevices, nullptr);
    std::vector<VkPhysicalDevice> physicalDevices(numPhysicalDevices);
    vkEnumeratePhysicalDevices(instance.GetInstanceHandle(), &numPhysicalDevices, physicalDevices.data());

    for (size_t i = 0; i < numPhysicalDevices; i++)
    {
        VkPhysicalDeviceProperties prop;
        vkGetPhysicalDeviceProperties(physicalDevices[i], &prop);
        if (prop.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
        {
            return physicalDevices[i];
        }
    }

    return VK_NULL_HANDLE;
}
