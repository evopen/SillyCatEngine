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
