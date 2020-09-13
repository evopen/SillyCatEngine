#include "Engine/pch.h"

#include "VulkanPipeline.h"

VulkanPipeline::VulkanPipeline(VulkanDevice* InDevice)
    : Pipieline(VK_NULL_HANDLE)
    , Device(InDevice)
{
}
