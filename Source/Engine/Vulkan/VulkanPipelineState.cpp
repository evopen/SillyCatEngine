#include "Engine/pch.h"

#include "VulkanPipelineState.h"

VulkanPipelineState::VulkanPipelineState()
{
}

VulkanGraphicsPipelineState::VulkanGraphicsPipelineState(std::shared_ptr<VulkanGraphicsShaderProgram>  InShaderProgram)
    : ShaderProgram(InShaderProgram)
{
}

VulkanComputePipelineState::VulkanComputePipelineState(std::shared_ptr<VulkanComputeShaderProgram> InShaderProgram)
    : ShaderProgram(InShaderProgram)
{
}
