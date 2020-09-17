#pragma once

#include "Engine/pch.h"

class VulkanDevice;
class VulkanRenderPass;

class VulkanFramebuffer
{
public:
    API VulkanFramebuffer(VulkanDevice* InDevice, VulkanRenderPass* InRenderPass, uint32_t InWidth, uint32_t InHeight);

private:
    VulkanDevice* Device;
    VulkanRenderPass* RenderPass;
    VkFramebuffer Framebuffer;
    uint32_t Width, Height;
};