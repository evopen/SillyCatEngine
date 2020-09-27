#include "Engine/pch.h"

#include "VulkanDevice.h"
#include "VulkanInstance.h"
#include "VulkanMemoryManager.h"
#include "VulkanQueue.h"
#include "VulkanUtil.h"


VulkanDevice::VulkanDevice()
    : Instance(nullptr)
    , Device(VK_NULL_HANDLE)
    , PhysicalDevice(VK_NULL_HANDLE)
    , OffscreenRender(false)
    , PhysicalDeviceProp()
{
}

VulkanDevice::VulkanDevice(std::shared_ptr<VulkanInstance> inInstance, VkPhysicalDevice inPhysicalDevice, bool InOffscreenRender)
    : Instance(inInstance)
    , Device(VK_NULL_HANDLE)
    , PhysicalDevice(inPhysicalDevice)
    , OffscreenRender(InOffscreenRender)
    , PhysicalDeviceProp()
{
    Init();
}


void VulkanDevice::CreateDevice()
{
    uint32_t GraphicsFamilyIndex = -1;
    uint32_t ComputeFamilyIndex  = -1;
    uint32_t TransferFamilyIndex = -1;

    for (int FamilyIndex = 0; FamilyIndex < QueueFamilyProps.size(); FamilyIndex++)
    {
        const auto& CurrFamilyProp = QueueFamilyProps[FamilyIndex];

        if (GraphicsFamilyIndex == -1 && (CurrFamilyProp.queueFlags & VK_QUEUE_GRAPHICS_BIT) != 0)
        {
            GraphicsFamilyIndex = FamilyIndex;
        }
        if (ComputeFamilyIndex == -1 && (CurrFamilyProp.queueFlags & VK_QUEUE_COMPUTE_BIT) != 0)
        {
            ComputeFamilyIndex = FamilyIndex;
        }
        if (TransferFamilyIndex == -1 && (CurrFamilyProp.queueFlags & VK_QUEUE_TRANSFER_BIT) != 0)
        {
            TransferFamilyIndex = FamilyIndex;
        }
    }

    std::set<uint32_t> UniqueFamilyIndex;
    UniqueFamilyIndex.insert(GraphicsFamilyIndex);
    UniqueFamilyIndex.insert(TransferFamilyIndex);
    UniqueFamilyIndex.insert(ComputeFamilyIndex);

    std::vector<VkDeviceQueueCreateInfo> QueueInfos(UniqueFamilyIndex.size());

    int i               = 0;
    float QueuePriority = 1.f;
    for (const auto& FamilyIndex : UniqueFamilyIndex)
    {
        QueueInfos[i]                  = {};
        QueueInfos[i].sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        QueueInfos[i].queueFamilyIndex = FamilyIndex;
        QueueInfos[i].queueCount       = 1;
        QueueInfos[i].pQueuePriorities = &QueuePriority;
        ++i;
    }

    VkPhysicalDeviceFeatures deviceFeatures = {
        .fillModeNonSolid = true,
    };

    VkDeviceCreateInfo deviceInfo = {
        .sType                   = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
        .queueCreateInfoCount    = static_cast<uint32_t>(QueueInfos.size()),
        .pQueueCreateInfos       = QueueInfos.data(),
        .enabledLayerCount       = static_cast<uint32_t>(DeviceLayers.size()),
        .ppEnabledLayerNames     = DeviceLayers.data(),
        .enabledExtensionCount   = static_cast<uint32_t>(DeviceExtensions.size()),
        .ppEnabledExtensionNames = DeviceExtensions.data(),
        .pEnabledFeatures        = &deviceFeatures,
    };

    CheckResult(vkCreateDevice(PhysicalDevice, &deviceInfo, nullptr, &Device));

    GraphicsQueue = std::make_shared<VulkanQueue>(this, GraphicsFamilyIndex);
    TransferQueue = std::make_shared<VulkanQueue>(this, TransferFamilyIndex);
    ComputeQueue  = std::make_shared<VulkanQueue>(this, ComputeFamilyIndex);

    MemoryManager.reset(new VulkanMemoryManager(this, Instance));
}

void VulkanDevice::Init()
{
    GetDeviceExtensions(DeviceExtensions, OffscreenRender);
    vkGetPhysicalDeviceProperties(PhysicalDevice, &PhysicalDeviceProp);

    GetQueueProperties(PhysicalDevice, QueueFamilyProps);

    CreateDevice();
}

void VulkanDevice::Destroy()
{
    vkDestroyDevice(Device, nullptr);
}

void VulkanDevice::GetDeviceExtensions(std::vector<const char*>& deviceExtensions, bool OffscreenRender)
{
    if (!OffscreenRender)
        deviceExtensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
}

void VulkanDevice::GetQueueProperties(
    VkPhysicalDevice physicalDevice, std::vector<VkQueueFamilyProperties>& queueProperties)
{
    uint32_t QueueFamilyCount;
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &QueueFamilyCount, nullptr);
    queueProperties.resize(QueueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &QueueFamilyCount, queueProperties.data());
}


std::tuple<VkPhysicalDevice, VkPhysicalDeviceProperties> VulkanDevice::SelectPhysicalDevice(const std::shared_ptr<VulkanInstance>& instance)
{
    uint32_t numPhysicalDevices;
    CheckResult(vkEnumeratePhysicalDevices(instance->GetInstanceHandle(), &numPhysicalDevices, nullptr));
    std::vector<VkPhysicalDevice> physicalDevices(numPhysicalDevices);
    vkEnumeratePhysicalDevices(instance->GetInstanceHandle(), &numPhysicalDevices, physicalDevices.data());

    for (size_t i = 0; i < numPhysicalDevices; i++)
    {
        VkPhysicalDeviceProperties prop;
        vkGetPhysicalDeviceProperties(physicalDevices[i], &prop);
        if (prop.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
        {
            return std::make_tuple(physicalDevices[i], prop);
        }
    }

    throw std::runtime_error("Failed to find suitable device");
}
