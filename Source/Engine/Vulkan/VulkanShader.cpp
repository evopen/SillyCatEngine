#include "Engine/pch.h"

#include "Engine/Filesystem/FileUtil.h"
#include "VulkanDevice.h"
#include "VulkanShader.h"
#include "VulkanShaderCompiler.h"
#include "VulkanUtil.h"

VulkanShader::VulkanShader(VulkanDevice* InDevice, std::filesystem::path InFilePath)
    : Device(InDevice)
    , FilePath(std::move(InFilePath))
    , ShaderModule(VK_NULL_HANDLE)
    , PipelineStageInfo({})
{
    ShaderType = ToShaderType(FilePath.extension().string());
    GLSL       = LoadFile(FilePath, false);

    Spirv = CompileGLSL(ShaderType, GLSL);

    VkShaderModuleCreateInfo ShaderModuleInfo = {};
    ShaderModuleInfo.sType                    = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    ShaderModuleInfo.codeSize                 = Spirv.size() * 4;
    ShaderModuleInfo.pCode                    = Spirv.data();


    vkCreateShaderModule(Device->GetDeviceHandle(), &ShaderModuleInfo, nullptr, &ShaderModule);

    PipelineStageInfo.sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    PipelineStageInfo.stage  = ToVulkanShaderType(ShaderType);
    PipelineStageInfo.module = ShaderModule;
    PipelineStageInfo.pName  = "main";
}

VulkanVertexShader::VulkanVertexShader(VulkanDevice* InDevice, std::filesystem::path InFilePath)
    : VulkanShader(InDevice, InFilePath)
{
    if (ShaderType != EShaderType::Vertex)
    {
        throw std::invalid_argument("Wrong vertex shader source code");
    }
}

VulkanPixelShader::VulkanPixelShader(VulkanDevice* InDevice, std::filesystem::path InFilePath)
    : VulkanShader(InDevice, InFilePath)
{
    if (ShaderType != EShaderType::Pixel)
    {
        throw std::invalid_argument("Wrong pixel shader source code");
    }
}

VulkanComputeShader::VulkanComputeShader(VulkanDevice* InDevice, std::filesystem::path InFilePath)
    : VulkanShader(InDevice, InFilePath)
{
    if (ShaderType != EShaderType::Compute)
    {
        throw std::invalid_argument("Wrong Compute shader source code");
    }
}
