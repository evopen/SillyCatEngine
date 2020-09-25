#pragma once

#include "Engine/pch.h"

class VulkanDevice;
class VulkanRenderPass;
class VulkanImageView;

class VulkanFramebuffer
{
public:
    API VulkanFramebuffer(VulkanDevice* InDevice, std::shared_ptr<VulkanRenderPass> InRenderPass, std::vector<std::shared_ptr<VulkanImageView>> InImageViews, uint32_t InWidth, uint32_t InHeight);
    API ~VulkanFramebuffer();

    VkFramebuffer GetHandle() { return Framebuffer; }
    uint32_t GetWidth() const { return Width; }
    uint32_t GetHeight() const { return Height; }

private:
    VulkanDevice* Device;
    std::shared_ptr<VulkanRenderPass> RenderPass;
    VkFramebuffer Framebuffer;
    uint32_t Width, Height;
    std::vector<std::shared_ptr<VulkanImageView>> ImageViews;
};