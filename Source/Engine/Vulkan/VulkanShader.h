#pragma once
#include "Engine/pch.h"

#include "Engine/Platform/Platform.h"
#include "Engine/Render/Definitions.h"

class VulkanDevice;

class VulkanShader
{
public:
    API VulkanShader(VulkanDevice* InDevice, std::filesystem::path InFilePath);

    VkPipelineShaderStageCreateInfo GetPipelineShaderStageInfo() const { return PipelineStageInfo; }
    ShaderReflectionInfo GetShaderReflection() const { return ReflectionInfo; }

protected:
    VulkanDevice* Device;
    VkShaderModule ShaderModule;
    std::filesystem::path FilePath;
    EShaderType ShaderType;
    std::vector<char> GLSL;
    std::vector<uint32_t> Spirv;
    ShaderReflectionInfo ReflectionInfo;

    VkPipelineShaderStageCreateInfo PipelineStageInfo;
};

class VulkanVertexShader : public VulkanShader
{
public:
    API VulkanVertexShader(VulkanDevice* InDevice, std::filesystem::path InFilePath);
};

class VulkanPixelShader : public VulkanShader
{
public:
    API VulkanPixelShader(VulkanDevice* InDevice, std::filesystem::path InFilePath);
};

class VulkanComputeShader : public VulkanShader
{
public:
    API VulkanComputeShader(VulkanDevice* InDevice, std::filesystem::path InFilePath);
};