#pragma once

#include "Engine/pch.h"

class VulkanDevice;
class VulkanRenderPass;

class VulkanFramebuffer
{
public:
    API VulkanFramebuffer(VulkanDevice* InDevice, VulkanRenderPass* InRenderPass, uint32_t InWidth, uint32_t InHeight);

    VkFramebuffer GetHandle() { return Framebuffer; }
    uint32_t GetWidth() const { return Width; }
    uint32_t GetHeight() const { return Height; }

private:
    VulkanDevice* Device;
    VulkanRenderPass* RenderPass;
    VkFramebuffer Framebuffer;
    uint32_t Width, Height;
};