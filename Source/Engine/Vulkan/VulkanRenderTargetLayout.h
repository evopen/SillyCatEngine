#pragma once

#include <vulkan/vulkan.h>

#include <vector>

class VulkanRenderTargetLayout
{
public:
    uint32_t GetAttachmentDescriptionCount() { return Descriptions.size(); }
    VkAttachmentDescription* GetAttachmentDescriptions() { return Descriptions.data(); }

    uint32_t GetInputAttachmentReferenceCount() { return InputReferences.size(); }
    VkAttachmentReference* GetInputAttachmentReferences() { return InputReferences.data(); }

    uint32_t GetColorAttachmentReferenceCount() { return ColorReferences.size(); }
    VkAttachmentReference* GetColorAttachmentReferences() { return ColorReferences.data(); }
    VkAttachmentReference* GetResolveAttachmentReferences() { return ResolveReferences.data(); }

    VkAttachmentReference* GetDepthStencilAttachmentReference() { return &DepthStencilReference; }


private:
    VkExtent2D Extent;

    std::vector<VkAttachmentDescription> Descriptions;

    std::vector<VkAttachmentReference> ColorReferences;
    std::vector<VkAttachmentReference> InputReferences;
    std::vector<VkAttachmentReference> ResolveReferences;
    VkAttachmentReference DepthStencilReference;
};
