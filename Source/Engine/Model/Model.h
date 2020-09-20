#pragma once

#include "Engine/pch.h"

#include "Engine/Platform/Platform.h"

class Model
{
public:
    API Model(std::filesystem::path InFilePath);
    std::string GetName() const { return Name; }

private:
    std::vector<glm::vec3> Vertices;
    std::string Name;
};
