#include "Engine/pch.h"

#include "Engine/Vulkan/VulkanMemoryManager.h"
#include "Model.h"

#include <ranges>

namespace Sce
{

    Model::Model(std::filesystem::path InFilePath, VulkanMemoryManager* inMemoryManager)
        : MemoryManager(inMemoryManager)
    {
        Assimp::Importer Importer;
        const aiScene* Scene = Importer.ReadFile(InFilePath.string().c_str(), aiProcess_Triangulate);
        if (!Scene)
        {
            throw std::runtime_error(Importer.GetErrorString());
        }
        Meshes.reserve(Scene->mNumMeshes);


        for (size_t m = 0; m < Scene->mNumMeshes; ++m)
        {
            aiMesh* Mesh = Scene->mMeshes[m];

            std::vector<uint32_t> indices;
            std::vector<glm::vec3> vertices;
            std::vector<glm::vec4> colors;
            indices.reserve(Mesh->mNumFaces * 3);
            vertices.reserve(Mesh->mNumVertices);
            colors.reserve(Mesh->mNumVertices);

            for (size_t f = 0; f < Mesh->mNumFaces; ++f)
            {
                for (size_t i = 0; i < 3; i++)
                {
                    indices.push_back(Mesh->mFaces[f].mIndices[i]);
                }
            }


            for (size_t i = 0; i < Mesh->mNumVertices; i++)
            {
                vertices.emplace_back(Mesh->mVertices[i].x, Mesh->mVertices[i].y, Mesh->mVertices[i].z);
            }

            if (Mesh->HasVertexColors(0))
            {
                for (size_t i = 0; i < Mesh->mNumVertices; i++)
                {
                    colors.emplace_back(Mesh->mColors[0][i].r, Mesh->mColors[0][i].g, Mesh->mColors[0][i].b, Mesh->mColors[0][i].a);
                }
            }
            else
            {
                for (size_t i = 0; i < Mesh->mNumVertices; i++)
                {
                    colors.emplace_back(0.8, 0.8, 0.8, 1.0);
                }
            }

            Meshes.emplace_back(MemoryManager, std::move(Mesh->mName.C_Str()), std::move(vertices), std::move(indices), std::move(colors));
        }
    }

    Model::~Model()
    {
    }


    uint32_t Model::GetVertexCount() const
    {
        return std::accumulate(Meshes.begin(), Meshes.end(), 0, [](uint32_t sum, const Mesh& mesh) { return sum + static_cast<uint32_t>(mesh.GetVertexCount()); });
    }

    uint32_t Model::GetIndexCount() const
    {
        return std::accumulate(Meshes.begin(), Meshes.end(), 0, [](uint32_t sum, const Mesh& mesh) { return sum + static_cast<uint32_t>(mesh.GetIndexCount()); });
    }


    std::vector<VkVertexInputBindingDescription> Model::GetVertexInputBindingDescriptions()
    {

        VkVertexInputBindingDescription VertexBindingDesc = {
            .binding   = 0,
            .stride    = Mesh::GetVertexStride(),
            .inputRate = VK_VERTEX_INPUT_RATE_VERTEX,
        };

        VkVertexInputBindingDescription ColorBindingDesc = {
            .binding   = 1,
            .stride    = Mesh::GetColorStride(),
            .inputRate = VK_VERTEX_INPUT_RATE_VERTEX,
        };

        std::vector<VkVertexInputBindingDescription> Descriptions(2);
        Descriptions[0] = VertexBindingDesc;
        Descriptions[1] = ColorBindingDesc;

        return Descriptions;
    }
}