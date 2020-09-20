#pragma once

#include "Engine/pch.h"

#include "Engine/Platform/Platform.h"

class Model
{
public:
    API Model(std::filesystem::path InFilePath);
    std::string GetName() const { return Name; }

    static std::vector<VkVertexInputBindingDescription> GetVertexInputBindingDescriptions();

private:
    std::vector<glm::vec3> Vertices;
    std::vector<glm::vec4> Colors;
    std::string Name;
    bool bHasVertexColors;
};
