#include "Engine/pch.h"

#include "VulkanDevice.h"
#include "VulkanImageView.h"

VulkanImageView::VulkanImageView(VulkanDevice* InDevice, VkImage InImage)
    : ImageView(VK_NULL_HANDLE)
    , Image(InImage)
    , Device(InDevice)
{
    VkImageViewCreateInfo ImageViewInfo = {
        .sType      = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
        .image      = Image,
        .viewType   = VK_IMAGE_VIEW_TYPE_2D,
        .format     = VK_FORMAT_B8G8R8A8_SRGB,
        .components = {
            .r = VK_COMPONENT_SWIZZLE_IDENTITY,
            .g = VK_COMPONENT_SWIZZLE_IDENTITY,
            .b = VK_COMPONENT_SWIZZLE_IDENTITY,
            .a = VK_COMPONENT_SWIZZLE_IDENTITY,
        },
        .subresourceRange = {
            .aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT,
            .baseMipLevel   = 0,
            .levelCount     = 1,
            .baseArrayLayer = 0,
            .layerCount     = 1,
        },
    };

    vkCreateImageView(Device->GetDeviceHandle(), &ImageViewInfo, nullptr, &ImageView);
}

VulkanImageView::~VulkanImageView()
{
    vkDestroyImageView(Device->GetDeviceHandle(), ImageView, nullptr);
}
