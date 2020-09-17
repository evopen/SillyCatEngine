#include "Engine/pch.h"

#include "VulkanPipelineLayout.h"

VulkanPipelineLayout::VulkanPipelineLayout(VulkanDevice* InDevice)
    : Layout(VK_NULL_HANDLE)
    , Device(InDevice)
{

}

VulkanComputePipelineLayout::VulkanComputePipelineLayout(VulkanDevice* InDevice)
    : VulkanPipelineLayout(InDevice)
{

}

VulkanGraphicsPipelineLayout::VulkanGraphicsPipelineLayout(VulkanDevice* InDevice)
    : VulkanPipelineLayout(InDevice)
{
}
