#pragma once

#include <vulkan/vulkan.h>

#include <vector>

class VulkanRenderTargetLayout
{
public:
    API VulkanRenderTargetLayout() = default;

    uint32_t GetAttachmentDescriptionCount() const { return static_cast<uint32_t>(Descriptions.size()); }
    VkAttachmentDescription* GetAttachmentDescriptions() { return Descriptions.data(); }

    uint32_t GetInputAttachmentReferenceCount() const { return static_cast<uint32_t>(InputReferences.size()); }
    VkAttachmentReference* GetInputAttachmentReferences() { return InputReferences.data(); }

    uint32_t GetColorAttachmentReferenceCount() const { return static_cast<uint32_t>(ColorReferences.size()); }
    VkAttachmentReference* GetColorAttachmentReferences() { return ColorReferences.data(); }
    VkAttachmentReference* GetResolveAttachmentReferences() { return ResolveReferences.data(); }

    VkAttachmentReference* GetDepthStencilAttachmentReference()
    {
        if (bHasDepthStencilAttachment)
        {
            return &DepthStencilReference;
        }
        else
        {
            return nullptr;
        }
    }


private:
    VkExtent2D Extent;

    std::vector<VkAttachmentDescription> Descriptions;

    std::vector<VkAttachmentReference> ColorReferences;
    std::vector<VkAttachmentReference> InputReferences;
    std::vector<VkAttachmentReference> ResolveReferences;

    bool bHasDepthStencilAttachment             = false;
    VkAttachmentReference DepthStencilReference = {};
};
