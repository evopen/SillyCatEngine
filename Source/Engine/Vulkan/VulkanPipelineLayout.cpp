#include "Engine/pch.h"

#include "VulkanDevice.h"
#include "VulkanPipelineLayout.h"
#include "VulkanShaderProgram.h"

VulkanPipelineLayout::VulkanPipelineLayout(VulkanDevice* InDevice)
    : Layout(VK_NULL_HANDLE)
    , Device(InDevice)
{
}

VulkanComputePipelineLayout::VulkanComputePipelineLayout(VulkanDevice* InDevice, VulkanShaderProgram* InShaderProgram)
    : VulkanPipelineLayout(InDevice)
{
}

VulkanGraphicsPipelineLayout::VulkanGraphicsPipelineLayout(VulkanDevice* InDevice, VulkanGraphicsShaderProgram* InShaderProgram)
    : VulkanPipelineLayout(InDevice)
    , ShaderProgram(InShaderProgram)
{
    auto DescriptorSetLayoutInfo = ShaderProgram->GetDescriptorSetLayoutCreateInfo();

    vkCreateDescriptorSetLayout(Device->GetDeviceHandle(), &DescriptorSetLayoutInfo, nullptr, &DescriptorSetLayout);

    VkPipelineLayoutCreateInfo PipelineLayoutInfo = {
        .sType          = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
        .setLayoutCount = 1,
        .pSetLayouts    = &DescriptorSetLayout,
    };
    vkCreatePipelineLayout(Device->GetDeviceHandle(), &PipelineLayoutInfo, nullptr, &Layout);
}
