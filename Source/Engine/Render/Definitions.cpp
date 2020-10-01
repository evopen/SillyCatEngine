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

VkFormat GlslBasicTypeToVkFormat(glslang::TBasicType Type, uint32_t VectorSize)
{
    VkFormat Result = VK_FORMAT_UNDEFINED;
    switch (Type)
    {
    case glslang::EbtFloat:
        switch (VectorSize)
        {
        case 2:
            Result = VK_FORMAT_R32G32_SFLOAT;
            break;
        case 3:
            Result = VK_FORMAT_R32G32B32_SFLOAT;
            break;
        case 4:
            Result = VK_FORMAT_R32G32B32A32_SFLOAT;
            break;
        }
        break;
    }

    if (Result == VK_FORMAT_UNDEFINED)
        throw std::runtime_error("unknown type");

    return Result;
}
