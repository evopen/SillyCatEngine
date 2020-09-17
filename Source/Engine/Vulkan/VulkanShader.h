#pragma once
#include "Engine/Platform/Platform.h"
#include "Engine/Render/Definitions.h"

#include <vulkan/vulkan.h>

#include <filesystem>
#include <vector>

class VulkanDevice;

class VulkanShader
{
public:
    API VulkanShader(VulkanDevice* InDevice, std::filesystem::path InFilePath);

    VkPipelineShaderStageCreateInfo GetPipelineShaderStageInfo() const { return PipelineStageInfo; }

protected:
    VulkanDevice* Device;
    VkShaderModule ShaderModule;
    std::filesystem::path FilePath;
    EShaderType ShaderType;
    std::vector<char> GLSL;
    std::vector<uint32_t> Spirv;

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