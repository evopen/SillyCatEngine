#include "Engine/pch.h"

#include "VulkanCommandBuffer.h"
#include "VulkanDevice.h"
#include "VulkanFence.h"
#include "VulkanInstance.h"
#include "VulkanMemoryManager.h"

VulkanMemoryManager::VulkanMemoryManager(VulkanDevice* InDevice, VulkanInstance* InInstance)
    : Device(InDevice)
    , Instance(InInstance)
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

    Buffers.insert_or_assign(Buffer, MemoryInfo{InSize, AllocationInfo.size, Allocation});

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
