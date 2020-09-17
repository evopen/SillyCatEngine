#include "Engine/pch.h"

#include "VulkanInstance.h"

static VKAPI_ATTR VkBool32 VKAPI_CALL DebugUtilsCallback(VkDebugUtilsMessageSeverityFlagBitsEXT MsgSeverity,
    VkDebugUtilsMessageTypeFlagsEXT MsgType, const VkDebugUtilsMessengerCallbackDataEXT* CallbackData, void* UserData)
{
    spdlog::level::level_enum Severity;
    switch (MsgSeverity)
    {

    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
        Severity = spdlog::level::critical;
        break;
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
        Severity = spdlog::level::warn;
        break;
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
        Severity = spdlog::level::info;
        break;
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
        Severity = spdlog::level::debug;
        break;
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_FLAG_BITS_MAX_ENUM_EXT:
        break;
    default:
        break;
    }
    spdlog::log(Severity, "[VulkanValidation] {}", CallbackData->pMessage);

    return VK_FALSE;
}


void VulkanInstance::SetupDebugLayerCallback()
{
    PFN_vkCreateDebugUtilsMessengerEXT CreateDebugUtilsMessengerEXT = (PFN_vkCreateDebugUtilsMessengerEXT)(
        void*) vkGetInstanceProcAddr(Instance, "vkCreateDebugUtilsMessengerEXT");
    VkDebugUtilsMessengerCreateInfoEXT DebugInfo = {
        .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
        .messageSeverity =
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT,
        .messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT
                       | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT
                       | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,

        .pfnUserCallback = DebugUtilsCallback,
    };

    if (CreateDebugUtilsMessengerEXT(Instance, &DebugInfo, nullptr, &DebugMessenger) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create debug callback");
    }
}
