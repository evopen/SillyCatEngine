#include "VulkanInstance.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

#define KHRONOS_VALIDATION_LAYER_NAME "VK_LAYER_KHRONOS_validation"

VulkanInstance::VulkanInstance()
    : Instance(VK_NULL_HANDLE)
    , Device(nullptr)
{
}


void VulkanInstance::Init()
{
    VkApplicationInfo appInfo         = {.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO, .apiVersion = VK_API_VERSION_1_2};
    VkInstanceCreateInfo instanceInfo = {
        .sType                   = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pApplicationInfo        = &appInfo,
        .enabledLayerCount       = InstanceLayers.size(),
        .ppEnabledLayerNames     = InstanceLayers.data(),
        .enabledExtensionCount   = InstanceExtensions.size(),
        .ppEnabledExtensionNames = InstanceExtensions.data(),
    };
    vkCreateInstance(&instanceInfo, nullptr, &Instance);
}

void VulkanInstance::Destroy()
{
    if (Instance != VK_NULL_HANDLE)
        vkDestroyInstance(Instance, nullptr);
}

void VulkanInstance::GetInstanceLayers(std::vector<const char*>& instanceLayers, const bool enableValidation)
{
    if (enableValidation)
    {
        instanceLayers.push_back(KHRONOS_VALIDATION_LAYER_NAME);
    }
}

void VulkanInstance::GetInstanceExtensions(
    std::vector<const char*>& instanceExtensions, const bool enableValidation, const bool renderOffscreen)
{
    if (enableValidation)
    {
        instanceExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }
    if (!renderOffscreen)
    {
        uint32_t glfwExtensionCount;
        const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
        instanceExtensions.reserve(glfwExtensionCount);
        for (uint32_t i = 0; i < glfwExtensionCount; i++)
        {
            instanceExtensions.push_back(glfwExtensions[i]);
        }
    }
}
