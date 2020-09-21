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
    ShaderReflectionInfo VSReflection = VertexShader->GetShaderReflection();
    std::vector<VkVertexInputAttributeDescription> Descriptions(VSReflection.InputInfos.size());

    for (uint32_t i = 0; i < Descriptions.size(); ++i)
    {
        Descriptions[i].location = VSReflection.InputInfos[i].Location;
        Descriptions[i].binding  = i;
        Descriptions[i].format   = GlslBasicTypeToVkFormat(VSReflection.InputInfos[i].Type, VSReflection.InputInfos[i].VectorSize);
        Descriptions[i].offset   = 0;
    }

    return Descriptions;
}
