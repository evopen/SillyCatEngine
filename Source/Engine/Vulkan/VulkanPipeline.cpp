#include "Engine/pch.h"

#include "VulkanDevice.h"
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
        .sType                           = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
        .vertexBindingDescriptionCount   = 0,
        .pVertexBindingDescriptions      = nullptr,
        .vertexAttributeDescriptionCount = 0,
        .pVertexAttributeDescriptions    = nullptr,
    };

    VkPipelineInputAssemblyStateCreateInfo InputAssemblyInfo = {
        .sType    = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
        .topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
    };

    VkPipelineRasterizationStateCreateInfo RasterizationInfo = {
        .sType       = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
        .polygonMode = VK_POLYGON_MODE_FILL,
        .cullMode    = VK_CULL_MODE_BACK_BIT,
        .frontFace   = VK_FRONT_FACE_CLOCKWISE,
        .lineWidth   = 1.f,
    };

    VkViewport Viewport = {
        .x = 0.f,
        .y = 0.f,
    };

    VkPipelineViewportStateCreateInfo ViewportInfo = {
        .sType         = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
        .viewportCount = 1,
        .pViewports    = nullptr,
        .scissorCount  = 1,
        .pScissors     = nullptr,
    };

    VkGraphicsPipelineCreateInfo PipelineInfo = {
        .sType               = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
        .stageCount          = PipelineState->GetStageCount(),
        .pStages             = PipelineState->GetPipelineShaderStageCreateInfos(),
        .pVertexInputState   = &VertexInputInfo,
        .pInputAssemblyState = &InputAssemblyInfo,
        .pTessellationState  = nullptr,
        .pViewportState      = nullptr,
        .pRasterizationState = &RasterizationInfo,
        .pMultisampleState   = nullptr,
        .pDepthStencilState  = nullptr,
        .pColorBlendState    = nullptr,
        .pDynamicState       = nullptr,

    };
    vkCreateGraphicsPipelines(Device->GetDeviceHandle(), nullptr, 1, &PipelineInfo, nullptr, &Pipeline);
}
