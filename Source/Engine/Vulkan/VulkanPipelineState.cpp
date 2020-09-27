#include "Engine/pch.h"

#include "VulkanPipelineState.h"

VulkanPipelineState::VulkanPipelineState()
{
}

VulkanGraphicsPipelineState::VulkanGraphicsPipelineState(std::shared_ptr<VulkanGraphicsShaderProgram> InShaderProgram)
    : ShaderProgram(InShaderProgram)
    , PolygonMode(VK_POLYGON_MODE_FILL)
{
}

VulkanComputePipelineState::VulkanComputePipelineState(std::shared_ptr<VulkanComputeShaderProgram> InShaderProgram)
    : ShaderProgram(InShaderProgram)
{
}
