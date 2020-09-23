#pragma once

#include "Engine/pch.h"

class VulkanDevice;

class VulkanSampler
{
public:
    API VulkanSampler(VulkanDevice* inDevice, VkFilter inFilter, VkSamplerMipmapMode inMipmapMode, VkSamplerAddressMode inAddressMode);
    API VkSampler GetSamplerHandle() { return Sampler; }

private:
    VulkanDevice* Device;
    VkSampler Sampler;
};
