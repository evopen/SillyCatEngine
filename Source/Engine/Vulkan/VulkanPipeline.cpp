#include "Engine/pch.h"

#include "VulkanPipeline.h"
#include "VulkanPipelineState.h"

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
        .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO
    };
    VkGraphicsPipelineCreateInfo PipelineInfo = {
        .sType      = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
        .stageCount = PipelineState->GetStageCount(),
        .pStages    = PipelineState->GetPipelineShaderStageCreateInfos(),

    };
}
