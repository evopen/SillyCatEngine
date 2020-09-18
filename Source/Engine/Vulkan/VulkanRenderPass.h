#pragma once

#include <vulkan/vulkan.h>

class VulkanDevice;
class VulkanRenderTargetLayout;

class VulkanRenderPass
{
public:
    API VulkanRenderPass(VulkanDevice* InDevice, VulkanRenderTargetLayout* InRTLayout);
    VkRenderPass GetRenderPassHandle() { return RenderPass; }

    VulkanRenderTargetLayout* GetRenderTargetLayout() { return RTLayout; }

private:
    VkRenderPass RenderPass;
    VulkanDevice* Device;
    VulkanRenderTargetLayout* RTLayout;
};
