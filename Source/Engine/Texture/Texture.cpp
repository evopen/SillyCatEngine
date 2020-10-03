#include "Engine/pch.h"

#include "Engine/Vulkan/VulkanMemoryManager.h"
#include "Texture.h"

namespace Sce
{
    Texture::Texture(std::filesystem::path inFilePath)
        : FilePath(inFilePath)
        , Image(VK_NULL_HANDLE)
    {
        int channels;
        uint8_t* data = stbi_load(inFilePath.string().c_str(), reinterpret_cast<int*>(&Extent.width), reinterpret_cast<int*>(&Extent.height), &channels, STBI_rgb_alpha);

        ImageData.resize(Extent.width * Extent.height * STBI_rgb_alpha);
        std::memcpy(ImageData.data(), data, Extent.width * Extent.height * STBI_rgb_alpha);
        stbi_image_free(data);
        Extent.depth = 1;
    }

    Texture::Texture(void* inBuffer, size_t len)
        : Image(VK_NULL_HANDLE)
    {
        int channels;
        uint8_t* data = stbi_load_from_memory((uint8_t*) inBuffer, static_cast<int>(len), reinterpret_cast<int*>(&Extent.width), reinterpret_cast<int*>(&Extent.height), &channels, STBI_rgb_alpha);
        ImageData.resize(Extent.width * Extent.height * STBI_rgb_alpha);
        std::memcpy(ImageData.data(), data, Extent.width * Extent.height * STBI_rgb_alpha);
        stbi_image_free(data);
        Extent.depth = 1;
    }


    VkImage Texture::GetVkImageHandle(VulkanMemoryManager* inMemManager)
    {
        if (Image == VK_NULL_HANDLE)
        {
            // automatically convert from uint to float
            Image = inMemManager->CreateImage(ImageData, Extent, VK_FORMAT_B8G8R8A8_SRGB, VMA_MEMORY_USAGE_GPU_ONLY, VK_IMAGE_USAGE_SAMPLED_BIT, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
        }
        return Image;
    }
}