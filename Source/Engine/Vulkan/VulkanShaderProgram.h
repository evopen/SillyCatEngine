#pragma once

#include "Engine/pch.h"

class VulkanVertexShader;
class VulkanPixelShader;
class VulkanComputeShader;

class VulkanShaderProgram
{
public:
    VulkanShaderProgram();

    VkPipelineShaderStageCreateInfo* GetPipelineShaderStageCreateInfos() { return PipelineStageInfos.data(); }
    uint32_t GetStageCount() const { return PipelineStageInfos.size(); }

protected:
    std::vector<VkPipelineShaderStageCreateInfo> PipelineStageInfos;
};

class VulkanGraphicsShaderProgram : public VulkanShaderProgram
{
public:
    VulkanGraphicsShaderProgram(VulkanVertexShader* InVertexShader, VulkanPixelShader* InPixelShader);
private:
    VulkanVertexShader* VertexShader;
    VulkanPixelShader* PixelShader;
};

class VulkanComputeShaderProgram : public VulkanShaderProgram
{
public:
    VulkanComputeShaderProgram(VulkanComputeShader* InComputeShader);

private:
    VulkanComputeShader* ComputeShader;
};
