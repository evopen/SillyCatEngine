#pragma once

#include "Engine/Platform/Platform.h"

#include <vulkan/vulkan.h>

class VulkanCommandBuffer;
class VulkanRenderPass;
class VulkanPipelineLayout;
class VulkanDevice;
class VulkanPipelineState;
class VulkanGraphicsPipelineState;
class VulkanComputePipelineState;

class VulkanPipeline : public std::enable_shared_from_this<VulkanPipeline>
{
public:
    API VulkanPipeline(VulkanDevice* InDevice);
    API ~VulkanPipeline();

    API virtual void Bind(VulkanCommandBuffer* inCmdBuffer) = 0;

    API VkPipeline GetPipelineHandle() { return Pipeline; }

protected:
    VkPipeline Pipeline;
    VulkanDevice* Device;
};

class VulkanGraphicsPipeline : public VulkanPipeline
{
public:
    API VulkanGraphicsPipeline(VulkanDevice* InDevice, std::shared_ptr<VulkanRenderPass> InRenderPass, std::shared_ptr<VulkanGraphicsPipelineState> InVulkanGraphicsPipelineState);
    void Bind(VulkanCommandBuffer* inCmdBuffer) override;

private:
    std::shared_ptr<VulkanGraphicsPipelineState> PipelineState;
    std::shared_ptr<VulkanRenderPass> RenderPass;
    std::vector<VkVertexInputBindingDescription> VertexInputBindingDescriptions;
    std::vector<VkVertexInputAttributeDescription> VertexInputAttributeDescriptions;
};

class VulkanComputePipeline : public VulkanPipeline
{
public:
    API VulkanComputePipeline(VulkanDevice* InDevice, VulkanPipelineLayout* InLayout, std::shared_ptr<VulkanComputePipelineState> InComputePipelineState);

private:
    std::shared_ptr<VulkanComputePipelineState> PipelineState;
};