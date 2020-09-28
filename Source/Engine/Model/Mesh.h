#pragma once
#include "Engine/Platform/Platform.h"

class VulkanMemoryManager;

namespace Sce
{
    class Mesh
    {
    public:
        explicit API Mesh(VulkanMemoryManager* inMemoryManager, std::string inName, std::vector<glm::vec3> inVertices, std::vector<uint32_t> inIndices, std::vector<glm::vec4> inColors);
        API ~Mesh();

        API uint32_t GetVertexCount() const { return static_cast<uint32_t>(Vertices.size()); }
        API uint32_t GetIndexCount() const { return static_cast<uint32_t>(Indices.size()); }

        VkBuffer GetVertexBuffer() const { return VertexBuffer; }
        VkBuffer GetIndexBuffer() const { return IndexBuffer; }
        VkBuffer GetColorBuffer() const { return ColorBuffer.has_value() ? ColorBuffer.value() : throw std::runtime_error("no color buffer"); }

        static constexpr uint32_t GetVertexStride() { return sizeof(decltype(Vertices)::value_type); }
        static constexpr uint32_t GetColorStride() { return sizeof(decltype(Colors)::value_type); }
        static constexpr uint32_t GetIndexStride() { return sizeof(decltype(Indices)::value_type); }


    private:
        VulkanMemoryManager* MemoryManager;

        std::string Name;
        std::vector<glm::vec3> Vertices;
        std::vector<uint32_t> Indices;
        std::vector<glm::vec4> Colors;

        VkBuffer VertexBuffer;
        VkBuffer IndexBuffer;
        std::optional<VkBuffer> ColorBuffer;

        void CreateVertexBuffer();
        void CreateIndexBuffer();
        void CreateColorBuffer();
    };
}
