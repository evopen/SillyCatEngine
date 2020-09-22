#pragma once

#include "Engine/Platform/Platform.h"

#include <vulkan/vulkan.h>

class VulkanRenderPass;
class VulkanPipelineLayout;
class VulkanDevice;
class VulkanPipelineState;
class VulkanGraphicsPipelineState;
class VulkanComputePipelineState;

class VulkanPipeline
{
public:
    API VulkanPipeline(VulkanDevice* InDevice);

    API VkPipeline GetPipelineHandle() { return Pipeline; }

protected:
    VkPipeline Pipeline;
    VulkanDevice* Device;
};

class VulkanGraphicsPipeline : public VulkanPipeline
{
public:
    API VulkanGraphicsPipeline(VulkanDevice* InDevice, VulkanRenderPass* InRenderPass, VulkanGraphicsPipelineState* InVulkanGraphicsPipelineState);

private:
    VulkanGraphicsPipelineState* PipelineState;
    VulkanRenderPass* RenderPass;
    std::vector<VkVertexInputBindingDescription> VertexInputBindingDescriptions;
    std::vector<VkVertexInputAttributeDescription> VertexInputAttributeDescriptions;
};

class VulkanComputePipeline : public VulkanPipeline
{
public:
    API VulkanComputePipeline(VulkanDevice* InDevice, VulkanPipelineLayout* InLayout, VulkanComputePipelineState* InComputePipelineState);

private:
    VulkanComputePipelineState* PipelineState;
};