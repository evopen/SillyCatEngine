#pragma once
#include <vulkan/vulkan.h>

#include <vector>

class VulkanDevice;

class VulkanInstance
{
public:
    VulkanInstance();
    void Init();
    void Destroy();
    bool IsRenderOffscreen() const { return RenderOffscreen; }

private:
    VkInstance Instance;
    VulkanDevice* Device;
    std::vector<const char*> InstanceExtensions;
    std::vector<const char*> InstanceLayers;
    bool RenderOffscreen;
    bool EnableValidation;

    static void GetInstanceLayers(std::vector<const char*>& instanceLayers, bool enableValidation);
    static void GetInstanceExtensions(std::vector<const char*>& InstanceExtensions, bool RenderOffscreen);
};
