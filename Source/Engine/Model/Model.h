#pragma once

#include "Engine/pch.h"

#include "Engine/Platform/Platform.h"

#include <numeric>

class VulkanMemoryManager;

namespace Sce
{
    struct Mesh
    {
        std::string Name;
        std::vector<glm::vec3> Vertices;
        std::vector<glm::vec4> Colors;
    };

    class Model
    {
    public:
        API Model(std::filesystem::path InFilePath, VulkanMemoryManager* inMemoryManager);
        API ~Model();
        std::string GetName() const { return Name; }

        API VkBuffer GetVertexBuffer();
        API VkBuffer GetColorBuffer();
        API uint32_t GetVertexCount()
        {
            size_t sum = 0;
            for (const auto& Mesh : Meshes)
            {
                sum += Mesh.Vertices.size();
            }
            return static_cast<uint32_t>(sum);
        }


        static std::vector<VkVertexInputBindingDescription> GetVertexInputBindingDescriptions();

    private:
        std::vector<Mesh> Meshes;

        std::string Name;
        VulkanMemoryManager* MemoryManager;
        bool bHasVertexColors;

        VkBuffer VertexBuffer;
        VkBuffer ColorBuffer;
    };
}