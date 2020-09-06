#pragma once
#include <vulkan/vulkan_core.h>

#include <stdexcept>

static inline void CheckResult(VkBool32 Result)
{
    if (Result != VK_SUCCESS)
    {
        std::runtime_error("Vulkan function exception!");
    }
}
