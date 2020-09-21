#pragma once
#include <map>
#include <stdexcept>
#include <string>

enum class EShaderType
{
    Vertex,
    Pixel,
    Compute
};


EShaderType ToShaderType(std::string FileExtension);

VkShaderStageFlagBits ToVulkanShaderType(EShaderType InShaderType);