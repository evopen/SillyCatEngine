#pragma once

class VulkanMemoryManager;
class VulkanWindowSurface;
class VulkanRenderPass;
class VulkanInstance;
class VulkanDevice;
class VulkanCommandBuffer;

namespace Sce
{
    class GUI : public std::enable_shared_from_this<GUI>
    {
    public:
        API GUI(std::shared_ptr<VulkanInstance> inInstance, VulkanDevice* inDevice, std::shared_ptr<VulkanWindowSurface> inWindowSurface, std::shared_ptr<VulkanRenderPass> inRenderPass, uint32_t inImageCount);
        API ~GUI();

        API void NewFrame();
        API void Render(VulkanCommandBuffer* inCmdBuffer);

        API ImGuiContext* GetContext() { return Context; }

    private:
        ImGuiContext* Context;
        std::shared_ptr<VulkanWindowSurface> Window;
        std::shared_ptr<VulkanInstance> Instance;
        VulkanDevice* Device;
        std::shared_ptr<VulkanRenderPass> RenderPass;
    };
}