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

    VkBuffer CreateBuffer(size_t InSize, VmaMemoryUsage InMemoryUsage, VkBufferUsageFlags InBufferUsage);
    void* MapBuffer(VkBuffer InBuffer);
    void UnMapBuffer(VkBuffer InBuffer);
    void CopyBuffer(VkBuffer SrcBuffer, VkBuffer DstBuffer);
    void FreeBuffer(VkBuffer InBuffer);

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
};
