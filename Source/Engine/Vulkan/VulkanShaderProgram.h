#pragma once

#include "Engine/pch.h"

class VulkanVertexShader;
class VulkanPixelShader;
class VulkanComputeShader;

class VulkanShaderProgram
{
public:
    API VulkanShaderProgram() = default;

    

protected:
};

class VulkanGraphicsShaderProgram : public VulkanShaderProgram
{
public:
    API VulkanGraphicsShaderProgram(VulkanVertexShader* InVertexShader, VulkanPixelShader* InPixelShader);

    VkPipelineShaderStageCreateInfo* GetPipelineShaderStageCreateInfos() { return PipelineStageInfos.data(); }
    uint32_t GetStageCount() const { return static_cast<uint32_t>(PipelineStageInfos.size()); }

private:
    VulkanVertexShader* VertexShader;
    VulkanPixelShader* PixelShader;
    std::vector<VkPipelineShaderStageCreateInfo> PipelineStageInfos;
};

class VulkanComputeShaderProgram : public VulkanShaderProgram
{
public:
    API VulkanComputeShaderProgram(VulkanComputeShader* InComputeShader);
    VkPipelineShaderStageCreateInfo GetPipelineShaderStageCreateInfo() { return PipelineStageInfo; }

private:
    VulkanComputeShader* ComputeShader;
    VkPipelineShaderStageCreateInfo PipelineStageInfo;
};
