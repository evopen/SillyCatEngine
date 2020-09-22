#pragma once

#include "Engine/pch.h"

#include "Engine/Platform/Platform.h"


class VulkanDevice;
class VulkanInstance;

class VulkanMemoryManager
{
public:
    API VulkanMemoryManager(VulkanDevice* InDevice, VulkanInstance* InInstance);

    VmaAllocator GetAllocatorHandle() { return Allocator; }

    API VkBuffer CreateBuffer(size_t InSize, VmaMemoryUsage InMemoryUsage, VkBufferUsageFlags InBufferUsage);
    API VkBuffer CreateBuffer(void* InData, size_t InSize, VmaMemoryUsage InMemoryUsage, VkBufferUsageFlags InBufferUsage);
    API void* MapBuffer(VkBuffer InBuffer);
    API void UnMapBuffer(VkBuffer InBuffer);
    API void CopyBuffer(VkBuffer SrcBuffer, VkBuffer DstBuffer);
    API void FreeBuffer(VkBuffer InBuffer);
    API VkDescriptorPool GetDescriptorPoolHandle() { return DescriptorPool; }

private:
    VulkanDevice* Device;
    VulkanInstance* Instance;

    VmaAllocator Allocator;

    struct MemoryInfo
    {
        size_t Size;
        size_t Capacity;
        VmaAllocation Allocation;
    };


    std::unordered_map<VkBuffer, MemoryInfo> Buffers;
    std::unordered_map<VkImage, MemoryInfo> Images;

    VkDescriptorPool DescriptorPool;
};
