#pragma once

#include "Engine/Platform/Platform.h"

#include <vulkan/vulkan.h>

#include <vector>

class VulkanInstance;
class VulkanQueue;

class VulkanDevice
{
public:
    VulkanDevice();
    API VulkanDevice(VulkanInstance* inInstance, VkPhysicalDevice inPhysicalDevice, bool InOffscreenRender);
    API void Init();
    API void Destroy();
    API static std::tuple<VkPhysicalDevice, VkPhysicalDeviceProperties> SelectPhysicalDevice(VulkanInstance instance);
    VkDevice GetDeviceHandle() const { return Device; }
    VkPhysicalDevice GetPhysicalDeviceHandle() const { return PhysicalDevice; }
    VulkanQueue* GetGraphicsQueue() { return GraphicsQueue; }

private:
    VulkanInstance* Instance;
    VkDevice Device;
    VkPhysicalDevice PhysicalDevice;
    VulkanQueue* GraphicsQueue;
    VulkanQueue* TransferQueue;
    VulkanQueue* ComputeQueue;
    bool OffscreenRender;
    std::vector<const char*> DeviceExtensions;
    std::vector<const char*> DeviceLayers;
    VkPhysicalDeviceProperties PhysicalDeviceProp;
    std::vector<VkQueueFamilyProperties> QueueFamilyProps;

    void CreateDevice();

    // static void GetDeviceLayers(std::vector<const char*>& deviceLayers);
    static void GetDeviceExtensions(std::vector<const char*>& deviceExtensions, bool OffscreenRender);
    static void GetQueueProperties(
        VkPhysicalDevice physicalDevice, std::vector<VkQueueFamilyProperties>& queueProperties);
};
