#pragma once

#include "Engine/pch.h"

class VulkanDevice;

class VulkanImageView
{
public:
    VulkanImageView(VulkanDevice* Device, VkImage InImage);

private:
    VkImageView ImageView;
    VkImage Image;
    VulkanDevice* Device;
};
