#include "Engine/pch.h"

#include "VulkanCommandBuffer.h"
#include "VulkanDevice.h"
#include "VulkanFence.h"
#include "VulkanInstance.h"
#include "VulkanMemoryManager.h"

VulkanMemoryManager::VulkanMemoryManager(VulkanDevice* InDevice, std::shared_ptr<VulkanInstance> inInstance)
    : Device(InDevice)
    , Instance(inInstance)
    , Allocator(VK_NULL_HANDLE)
{
    VmaAllocatorCreateInfo AllocatorInfo = {
        .physicalDevice = Device->GetPhysicalDeviceHandle(),
        .device         = Device->GetDeviceHandle(),
        .instance       = Instance->GetInstanceHandle(),
    };

    vmaCreateAllocator(&AllocatorInfo, &Allocator);

    std::vector<VkDescriptorPoolSize> PoolSizes = {
        {
            VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
            64,
        },
        {
            VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
            1024,
        },
    };

    VkDescriptorPoolCreateInfo DescriptorPoolInfo = {
        .sType         = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
        .maxSets       = 1024,
        .poolSizeCount = static_cast<uint32_t>(PoolSizes.size()),
        .pPoolSizes    = PoolSizes.data(),
    };
    vkCreateDescriptorPool(Device->GetDeviceHandle(), &DescriptorPoolInfo, nullptr, &DescriptorPool);
}

VkBuffer VulkanMemoryManager::CreateBuffer(size_t InSize, VmaMemoryUsage InMemoryUsage, VkBufferUsageFlags InBufferUsage)
{
    VkBufferCreateInfo StagingBufferInfo = {
        .sType       = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
        .size        = InSize,
        .usage       = InBufferUsage,
        .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
    };

    VmaAllocationCreateInfo AllocationCreateInfo = {
        .usage = InMemoryUsage,
    };

    VkBuffer Buffer;
    VmaAllocation Allocation;

    VmaAllocationInfo AllocationInfo;
    vmaCreateBuffer(Allocator, &StagingBufferInfo, &AllocationCreateInfo, &Buffer, &Allocation, &AllocationInfo);

    Buffers.insert_or_assign(Buffer, BufferMemoryInfo{InSize, AllocationInfo.size, Allocation});

    return Buffer;
}

VkBuffer VulkanMemoryManager::CreateBuffer(void* InData, size_t InSize, VmaMemoryUsage InMemoryUsage, VkBufferUsageFlags InBufferUsage)
{
    VkBuffer Buffer, StagingBuffer;
    void* Data;
    switch (InMemoryUsage)
    {
    case VMA_MEMORY_USAGE_CPU_ONLY:
    case VMA_MEMORY_USAGE_CPU_TO_GPU:
        Buffer = CreateBuffer(InSize, InMemoryUsage, InBufferUsage);
        Data   = MapBuffer(Buffer);
        memcpy(Data, InData, InSize);
        UnMapBuffer(Buffer);
        break;
    case VMA_MEMORY_USAGE_GPU_ONLY:
        StagingBuffer = CreateBuffer(InSize, VMA_MEMORY_USAGE_CPU_ONLY, VK_BUFFER_USAGE_TRANSFER_SRC_BIT);
        Data          = MapBuffer(StagingBuffer);
        std::memcpy(Data, InData, InSize);
        UnMapBuffer(StagingBuffer);
        Buffer = CreateBuffer(InSize, VMA_MEMORY_USAGE_GPU_ONLY, VK_BUFFER_USAGE_TRANSFER_DST_BIT | InBufferUsage);
        CopyBuffer(StagingBuffer, Buffer);
        FreeBuffer(StagingBuffer);
        break;
    default:
        throw std::runtime_error("unknown usage");
    }
    return Buffer;
}

VkImage VulkanMemoryManager::CreateImage(VkExtent3D inExtent, VkFormat inFormat, VmaMemoryUsage inMemoryUsage, VkImageUsageFlags inImageUsage)
{
    VkImageCreateInfo ImageInfo = {
        .sType         = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
        .imageType     = VK_IMAGE_TYPE_2D,
        .format        = inFormat,
        .extent        = inExtent,
        .mipLevels     = 1,
        .arrayLayers   = 1,
        .samples       = VK_SAMPLE_COUNT_1_BIT,
        .tiling        = VK_IMAGE_TILING_OPTIMAL,
        .usage         = inImageUsage,
        .sharingMode   = VK_SHARING_MODE_EXCLUSIVE,
        .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
    };

    VkImage Image;
    VmaAllocation Allocation;

    VmaAllocationCreateInfo AllocationCreateInfo = {
        .usage = inMemoryUsage,
    };
    VmaAllocationInfo AllocationInfo;

    vmaCreateImage(Allocator, &ImageInfo, &AllocationCreateInfo, &Image, &Allocation, &AllocationInfo);

    Images.insert_or_assign(Image, ImageMemoryInfo{AllocationInfo.size, Allocation, inExtent});
    return Image;
}

