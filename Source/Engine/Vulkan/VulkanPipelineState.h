#pragma once

#include "Engine/Platform/Platform.h"
#include "VulkanShaderProgram.h"

#include <vulkan/vulkan.h>

class VulkanPipelineState
{
public:
    API VulkanPipelineState();


protected:
};

class VulkanGraphicsPipelineState : public VulkanPipelineState
{
public:
    API VulkanGraphicsPipelineState(VulkanGraphicsShaderProgram* InShaderProgram);
    uint32_t GetStageCount() const { return static_cast<VulkanGraphicsShaderProgram*>(ShaderProgram)->GetStageCount(); }
    VkPipelineShaderStageCreateInfo* GetPipelineShaderStageCreateInfos() const { return ShaderProgram->GetPipelineShaderStageCreateInfos(); }

private:
    VulkanGraphicsShaderProgram* ShaderProgram;
};

class VulkanComputePipelineState : public VulkanPipelineState
{
public:
    API VulkanComputePipelineState(VulkanComputeShaderProgram* InShaderProgram);

    VkPipelineShaderStageCreateInfo GetPipelineShaderStageCreateInfo() const { return ShaderProgram->GetPipelineShaderStageCreateInfo(); }

private:
    VulkanComputeShaderProgram* ShaderProgram;
};