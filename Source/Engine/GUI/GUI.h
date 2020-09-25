#pragma once

class VulkanMemoryManager;
class VulkanWindowSurface;
class VulkanRenderPass;
namespace Sce
{
    class GUI
    {
    public:
        GUI(VulkanInstance* inInstance, VulkanDevice* inDevice, VulkanMemoryManager* inMemoryManager, std::shared_ptr<VulkanRenderPass> inRenderPass, uint32_t inImageCount);
        ~GUI();

    private:
        ImGuiContext* Context;
        std::shared_ptr<VulkanWindowSurface> Window;
        VulkanInstance* Instance;
        VulkanDevice* Device;
        VulkanMemoryManager* MemoryManager;
        std::shared_ptr<VulkanRenderPass> RenderPass;
    };
}