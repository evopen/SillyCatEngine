#pragma once

#include "Engine/pch.h"

class VulkanVertexShader;
class VulkanPixelShader;
class VulkanComputeShader;

class VulkanShaderProgram
{
public:
    API VulkanShaderProgram() = default;

    VkPipelineShaderStageCreateInfo* GetPipelineShaderStageCreateInfos() { return PipelineStageInfos.data(); }
    uint32_t GetStageCount() const { return static_cast<uint32_t>(PipelineStageInfos.size()); }

protected:
    std::vector<VkPipelineShaderStageCreateInfo> PipelineStageInfos;
};

class VulkanGraphicsShaderProgram : public VulkanShaderProgram
{
public:
    API VulkanGraphicsShaderProgram(VulkanVertexShader* InVertexShader, VulkanPixelShader* InPixelShader);

private:
    VulkanVertexShader* VertexShader;
    VulkanPixelShader* PixelShader;
};

class VulkanComputeShaderProgram : public VulkanShaderProgram
{
public:
    API VulkanComputeShaderProgram(VulkanComputeShader* InComputeShader);

private:
    VulkanComputeShader* ComputeShader;
};
