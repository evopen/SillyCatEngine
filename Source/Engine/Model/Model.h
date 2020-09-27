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
        std::vector<uint32_t> Indices;
        std::vector<glm::vec4> Colors;
    };

    class Model
    {
    public:
        API Model(std::filesystem::path InFilePath, VulkanMemoryManager* inMemoryManager);
        API ~Model();
        std::string GetName() const { return Name; }

        API VkBuffer GetVertexBuffer();
        API VkBuffer GetIndexBuffer();
        API VkBuffer GetColorBuffer();
        API uint32_t GetVertexCount()
        {
            return std::accumulate(Meshes.begin(), Meshes.end(), 0, [](uint32_t sum, const Mesh& mesh) { return sum + static_cast<uint32_t>(mesh.Vertices.size()); });
        }
        API uint32_t GetIndexCount()
        {
            return std::accumulate(Meshes.begin(), Meshes.end(), 0, [](uint32_t sum, const Mesh& mesh) { return sum + static_cast<uint32_t>(mesh.Indices.size()); });
        }
        API uint32_t GetMeshCount() { return static_cast<uint32_t>(Meshes.size()); }
        API Mesh& GetMesh(size_t index) { return Meshes[index]; }


        static std::vector<VkVertexInputBindingDescription> GetVertexInputBindingDescriptions();

    private:
        std::vector<Mesh> Meshes;

        std::string Name;
        VulkanMemoryManager* MemoryManager;
        bool bHasVertexColors;

        VkBuffer VertexBuffer;
        VkBuffer ColorBuffer;
        VkBuffer IndexBuffer;
    };
}