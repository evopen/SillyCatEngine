#pragma once

#include "Engine/pch.h"

#include "Engine/Platform/Platform.h"

class VulkanMemoryManager;


namespace Sce
{
    class Texture
    {
    public:
        API Texture(std::filesystem::path inFilePath);
        API Texture(void* inBuffer, size_t len);
        API VkImage GetVkImageHandle(VulkanMemoryManager* inMemManager);

    private:
        std::filesystem::path FilePath;
        std::vector<uint8_t> ImageData;

        VkExtent3D Extent;

        VkImage Image;
    };
}