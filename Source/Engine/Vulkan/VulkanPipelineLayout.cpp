#include "Engine/pch.h"

#include "VulkanDevice.h"
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
    VkPipelineLayoutCreateInfo PipelineLayoutInfo = {
        .sType          = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
        .setLayoutCount = 0,
        .pSetLayouts    = nullptr,
    };
    vkCreatePipelineLayout(Device->GetDeviceHandle(), &PipelineLayoutInfo, nullptr, &Layout);
}
