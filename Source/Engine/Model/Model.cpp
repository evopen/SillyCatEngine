#include "Engine/pch.h"

#include "Engine/Vulkan/VulkanMemoryManager.h"
#include "Model.h"

Model::Model(std::filesystem::path InFilePath)
    : VertexBuffer(VK_NULL_HANDLE)
    , ColorBuffer(VK_NULL_HANDLE)
{
    Assimp::Importer Importer;
    const aiScene* Scene = Importer.ReadFile(InFilePath.string().c_str(), aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_Triangulate);
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
            Colors[i].r = Mesh->mColors[0][i].r;
            Colors[i].g = Mesh->mColors[0][i].g;
            Colors[i].b = Mesh->mColors[0][i].b;
            Colors[i].a = Mesh->mColors[0][i].a;
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

VkBuffer Model::GetVertexBuffer(VulkanMemoryManager* InMemoryManager)
{
    if (VertexBuffer == VK_NULL_HANDLE)
    {
        size_t BufferSize      = sizeof(glm::vec3) * Vertices.size();
        VkBuffer StagingBuffer = InMemoryManager->CreateBuffer(BufferSize, VMA_MEMORY_USAGE_CPU_ONLY, VK_BUFFER_USAGE_TRANSFER_SRC_BIT);
        void* Data             = InMemoryManager->MapBuffer(StagingBuffer);
        std::memcpy(Data, Vertices.data(), Vertices.size() * sizeof(glm::vec3));
        InMemoryManager->UnMapBuffer(StagingBuffer);
        VertexBuffer = InMemoryManager->CreateBuffer(BufferSize, VMA_MEMORY_USAGE_GPU_ONLY, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
        InMemoryManager->CopyBuffer(StagingBuffer, VertexBuffer);
        InMemoryManager->FreeBuffer(StagingBuffer);
    }
    return VertexBuffer;
}

VkBuffer Model::GetColorBuffer(VulkanMemoryManager* InMemoryManager)
{
    if (ColorBuffer == VK_NULL_HANDLE)
    {
        size_t BufferSize      = sizeof(glm::vec4) * Colors.size();
        VkBuffer StagingBuffer = InMemoryManager->CreateBuffer(BufferSize, VMA_MEMORY_USAGE_CPU_ONLY, VK_BUFFER_USAGE_TRANSFER_SRC_BIT);
        void* Data             = InMemoryManager->MapBuffer(StagingBuffer);
        std::memcpy(Data, Colors.data(), Colors.size() * sizeof(glm::vec4));
        InMemoryManager->UnMapBuffer(StagingBuffer);
        ColorBuffer = InMemoryManager->CreateBuffer(BufferSize, VMA_MEMORY_USAGE_GPU_ONLY, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
        InMemoryManager->CopyBuffer(StagingBuffer, ColorBuffer);
        InMemoryManager->FreeBuffer(StagingBuffer);
    }
    return ColorBuffer;
}


std::vector<VkVertexInputBindingDescription> Model::GetVertexInputBindingDescriptions()
{

    VkVertexInputBindingDescription VertexBindingDesc = {
        .binding   = 0,
        .stride    = sizeof(decltype(Vertices)::value_type),
        .inputRate = VK_VERTEX_INPUT_RATE_VERTEX,
    };

    VkVertexInputBindingDescription ColorBindingDesc = {
        .binding   = 1,
        .stride    = sizeof(decltype(Colors)::value_type),
        .inputRate = VK_VERTEX_INPUT_RATE_VERTEX,
    };

    std::vector<VkVertexInputBindingDescription> Descriptions(2);
    Descriptions[0] = VertexBindingDesc;
    Descriptions[1] = ColorBindingDesc;

    return Descriptions;
}
