#pragma once

#include "Engine/Platform/Platform.h"

#include <vulkan/vulkan.h>

class VulkanDevice;
class VulkanPipelineState;
class VulkanGraphicsPipelineState;
class VulkanComputePipelineState;

class VulkanPipeline
{
public:
    API VulkanPipeline(VulkanDevice* InDevice);

    API VkPipeline* GetPipelineHandle() { return &Pipeline; }

protected:
    VkPipeline Pipeline;
    VulkanDevice* Device;

};

class VulkanGraphicsPipeline : VulkanPipeline
{
public:
    API VulkanGraphicsPipeline(VulkanDevice* InDevice, VulkanGraphicsPipelineState* InVulkanGraphicsPipelineState);

private:
    VulkanGraphicsPipelineState* PipelineState;
};
