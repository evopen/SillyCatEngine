#pragma once

#include <vulkan/vulkan.h>

class VulkanDevice;
class VulkanRenderTargetLayout;

class VulkanRenderPass
{
public:
    VulkanRenderPass(VulkanDevice* InDevice, VulkanRenderTargetLayout* InRTLayout);

private:
    VkRenderPass RenderPass;
    VulkanDevice* Device;
    VulkanRenderTargetLayout* RTLayout;

};
