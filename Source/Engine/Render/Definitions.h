#pragma once


enum class EShaderType
{
    Vertex,
    Pixel,
    Compute
};

enum class EGlslDataType
{
    Float,
    Float16,
    Double,
    Int8,
    Uint8,
    Int,
    Uint,
    Bool,
};

struct ShaderReflectionInfo
{
    struct InputInfo
    {
        std::string Name;
        uint32_t Location;
        glslang::TBasicType Type;
        uint32_t VectorSize;
    };
    std::vector<InputInfo> InputInfos;
};


EShaderType ToShaderType(std::string FileExtension);

VkShaderStageFlagBits ToVulkanShaderType(EShaderType InShaderType);

VkFormat GlslBasicTypeToVkFormat(glslang::TBasicType Type, uint32_t VectorSize);
