#pragma once
#include <Engine/Platform/Platform.h>
#include <vulkan/vulkan.h>

#include <vector>

class VulkanDevice;

class VulkanInstance
{
public:
    API VulkanInstance();
    API VulkanInstance(bool InRenderOffscreen, bool InEnableValidation);
    API void Init();
    API void Destroy() const;
    API bool IsRenderOffscreen() const { return RenderOffscreen; }
    API VkInstance GetInstanceHandle() const { return Instance; }

private:
    VkInstance Instance;
    VulkanDevice* Device;
    std::vector<const char*> InstanceExtensions;
    std::vector<const char*> InstanceLayers;
    bool RenderOffscreen;
    bool EnableValidation;

    static void GetInstanceLayers(std::vector<const char*>& instanceLayers, bool enableValidation);
    static void GetInstanceExtensions(
        std::vector<const char*>& InstanceExtensions, const bool enableValidation, bool RenderOffscreen);
};
