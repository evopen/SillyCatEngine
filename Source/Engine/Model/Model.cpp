#include "Engine/pch.h"

#include "Engine/Vulkan/VulkanMemoryManager.h"
#include "Model.h"

#include <ranges>

namespace Sce
{

    Model::Model(std::filesystem::path InFilePath, VulkanMemoryManager* inMemoryManager)
        : MemoryManager(inMemoryManager)
        , VertexBuffer(VK_NULL_HANDLE)
        , ColorBuffer(VK_NULL_HANDLE)
    {
        Assimp::Importer Importer;
        const aiScene* Scene = Importer.ReadFile(InFilePath.string().c_str(), aiProcess_Triangulate | aiProcess_OptimizeMeshes);
        if (!Scene)
        {
            throw std::runtime_error(Importer.GetErrorString());
        }
        Meshes.resize(Scene->mNumMeshes);


        for (size_t m = 0; m < Meshes.size(); ++m)
        {
            aiMesh* Mesh   = Scene->mMeshes[m];
            Meshes[m].Name = Mesh->mName.C_Str();

            for (size_t f = 0; f < Mesh->mNumFaces; ++f)
            {
                aiFace* face = &Mesh->mFaces[f];
                for (size_t i = 0; i < 3; i++)
                {
                    Meshes[m].Indices.push_back(face->mIndices[i]);
                }
            }

            unsigned VerticesCount = Mesh->mNumVertices;

            Meshes[m].Vertices.resize(VerticesCount);
            Meshes[m].Colors.resize(VerticesCount);


            for (size_t i = 0; i < VerticesCount; i++)
            {
                Meshes[m].Vertices[i].x = Mesh->mVertices[i].x;
                Meshes[m].Vertices[i].y = Mesh->mVertices[i].y;
                Meshes[m].Vertices[i].z = Mesh->mVertices[i].z;
            }

            bHasVertexColors = Mesh->HasVertexColors(0) ? true : false;
            if (bHasVertexColors)
            {
                for (size_t i = 0; i < VerticesCount; i++)
                {
                    Meshes[m].Colors[i].r = Mesh->mColors[0][i].r;
                    Meshes[m].Colors[i].g = Mesh->mColors[0][i].g;
                    Meshes[m].Colors[i].b = Mesh->mColors[0][i].b;
                    Meshes[m].Colors[i].a = Mesh->mColors[0][i].a;
                }
            }
            else
            {
                for (size_t i = 0; i < VerticesCount; i++)
                {
                    Meshes[m].Colors[i].r = 0.8f;
                    Meshes[m].Colors[i].g = 0.8f;
                    Meshes[m].Colors[i].b = 0.8f;
                    Meshes[m].Colors[i].a = 1.0f;
                }
            }
        }
    }

    Model::~Model()
    {
        if (VertexBuffer != VK_NULL_HANDLE)
            MemoryManager->FreeBuffer(VertexBuffer);
        if (ColorBuffer != VK_NULL_HANDLE)
            MemoryManager->FreeBuffer(ColorBuffer);
    }

    VkBuffer Model::GetVertexBuffer()
    {
        if (VertexBuffer == VK_NULL_HANDLE)
        {
            size_t BufferSize      = sizeof(decltype(Mesh::Vertices)::value_type) * GetVertexCount();
            VkBuffer StagingBuffer = MemoryManager->CreateBuffer(BufferSize, VMA_MEMORY_USAGE_CPU_ONLY, VK_BUFFER_USAGE_TRANSFER_SRC_BIT);
            void* Data             = MemoryManager->MapBuffer(StagingBuffer);
            for (const auto& Mesh : Meshes)
            {
                std::memcpy(Data, Mesh.Vertices.data(), Mesh.Vertices.size() * sizeof(glm::vec3));
                Data = static_cast<char*>(Data) + Mesh.Vertices.size() * sizeof(glm::vec3);
            }
            MemoryManager->UnMapBuffer(StagingBuffer);
            VertexBuffer = MemoryManager->CreateBuffer(BufferSize, VMA_MEMORY_USAGE_GPU_ONLY, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
            MemoryManager->CopyBuffer(StagingBuffer, VertexBuffer);
            MemoryManager->FreeBuffer(StagingBuffer);
        }
        return VertexBuffer;
    }

    VkBuffer Model::GetIndexBuffer()
    {
        if (IndexBuffer == VK_NULL_HANDLE)
        {
            size_t BufferSize      = sizeof(decltype(Mesh::Indices)::value_type) * GetIndexCount();
            VkBuffer StagingBuffer = MemoryManager->CreateBuffer(BufferSize, VMA_MEMORY_USAGE_CPU_ONLY, VK_BUFFER_USAGE_TRANSFER_SRC_BIT);
            void* Data             = MemoryManager->MapBuffer(StagingBuffer);
            for (const auto& Mesh : Meshes)
            {
                std::memcpy(Data, Mesh.Indices.data(), Mesh.Indices.size() * sizeof(decltype(Mesh::Indices)::value_type));
                Data = static_cast<char*>(Data) + Mesh.Indices.size() * sizeof(decltype(Mesh::Indices)::value_type);
            }
            MemoryManager->UnMapBuffer(StagingBuffer);
            IndexBuffer = MemoryManager->CreateBuffer(BufferSize, VMA_MEMORY_USAGE_GPU_ONLY, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT);
            MemoryManager->CopyBuffer(StagingBuffer, IndexBuffer);
            MemoryManager->FreeBuffer(StagingBuffer);
        }
        return IndexBuffer;
    }

    VkBuffer Model::GetColorBuffer()
    {
        if (ColorBuffer == VK_NULL_HANDLE)
        {
            size_t BufferSize      = sizeof(decltype(Mesh::Colors)::value_type) * GetVertexCount();
            VkBuffer StagingBuffer = MemoryManager->CreateBuffer(BufferSize, VMA_MEMORY_USAGE_CPU_ONLY, VK_BUFFER_USAGE_TRANSFER_SRC_BIT);
            void* Data             = MemoryManager->MapBuffer(StagingBuffer);
            for (const auto& Mesh : Meshes)
            {
                std::memcpy(Data, Mesh.Colors.data(), Mesh.Colors.size() * sizeof(decltype(Mesh::Colors)::value_type));
                Data = static_cast<char*>(Data) + Mesh.Vertices.size() * sizeof(decltype(Mesh::Colors)::value_type);
            }
            MemoryManager->UnMapBuffer(StagingBuffer);
            ColorBuffer = MemoryManager->CreateBuffer(BufferSize, VMA_MEMORY_USAGE_GPU_ONLY, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
            MemoryManager->CopyBuffer(StagingBuffer, ColorBuffer);
            MemoryManager->FreeBuffer(StagingBuffer);
        }
        return ColorBuffer;
    }


    std::vector<VkVertexInputBindingDescription> Model::GetVertexInputBindingDescriptions()
    {

        VkVertexInputBindingDescription VertexBindingDesc = {
            .binding   = 0,
            .stride    = sizeof(decltype(Mesh::Vertices)::value_type),
            .inputRate = VK_VERTEX_INPUT_RATE_VERTEX,
        };

        VkVertexInputBindingDescription ColorBindingDesc = {
            .binding   = 1,
            .stride    = sizeof(decltype(Mesh::Colors)::value_type),
            .inputRate = VK_VERTEX_INPUT_RATE_VERTEX,
        };

        std::vector<VkVertexInputBindingDescription> Descriptions(2);
        Descriptions[0] = VertexBindingDesc;
        Descriptions[1] = ColorBindingDesc;

        return Descriptions;
    }
}