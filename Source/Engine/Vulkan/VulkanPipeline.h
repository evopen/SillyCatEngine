#pragma once

#include <vulkan/vulkan.h>

class VulkanDevice;

class VulkanPipeline
{
public:
    VulkanPipeline(VulkanDevice* InDevice);
private:
    VkPipeline Pipieline;
    VulkanDevice* Device;
};