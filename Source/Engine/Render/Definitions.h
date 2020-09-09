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


static EShaderType ToShaderType(std::string FileExtension)
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