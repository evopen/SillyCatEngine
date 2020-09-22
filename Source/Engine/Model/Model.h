#pragma once

#include "Engine/pch.h"

#include "Engine/Platform/Platform.h"

class VulkanMemoryManager;

class Model
{
public:
    API Model(std::filesystem::path InFilePath);
    std::string GetName() const { return Name; }

    API VkBuffer GetVertexBuffer(VulkanMemoryManager* InMemoryManager);
    API VkBuffer GetColorBuffer(VulkanMemoryManager* InMemoryManager);
    API uint32_t GetVertexCount() { return static_cast<uint32_t>(Vertices.size()); }


    static std::vector<VkVertexInputBindingDescription> GetVertexInputBindingDescriptions();

private:
    std::vector<glm::vec3> Vertices;
    std::vector<glm::vec4> Colors;
    std::string Name;
    bool bHasVertexColors;

    VkBuffer VertexBuffer;
    VkBuffer ColorBuffer;
};
