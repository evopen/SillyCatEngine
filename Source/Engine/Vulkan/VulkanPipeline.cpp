#include "Engine/pch.h"

#include "VulkanCommandBuffer.h"
#include "VulkanDevice.h"
#include "VulkanPipeline.h"
#include "VulkanPipelineLayout.h"
#include "VulkanPipelineState.h"
#include "VulkanRenderPass.h"
#include "VulkanRenderTargetLayout.h"

VulkanPipeline::VulkanPipeline(VulkanDevice* InDevice)
    : Pipeline(VK_NULL_HANDLE)
    , Device(InDevice)
{
}

VulkanPipeline::~VulkanPipeline()
{
    if (Pipeline != VK_NULL_HANDLE)
        vkDestroyPipeline(Device->GetDeviceHandle(), Pipeline, nullptr);
}


VulkanGraphicsPipeline::VulkanGraphicsPipeline(VulkanDevice* InDevice, std::shared_ptr<VulkanRenderPass> InRenderPass, std::shared_ptr<VulkanGraphicsPipelineState> InVulkanGraphicsPipelineState)
    : VulkanPipeline(InDevice)
    , PipelineState(InVulkanGraphicsPipelineState)
    , RenderPass(InRenderPass)
    , VertexInputBindingDescriptions(VulkanGraphicsPipelineState::GetVertexInputBindingDescriptions())
    , VertexInputAttributeDescriptions(PipelineState->GetVertexInputAttributeDescriptions())
{
    VkPipelineVertexInputStateCreateInfo VertexInputInfo = {
        .sType                           = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
        .vertexBindingDescriptionCount   = static_cast<uint32_t>(VertexInputBindingDescriptions.size()),
        .pVertexBindingDescriptions      = VertexInputBindingDescriptions.data(),
        .vertexAttributeDescriptionCount = static_cast<uint32_t>(VertexInputAttributeDescriptions.size()),
        .pVertexAttributeDescriptions    = VertexInputAttributeDescriptions.data(),
    };

    VkPipelineInputAssemblyStateCreateInfo InputAssemblyInfo = {
        .sType    = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
        .topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
    };

    VkPipelineRasterizationStateCreateInfo RasterizationInfo = {
        .sType       = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
        .polygonMode = PipelineState->GetPolygonMode(),
        .cullMode    = VK_CULL_MODE_NONE,
        .frontFace   = VK_FRONT_FACE_CLOCKWISE,
        .lineWidth   = 1.f,
    };

    VkPipelineViewportStateCreateInfo ViewportInfo = {
        .sType         = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
        .viewportCount = 1,
        .scissorCount  = 1,
    };

    std::vector<VkDynamicState> DynamicStates = {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR,
    };

    VkPipelineDynamicStateCreateInfo DynamicStateInfo = {
        .sType             = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
        .dynamicStateCount = static_cast<uint32_t>(DynamicStates.size()),
        .pDynamicStates    = DynamicStates.data(),
    };

    VkPipelineMultisampleStateCreateInfo MultisampleInfo = {
        .sType                = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
        .rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
    };

    std::vector<VkPipelineColorBlendAttachmentState> ColorBlendStates(RenderPass->GetRenderTargetLayout()->GetColorAttachmentReferenceCount());
    for (auto& State : ColorBlendStates)
    {
        State.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
        State.blendEnable    = VK_FALSE;
    }

    VkPipelineColorBlendStateCreateInfo ColorBlendInfo = {
        .sType           = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
        .logicOpEnable   = VK_FALSE,
        .attachmentCount = RenderPass->GetRenderTargetLayout()->GetColorAttachmentReferenceCount(),
        .pAttachments    = ColorBlendStates.data(),
    };

    VkGraphicsPipelineCreateInfo PipelineInfo = {
        .sType               = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
        .stageCount          = PipelineState->GetStageCount(),
        .pStages             = PipelineState->GetPipelineShaderStageCreateInfos(),
        .pVertexInputState   = &VertexInputInfo,
        .pInputAssemblyState = &InputAssemblyInfo,
        .pTessellationState  = nullptr,
        .pViewportState      = &ViewportInfo,
        .pRasterizationState = &RasterizationInfo,
        .pMultisampleState   = &MultisampleInfo,
        .pDepthStencilState  = nullptr,
        .pColorBlendState    = &ColorBlendInfo,
        .pDynamicState       = &DynamicStateInfo,
        .layout              = PipelineState->GetPipelineLayoutHandle(),
        .renderPass          = RenderPass->GetRenderPassHandle(),
    };

    vkCreateGraphicsPipelines(Device->GetDeviceHandle(), nullptr, 1, &PipelineInfo, nullptr, &Pipeline);
}

void VulkanGraphicsPipeline::Bind(VulkanCommandBuffer* inCmdBuffer)
{
    vkCmdBindPipeline(inCmdBuffer->GetHandle(), VK_PIPELINE_BIND_POINT_GRAPHICS, Pipeline);
    inCmdBuffer->PossessObject(shared_from_this());
}

VulkanComputePipeline::VulkanComputePipeline(VulkanDevice* InDevice, VulkanPipelineLayout* InLayout, std::shared_ptr<VulkanComputePipelineState> InComputePipelineState)
    : VulkanPipeline(InDevice)
    , PipelineState(InComputePipelineState)
{
    VkComputePipelineCreateInfo PipelineInfo = {
        .sType  = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO,
        .stage  = PipelineState->GetPipelineShaderStageCreateInfo(),
        .layout = PipelineState->GetPipelineLayoutHandle(),
    };
    vkCreateComputePipelines(Device->GetDeviceHandle(), nullptr, 1, &PipelineInfo, nullptr, &Pipeline);
}
