#include "Engine/pch.h"

#include "Engine/Vulkan/VulkanMemoryManager.h"
#include "Texture2D.h"

Texture2D::Texture2D(std::filesystem::path inFilePath)
    : Image(VK_NULL_HANDLE)
    , FilePath(inFilePath)
{
    int width, height, channels;
    uint8_t* data = stbi_load(inFilePath.string().c_str(), reinterpret_cast<int*>(&Extent.width), reinterpret_cast<int*>(&Extent.height), &channels, STBI_rgb_alpha);

    ImageData.resize(Extent.width * Extent.height * STBI_rgb_alpha);
    std::memcpy(ImageData.data(), data, Extent.width * Extent.height * STBI_rgb_alpha);
    stbi_image_free(data);
    Extent.depth = 1;
}

VkImage Texture2D::GetVkImageHandle(VulkanMemoryManager* inMemManager)
{
    if (Image == VK_NULL_HANDLE)
    {
        // automatically convert from uint to float
        Image = inMemManager->CreateImage(ImageData, Extent, VK_FORMAT_R8G8B8A8_SRGB, VMA_MEMORY_USAGE_GPU_ONLY, VK_IMAGE_USAGE_SAMPLED_BIT, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
    }
    return Image;
}
