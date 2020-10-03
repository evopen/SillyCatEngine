#pragma once

#include "Engine/pch.h"

#include "Engine/Platform/Platform.h"


class VulkanRenderTargetLayout
{
public:
    API VulkanRenderTargetLayout(uint32_t ColorRenderTargetCount, bool inUseDepthBuffer);

    uint32_t GetAttachmentDescriptionCount() const { return static_cast<uint32_t>(Descriptions.size()); }
    VkAttachmentDescription* GetAttachmentDescriptions() { return Descriptions.data(); }

    uint32_t GetInputAttachmentReferenceCount() const { return static_cast<uint32_t>(InputReferences.size()); }
    VkAttachmentReference* GetInputAttachmentReferences() { return InputReferences.data(); }

    uint32_t GetColorAttachmentReferenceCount() const { return static_cast<uint32_t>(ColorReferences.size()); }
    VkAttachmentReference* GetColorAttachmentReferences() { return ColorReferences.data(); }
    VkAttachmentReference* GetResolveAttachmentReferences() { return ResolveReferences.data(); }

    VkAttachmentReference* GetDepthStencilAttachmentReference() { return DepthStencilReference.has_value() ? &DepthStencilReference.value() : nullptr; }


private:
    std::vector<VkAttachmentDescription> Descriptions;

    std::vector<VkAttachmentReference> ColorReferences;
    std::vector<VkAttachmentReference> InputReferences;
    std::vector<VkAttachmentReference> ResolveReferences;

    std::optional<VkAttachmentReference> DepthStencilReference;
};
