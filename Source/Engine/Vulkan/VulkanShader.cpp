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

    std::tie(Spirv, ReflectionInfo) = std::move(CompileGLSL(ShaderType, GLSL));

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

VulkanShader::~VulkanShader()
{
    if (ShaderModule != VK_NULL_HANDLE)
    {
        vkDestroyShaderModule(Device->GetDeviceHandle(), ShaderModule, nullptr);
    }
}

std::vector<VkDescriptorSetLayoutBinding> VulkanShader::GetDescriptorSetLayoutBindings()
{
    std::vector<VkDescriptorSetLayoutBinding> Bindings(ReflectionInfo.UniformBlockInfos.size());
    for (size_t i = 0; i < ReflectionInfo.UniformBlockInfos.size(); i++)
    {
        Bindings[i].binding         = ReflectionInfo.UniformBlockInfos[i].Binding;
        Bindings[i].descriptorCount = 1;
        Bindings[i].descriptorType  = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        Bindings[i].stageFlags      = ToVulkanShaderType(ShaderType);
    }

    for (size_t i = 0; i < ReflectionInfo.Sampler2DInfos.size(); i++)
    {
        Bindings.emplace_back(VkDescriptorSetLayoutBinding{
            .binding         = ReflectionInfo.Sampler2DInfos[i].Binding,
            .descriptorType  = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
            .descriptorCount = 1,
            .stageFlags      = static_cast<VkShaderStageFlags>(ToVulkanShaderType(ShaderType)),
        });
    }

    return Bindings;
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
