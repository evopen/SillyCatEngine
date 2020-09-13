#pragma once

#include <vulkan/vulkan.h>

class VulkanPipelineState
{
public:
    VulkanPipelineState()
    {
        VkGraphicsPipelineCreateInfo PipelineInfo = {
            .sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,

        };
    }
private:

};