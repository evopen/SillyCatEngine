#pragma once

#include "Engine/Platform/Platform.h"

#include <vulkan/vulkan.h>

#include <vector>

class VulkanInstance;
class VulkanQueue;
class VulkanMemoryManager;

class VulkanDevice
{
public:
    VulkanDevice();
    API VulkanDevice(std::shared_ptr<VulkanInstance> inInstance, VkPhysicalDevice inPhysicalDevice, bool InOffscreenRender);
    API void Init();
    API void Destroy();
    API static std::tuple<VkPhysicalDevice, VkPhysicalDeviceProperties> SelectPhysicalDevice(const std::shared_ptr<VulkanInstance>& instance);
    VkDevice GetDeviceHandle() const { return Device; }
    VkPhysicalDevice GetPhysicalDeviceHandle() const { return PhysicalDevice; }
    VulkanQueue* GetGraphicsQueue() { return GraphicsQueue.get(); }
    VulkanQueue* GetPresentQueue() { return GraphicsQueue.get(); }
    VulkanQueue* GetComputeQueue() { return ComputeQueue.get(); }
    VulkanQueue* GetTransferQueue() { return TransferQueue.get(); }

    VulkanMemoryManager* GetMemoryManager() { return MemoryManager.get(); }

private:
    std::shared_ptr<VulkanInstance> Instance;
    VkDevice Device;
    VkPhysicalDevice PhysicalDevice;
    std::shared_ptr<VulkanQueue> GraphicsQueue;
    std::shared_ptr<VulkanQueue> TransferQueue;
    std::shared_ptr<VulkanQueue> ComputeQueue;
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

    std::unique_ptr<VulkanMemoryManager> MemoryManager;
};
