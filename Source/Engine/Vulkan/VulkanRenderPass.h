#pragma once

class VulkanFramebuffer;
class VulkanCommandBuffer;
class VulkanDevice;
class VulkanRenderTargetLayout;

class VulkanRenderPass : public std::enable_shared_from_this<VulkanRenderPass>
{
public:
    API VulkanRenderPass(VulkanDevice* InDevice, VulkanRenderTargetLayout* InRTLayout);
    VkRenderPass GetRenderPassHandle() { return RenderPass; }

    API void Begin(VulkanCommandBuffer* inCmdBuffer, std::shared_ptr<VulkanFramebuffer> Framebuffer, std::vector<VkClearValue> inClearValues);
    API void End();

    VulkanRenderTargetLayout* GetRenderTargetLayout() { return RTLayout; }

    bool HasDepthBuffer();

private:
    VkRenderPass RenderPass;
    VulkanDevice* Device;
    VulkanRenderTargetLayout* RTLayout;

    std::optional<VulkanCommandBuffer*> BeganInCmdBuf;
};
