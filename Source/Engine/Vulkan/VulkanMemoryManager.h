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
    API VkImage CreateImage(VkExtent3D inExtent, VkFormat inFormat, VmaMemoryUsage inMemoryUsage, VkImageUsageFlags inImageUsage);
    API VkImage CreateImage(const std::vector<uint8_t>& ImageData, VkExtent3D inExtent, VkFormat inFormat, VmaMemoryUsage inMemoryUsage, VkImageUsageFlags inImageUsage, VkImageLayout inImageLayout);
    API void CopyBufferToImage(VkBuffer inSrcBuffer, VkImage inDstImage);
    API void* MapBuffer(VkBuffer InBuffer);
    API void UnMapBuffer(VkBuffer InBuffer);
    API void CopyBuffer(VkBuffer SrcBuffer, VkBuffer DstBuffer);
    API void FreeBuffer(VkBuffer InBuffer);
    API VkDescriptorPool GetDescriptorPoolHandle() { return DescriptorPool; }
    API void TransitImageLayout(VkImage inImage, VkAccessFlags inSrcAccessMask, VkAccessFlags inDstAccessMask, VkPipelineStageFlags inSrcStage, VkPipelineStageFlags inDstStage, VkImageLayout inOldLayout, VkImageLayout inNewLayout);

private:
    VulkanDevice* Device;
    VulkanInstance* Instance;

    VmaAllocator Allocator;

    struct BufferMemoryInfo
    {
        size_t Size;
        size_t Capacity;
        VmaAllocation Allocation;
    };

    struct ImageMemoryInfo
    {
        size_t Capacity;
        VmaAllocation Allocation;
        VkExtent3D Extent;
    };


    std::unordered_map<VkBuffer, BufferMemoryInfo> Buffers;
    std::unordered_map<VkImage, ImageMemoryInfo> Images;

    VkDescriptorPool DescriptorPool;
};
