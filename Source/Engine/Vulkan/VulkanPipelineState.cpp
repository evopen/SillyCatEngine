#include "Engine/pch.h"

#include "VulkanPipelineState.h"

VulkanPipelineState::VulkanPipelineState(VulkanShaderProgram* InShaderProgram)
    : ShaderProgram(InShaderProgram)
{
}

VulkanGraphicsPipelineState::VulkanGraphicsPipelineState(VulkanShaderProgram* InShaderProgram)
    : VulkanPipelineState(InShaderProgram)
{
}
