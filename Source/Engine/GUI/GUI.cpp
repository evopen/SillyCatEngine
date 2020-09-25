#include "Engine/pch.h"

#include "Engine/Vulkan/VulkanDevice.h"
#include "Engine/Vulkan/VulkanInstance.h"
#include "Engine/Vulkan/VulkanMemoryManager.h"
#include "Engine/Vulkan/VulkanQueue.h"
#include "Engine/Vulkan/VulkanRenderPass.h"
#include "Engine/Vulkan/VulkanUtil.h"
#include "Engine/Vulkan/VulkanWindowSurface.h"
#include "GUI.h"

namespace Sce
{
    GUI::GUI(VulkanInstance* inInstance, VulkanDevice* inDevice, VulkanMemoryManager* inMemoryManager, std::shared_ptr<VulkanRenderPass> inRenderPass, uint32_t inImageCount)
        : Instance(inInstance)
        , Device(inDevice)
        , MemoryManager(inMemoryManager)
        , RenderPass(inRenderPass)
    {
        Context = ImGui::CreateContext();
        ImGui::StyleColorsClassic();
        ImGui_ImplGlfw_InitForVulkan(Window->GetWindowHandle(), true);
        ImGui_ImplVulkan_InitInfo ImGuiVulkanInfo = {
            .Instance        = Instance->GetInstanceHandle(),
            .PhysicalDevice  = Device->GetPhysicalDeviceHandle(),
            .Device          = Device->GetDeviceHandle(),
            .QueueFamily     = Device->GetGraphicsQueue()->GetFamilyIndex(),
            .Queue           = Device->GetGraphicsQueue()->GetHandle(),
            .PipelineCache   = nullptr,
            .DescriptorPool  = MemoryManager->GetDescriptorPoolHandle(),
            .MinImageCount   = 2,
            .ImageCount      = inImageCount,
            .MSAASamples     = VK_SAMPLE_COUNT_1_BIT,
            .CheckVkResultFn = CheckResult,
        };

        ImGui_ImplVulkan_Init(&ImGuiVulkanInfo, RenderPass->GetRenderPassHandle());
    }

    GUI::~GUI()
    {
        ImGui::DestroyContext(Context);
        ImGui_ImplGlfw_Shutdown();
    }
}
