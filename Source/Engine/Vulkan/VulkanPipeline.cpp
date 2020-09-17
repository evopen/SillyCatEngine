#include "Engine/pch.h"

#include "VulkanPipeline.h"
#include "VulkanPipelineState.h"
#include "VulkanDevice.h"

VulkanPipeline::VulkanPipeline(VulkanDevice* InDevice)
    : Pipeline(VK_NULL_HANDLE)
    , Device(InDevice)
{
}

VulkanGraphicsPipeline::VulkanGraphicsPipeline(VulkanDevice* InDevice, VulkanGraphicsPipelineState* InVulkanGraphicsPipelineState)
    : VulkanPipeline(InDevice)
    , PipelineState(InVulkanGraphicsPipelineState)
{
    VkPipelineVertexInputStateCreateInfo VertexInputInfo = {
        .sType                           = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
        .vertexBindingDescriptionCount   = 0,
        .pVertexBindingDescriptions      = nullptr,
        .vertexAttributeDescriptionCount = 0,
        .pVertexAttributeDescriptions    = nullptr,
    };

    VkPipelineInputAssemblyStateCreateInfo InputAssemblyInfo = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
        .topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
    };


    VkGraphicsPipelineCreateInfo PipelineInfo = {
        .sType               = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
        .stageCount          = PipelineState->GetStageCount(),
        .pStages             = PipelineState->GetPipelineShaderStageCreateInfos(),
        .pVertexInputState   = &VertexInputInfo,
        .pInputAssemblyState = &InputAssemblyInfo,
        .pTessellationState  = nullptr,
        .pViewportState      = nullptr,
        .pRasterizationState = nullptr,
        .pMultisampleState = nullptr,
        .pDepthStencilState = nullptr,
        .pColorBlendState = nullptr,
        .pDynamicState = nullptr,
        
    };
    vkCreateGraphicsPipelines(Device->GetDeviceHandle(), nullptr, 1, &PipelineInfo, nullptr, &Pipeline);
}
