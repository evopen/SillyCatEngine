#include "Engine/pch.h"

#include "Definitions.h"


EShaderType ToShaderType(std::string FileExtension)
{
    std::map<std::string, EShaderType> Map = {
        {".vert", EShaderType::Vertex},
        {".frag", EShaderType::Pixel},
        {".comp", EShaderType::Compute},
    };
    if (!Map.contains(FileExtension))
        throw std::invalid_argument("Invalid file type");
    return Map.at(FileExtension);
}

VkShaderStageFlagBits ToVulkanShaderType(EShaderType InShaderType)
{
    std::map<EShaderType, VkShaderStageFlagBits> Map = {
        {EShaderType::Vertex, VK_SHADER_STAGE_VERTEX_BIT},
        {EShaderType::Pixel, VK_SHADER_STAGE_FRAGMENT_BIT},
        {EShaderType::Compute, VK_SHADER_STAGE_COMPUTE_BIT},
    };

    return Map.at(InShaderType);
}