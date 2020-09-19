#pragma once

#include "Engine/pch.h"

class VulkanDevice;

class VulkanImageView
{
public:
    API VulkanImageView(VulkanDevice* Device, VkImage InImage);
    VkImageView GetHandle() const { return ImageView; }

private:
    VkImageView ImageView;
    VkImage Image;
    VulkanDevice* Device;
};
