#pragma once

#include <vulkan/vulkan.h>

class VulkanDevice;

class VulkanPipelineLayout
{
public:
    VulkanPipelineLayout(VulkanDevice* InDevice);
private:
    VkPipelineLayout Layout;
    VulkanDevice* Device;
};


class VulkanComputePipelineLayout : public VulkanPipelineLayout
{
    VulkanComputePipelineLayout(VulkanDevice* InDevice);
};

class VulkanGraphicsPipelineLayout : public VulkanPipelineLayout
{
     VulkanGraphicsPipelineLayout(VulkanDevice* InDevice);
};
