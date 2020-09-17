#pragma once

#include "Engine/Platform/Platform.h"
#include "VulkanShaderProgram.h"

#include <vulkan/vulkan.h>

class VulkanPipelineState
{
public:
    API VulkanPipelineState(VulkanShaderProgram* InShaderProgram);
    uint32_t GetStageCount() const { return ShaderProgram->GetStageCount(); }
    VkPipelineShaderStageCreateInfo* GetPipelineShaderStageCreateInfos() const { return ShaderProgram->GetPipelineShaderStageCreateInfos(); }

private:
    VulkanShaderProgram* ShaderProgram;
};

class VulkanGraphicsPipelineState : public VulkanPipelineState
{
public:
    API VulkanGraphicsPipelineState(VulkanShaderProgram* InShaderProgram);

private:
};