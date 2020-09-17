#pragma once

#include <vulkan/vulkan.h>

class VulkanDevice;

class VulkanPipelineLayout
{
public:
    API VulkanPipelineLayout(VulkanDevice* InDevice);
    API VkPipelineLayout GetLayoutHandle() { return Layout; }

protected:
    VkPipelineLayout Layout;
    VulkanDevice* Device;
};


class VulkanComputePipelineLayout : public VulkanPipelineLayout
{
public:
    API VulkanComputePipelineLayout(VulkanDevice* InDevice);
};

class VulkanGraphicsPipelineLayout : public VulkanPipelineLayout
{
public:
    API VulkanGraphicsPipelineLayout(VulkanDevice* InDevice);
};
