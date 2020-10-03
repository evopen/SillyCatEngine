#include "Engine/pch.h"

#include "Engine/Vulkan/VulkanMemoryManager.h"
#include "Material.h"
#include "Mesh.h"

namespace Sce
{
    Mesh::Mesh(VulkanMemoryManager* inMemoryManager, std::string inName, std::vector<glm::vec3> inVertices, std::vector<uint32_t> inIndices, std::vector<glm::vec4> inColors, std::optional<std::vector<glm::vec2>> inTextureCoords, std::shared_ptr<Material> inMaterial)
        : MemoryManager(inMemoryManager)
        , Name(std::move(inName))
        , Vertices(std::move(inVertices))
        , Indices(std::move(inIndices))
        , Colors(std::move(inColors))
        , VertexBuffer(VK_NULL_HANDLE)
        , IndexBuffer(VK_NULL_HANDLE)
        , MeshMaterial(inMaterial)
        , TextureCoords(std::move(inTextureCoords))
    {
        CreateVertexBuffer();
        CreateIndexBuffer();
        CreateColorBuffer();
        CreateTextureCoordBuffer();
    }

    Mesh::~Mesh()
    {
        if (VertexBuffer != VK_NULL_HANDLE)
            MemoryManager->FreeBuffer(VertexBuffer);
        if (ColorBuffer.has_value())
            MemoryManager->FreeBuffer(ColorBuffer.value());
    }

    void Mesh::CreateVertexBuffer()
    {
        const size_t BufferSize = sizeof(decltype(Vertices)::value_type) * GetVertexCount();

        VertexBuffer = MemoryManager->CreateBuffer(Vertices.data(), BufferSize, VMA_MEMORY_USAGE_GPU_ONLY, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
    }

    void Mesh::CreateIndexBuffer()
    {
        const size_t BufferSize = sizeof(decltype(Indices)::value_type) * GetIndexCount();

        IndexBuffer = MemoryManager->CreateBuffer(Indices.data(), BufferSize, VMA_MEMORY_USAGE_GPU_ONLY, VK_BUFFER_USAGE_INDEX_BUFFER_BIT);
    }

    void Mesh::CreateColorBuffer()
    {
        const size_t BufferSize = sizeof(decltype(Colors)::value_type) * GetVertexCount();

        ColorBuffer = MemoryManager->CreateBuffer(Colors.data(), BufferSize, VMA_MEMORY_USAGE_GPU_ONLY, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
    }

    void Mesh::CreateTextureCoordBuffer()
    {
        const size_t BufferSize = sizeof(decltype(TextureCoords)::value_type) * GetVertexCount();

        TextureCoordBuffer = MemoryManager->CreateBuffer(TextureCoords->data(), BufferSize, VMA_MEMORY_USAGE_GPU_ONLY, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
    }
}
