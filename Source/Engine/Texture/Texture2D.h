#pragma once

#include "Engine/pch.h"

#include "Engine/Platform/Platform.h"

class VulkanMemoryManager;

class Texture2D
{
public:
    API Texture2D(std::filesystem::path inFilePath);
    API VkImage GetVkImageHandle(VulkanMemoryManager* inMemManager);

private:
    std::filesystem::path FilePath;
    std::vector<uint8_t> ImageData;

    VkExtent3D Extent;

    VkImage Image;
};
