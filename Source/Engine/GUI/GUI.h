#pragma once

class VulkanMemoryManager;
class VulkanWindowSurface;
class VulkanRenderPass;
namespace Sce
{
    class GUI : public std::enable_shared_from_this<GUI>
    {
    public:
        API GUI(std::shared_ptr<VulkanInstance> inInstance, VulkanDevice* inDevice, VulkanMemoryManager* inMemoryManager, std::shared_ptr<VulkanWindowSurface> inWindowSurface, std::shared_ptr<VulkanRenderPass> inRenderPass, uint32_t inImageCount, bool inShowDemoWindow);
        API ~GUI();

        API void NewFrame();
        API void Render(VulkanCommandBuffer* inCmdBuffer);

    private:
        ImGuiContext* Context;
        std::shared_ptr<VulkanWindowSurface> Window;
        std::shared_ptr<VulkanInstance> Instance;
        VulkanDevice* Device;
        VulkanMemoryManager* MemoryManager;
        std::shared_ptr<VulkanRenderPass> RenderPass;

        bool bShowDemoWindow;
    };
}