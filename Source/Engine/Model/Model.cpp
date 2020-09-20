#include "Engine/pch.h"

#include "Model.h"

Model::Model(std::filesystem::path InFilePath)
{
    Assimp::Importer Importer;
    const aiScene* Scene = Importer.ReadFile(InFilePath.string().c_str(), aiProcess_Triangulate);
    if (!Scene)
    {
        throw std::runtime_error(Importer.GetErrorString());
    }
    if (Scene->mNumMeshes != 1)
    {
        throw std::runtime_error("not supported yet");
    }

    aiMesh* Mesh           = Scene->mMeshes[0];
    Name                   = Mesh->mName.C_Str();
    unsigned VerticesCount = Mesh->mNumVertices;

    Vertices.resize(VerticesCount);
    Colors.resize(VerticesCount);


    for (size_t i = 0; i < VerticesCount; i++)
    {
        Vertices[i].x = Mesh->mVertices[i].x;
        Vertices[i].y = Mesh->mVertices[i].y;
        Vertices[i].z = Mesh->mVertices[i].z;
    }

    bHasVertexColors = Mesh->HasVertexColors(0) ? true : false;
    if (bHasVertexColors)
    {
        for (size_t i = 0; i < VerticesCount; i++)
        {
            Colors[i].r = Mesh->mColors[0]->r;
            Colors[i].g = Mesh->mColors[0]->g;
            Colors[i].b = Mesh->mColors[0]->b;
            Colors[i].a = Mesh->mColors[0]->a;
        }
    }
    else
    {
        for (size_t i = 0; i < VerticesCount; i++)
        {
            Colors[i].r = 0.8f;
            Colors[i].g = 0.8f;
            Colors[i].b = 0.8f;
            Colors[i].a = 1.0f;
        }
    }
}

std::vector<VkVertexInputBindingDescription> Model::GetVertexInputBindingDescriptions()
{
    VkVertexInputBindingDescription VertexBindingDesc = {
        .binding   = 0,
        .stride    = sizeof(glm::vec3),
        .inputRate = VK_VERTEX_INPUT_RATE_VERTEX,
    };

    VkVertexInputBindingDescription ColorBindingDesc = {
        .binding   = 1,
        .stride    = sizeof(glm::vec4),
        .inputRate = VK_VERTEX_INPUT_RATE_VERTEX,
    };

    std::vector<VkVertexInputBindingDescription> Descriptions(2);
    Descriptions[0] = VertexBindingDesc;
    Descriptions[1] = ColorBindingDesc;

    return Descriptions;
}
