#pragma once

#include <vulkan/vulkan.h>

#include <vector>

class VulkanInstance;

class VulkanDevice
{
public:
    VulkanDevice();

private:
    VulkanInstance* Instance;
    VkDevice Device;
    VkPhysicalDevice PhysicalDevice;
    bool OffscreenRender;
    std::vector<const char*> DeviceExtension;
    std::vector<const char*> DeviceLayers;
    VkPhysicalDeviceProperties GpuProps;

    void CreateDevice();
    void Init();

    static void GetDeviceLayers(std::vector<const char*>& deviceLayers);
    static void GetDeviceExtensions(std::vector<const char*>& deviceExtensions, bool OffscreenRender);
    static VkPhysicalDevice SelectPhysicalDevice(VulkanInstance instance);
};
