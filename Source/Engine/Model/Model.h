#pragma once

#include "Engine/pch.h"

#include "Engine/Platform/Platform.h"
#include "Material.h"
#include "Mesh.h"

#include <numeric>

class VulkanMemoryManager;

namespace Sce
{
    class Model
    {
    public:
        API Model(std::filesystem::path InFilePath, VulkanMemoryManager* inMemoryManager);
        API ~Model();
        std::string GetName() const { return Name; }

        API uint32_t GetVertexCount() const;

        API uint32_t GetIndexCount() const;

        API uint32_t GetMeshCount() { return static_cast<uint32_t>(Meshes.size()); }
        API std::shared_ptr<Mesh> GetMesh(size_t index) const { return Meshes[index]; }


        static std::vector<VkVertexInputBindingDescription> GetVertexInputBindingDescriptions();

    private:
        std::vector<std::shared_ptr<Mesh>> Meshes;
        std::vector<std::shared_ptr<Material>> Materials;
        std::string Name;
        VulkanMemoryManager* MemoryManager;

        void LoadMesh(const aiScene* Scene);
        void LoadMaterial(const aiScene* scene);
    };
}