// always use staging buffer, because i use tilling_optimal layout
VkImage VulkanMemoryManager::CreateImage(const std::vector<uint8_t>& ImageData, VkExtent3D inExtent, VkFormat inFormat, VmaMemoryUsage inMemoryUsage, VkImageUsageFlags inImageUsage, VkImageLayout inImageLayout)
{
    VkImage Image;
    VkBuffer StagingBuffer;
    void* Data;
    switch (inMemoryUsage)
    {
    case VMA_MEMORY_USAGE_CPU_ONLY:
    case VMA_MEMORY_USAGE_CPU_TO_GPU:
    case VMA_MEMORY_USAGE_GPU_ONLY:
        Image         = CreateImage(inExtent, inFormat, inMemoryUsage, VK_BUFFER_USAGE_TRANSFER_DST_BIT | inImageUsage);
        StagingBuffer = CreateBuffer(ImageData.size(), VMA_MEMORY_USAGE_CPU_ONLY, VK_BUFFER_USAGE_TRANSFER_SRC_BIT);
        Data          = MapBuffer(StagingBuffer);
        std::memcpy(Data, ImageData.data(), ImageData.size());
        UnMapBuffer(StagingBuffer);

        TransitImageLayout(Image, 0, 0, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
        CopyBufferToImage(StagingBuffer, Image);
        TransitImageLayout(Image, 0, 0, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, inImageLayout);
        FreeBuffer(StagingBuffer);
        break;
    default:
        throw std::runtime_error("unknown usage");
    }
    return Image;
}

void VulkanMemoryManager::CopyBufferToImage(VkBuffer inSrcBuffer, VkImage inDstImage)
{
    VkBufferImageCopy Region = {
        .bufferOffset      = 0,
        .bufferRowLength   = 0,
        .bufferImageHeight = 0,
        .imageSubresource  = {
            .aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT,
            .mipLevel       = 0,
            .baseArrayLayer = 0,
            .layerCount     = 1,
        },
        .imageOffset = {0, 0, 0},
        .imageExtent = Images.at(inDstImage).Extent,
    };

    VulkanCommandBuffer cmdBuffer(Device, Device->GetTransferQueue());
    cmdBuffer.Begin();
    vkCmdCopyBufferToImage(cmdBuffer.GetHandle(), inSrcBuffer, inDstImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &Region);
    cmdBuffer.End();
    cmdBuffer.Submit();
    cmdBuffer.Wait();
}

void* VulkanMemoryManager::MapBuffer(VkBuffer InBuffer)
{
    void* Data;
    vmaMapMemory(Allocator, Buffers.at(InBuffer).Allocation, &Data);
    return Data;
}

void VulkanMemoryManager::UnMapBuffer(VkBuffer InBuffer)
{
    vmaUnmapMemory(Allocator, Buffers.at(InBuffer).Allocation);
}

void VulkanMemoryManager::CopyBuffer(VkBuffer SrcBuffer, VkBuffer DstBuffer)
{
    if (Buffers.at(SrcBuffer).Size != Buffers.at(DstBuffer).Size)
    {
        throw std::runtime_error("unsupported");
    }

    VkBufferCopy CopyRegion{
        .srcOffset = 0,
        .dstOffset = 0,
        .size      = Buffers.at(SrcBuffer).Size};

    VulkanCommandBuffer CmdBuffer(Device, Device->GetTransferQueue());
    CmdBuffer.Begin();
    vkCmdCopyBuffer(CmdBuffer.GetHandle(), SrcBuffer, DstBuffer, 1, &CopyRegion);
    CmdBuffer.End();
    CmdBuffer.Submit();
    CmdBuffer.Wait();
}

void VulkanMemoryManager::FreeBuffer(VkBuffer InBuffer)
{
    vmaDestroyBuffer(Allocator, InBuffer, Buffers.at(InBuffer).Allocation);
    Buffers.erase(InBuffer);
}

void VulkanMemoryManager::TransitImageLayout(VkImage inImage, VkAccessFlags inSrcAccessMask, VkAccessFlags inDstAccessMask, VkPipelineStageFlags inSrcStage, VkPipelineStageFlags inDstStage, VkImageLayout inOldLayout, VkImageLayout inNewLayout)
{
    VkImageMemoryBarrier memoryBarrier = {
        .sType            = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
        .srcAccessMask    = inSrcAccessMask,
        .dstAccessMask    = inDstAccessMask,
        .oldLayout        = inOldLayout,
        .newLayout        = inNewLayout,
        .image            = inImage,
        .subresourceRange = {
            .aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT,
            .baseMipLevel   = 0,
            .levelCount     = 1,
            .baseArrayLayer = 0,
            .layerCount     = 1,
        },
    };
    VulkanCommandBuffer cmdBuffer(Device, Device->GetTransferQueue());
    cmdBuffer.Begin();
    vkCmdPipelineBarrier(cmdBuffer.GetHandle(), inSrcStage, inDstStage, 0,
        0, nullptr,
        0, nullptr,
        1, &memoryBarrier);
    cmdBuffer.End();
    cmdBuffer.Submit();
    cmdBuffer.Wait();
}
