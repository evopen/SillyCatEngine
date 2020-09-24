#include "Engine/pch.h"

#include "VulkanUtil.h"

#include <stdexcept>

void CheckResult(VkResult Result)
{
    if (Result != VK_SUCCESS)
    {
        throw std::runtime_error("Vulkan function exception!");
    }
}
