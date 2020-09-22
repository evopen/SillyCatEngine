#pragma once

#include <vulkan/vulkan.h>

class VulkanGraphicsShaderProgram;
class VulkanShaderProgram;
class VulkanDevice;

class VulkanPipelineLayout
{
public:
    API VulkanPipelineLayout(VulkanDevice* InDevice);
    API VkPipelineLayout GetLayoutHandle() { return Layout; }

protected:
    VkPipelineLayout Layout;
    VulkanDevice* Device;
    VkDescriptorSetLayout DescriptorSetLayout;
};


class VulkanComputePipelineLayout : public VulkanPipelineLayout
{
public:
    API VulkanComputePipelineLayout(VulkanDevice* InDevice, VulkanShaderProgram* InShaderProgram);

private:
};

class VulkanGraphicsPipelineLayout : public VulkanPipelineLayout
{
public:
    API VulkanGraphicsPipelineLayout(VulkanDevice* InDevice, VulkanGraphicsShaderProgram* InShaderProgram);

private:
    VulkanGraphicsShaderProgram* ShaderProgram;
};
