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

private:
    VulkanDevice* Device;
    VkShaderModule ShaderModule;
    std::filesystem::path FilePath;
    EShaderType ShaderType;
    std::vector<char> GLSL;
    std::vector<uint32_t> Spirv;
};
