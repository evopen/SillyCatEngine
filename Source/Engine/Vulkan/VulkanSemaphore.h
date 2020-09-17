#pragma once

class VulkanDevice;

class VulkanSemaphore
{
public:
    API VulkanSemaphore(VulkanDevice* InDevice);
    VkSemaphore GetHandle() { return Semaphore; }

private:
    VulkanDevice* Device;
    VkSemaphore Semaphore;
};