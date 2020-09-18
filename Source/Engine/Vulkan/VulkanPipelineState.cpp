#include "Engine/pch.h"

#include "VulkanPipelineState.h"

VulkanPipelineState::VulkanPipelineState()
{
}

VulkanGraphicsPipelineState::VulkanGraphicsPipelineState(VulkanGraphicsShaderProgram* InShaderProgram)
    : ShaderProgram(InShaderProgram)
{
}

VulkanComputePipelineState::VulkanComputePipelineState(VulkanComputeShaderProgram* InShaderProgram)
    : ShaderProgram(InShaderProgram)
{
}
