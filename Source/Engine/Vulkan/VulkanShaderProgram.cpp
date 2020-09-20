#include "Engine/pch.h"

#include "VulkanShader.h"
#include "VulkanShaderProgram.h"

VulkanGraphicsShaderProgram::VulkanGraphicsShaderProgram(VulkanVertexShader* InVertexShader, VulkanPixelShader* InPixelShader)
    : VertexShader(InVertexShader)
    , PixelShader(InPixelShader)
{
    PipelineStageInfos.push_back(VertexShader->GetPipelineShaderStageInfo());
    PipelineStageInfos.push_back(PixelShader->GetPipelineShaderStageInfo());
}

std::vector<VkVertexInputAttributeDescription> VulkanGraphicsShaderProgram::GetVertexInputAttributeDescriptions()
{
    VkVertexInputAttributeDescription VertexAttributeDesc = {
        .location = 0,
        .binding  = 0,
        .format   = VK_FORMAT_R16G16B16_SFLOAT,
        .offset   = 0,
    };

    VkVertexInputAttributeDescription ColorAttributeDesc = {
        .location = 1,
        .binding  = 1,
        .format   = VK_FORMAT_R16G16B16A16_SFLOAT,
        .offset   = 0,
    };

    return {VertexAttributeDesc, ColorAttributeDesc};
}
