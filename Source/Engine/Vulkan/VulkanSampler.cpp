#include "Engine/pch.h"

#include "VulkanDevice.h"
#include "VulkanSampler.h"

VulkanSampler::VulkanSampler(VulkanDevice* inDevice, VkFilter inFilter, VkSamplerMipmapMode inMipmapMode, VkSamplerAddressMode inAddressMode)
    : Device(inDevice)
    , Sampler(VK_NULL_HANDLE)
{
    VkSamplerCreateInfo SamplerInfo = {
        .sType                   = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
        .magFilter               = inFilter,
        .minFilter               = inFilter,
        .mipmapMode              = inMipmapMode,
        .addressModeU            = inAddressMode,
        .addressModeV            = inAddressMode,
        .addressModeW            = inAddressMode,
        .mipLodBias              = 0.0f,
        .anisotropyEnable        = VK_TRUE,
        .maxAnisotropy           = 16.f,
        .compareEnable           = VK_FALSE,
        .minLod                  = 0.f,
        .maxLod                  = 0.f,
        .borderColor             = VK_BORDER_COLOR_FLOAT_OPAQUE_BLACK,
        .unnormalizedCoordinates = VK_FALSE,
    };
    vkCreateSampler(Device->GetDeviceHandle(), &SamplerInfo, nullptr, &Sampler);
}
