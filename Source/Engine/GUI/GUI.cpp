#include "Engine/pch.h"

#include "Engine/Vulkan/VulkanCommandBuffer.h"
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
    GUI::GUI(std::shared_ptr<VulkanInstance> inInstance, VulkanDevice* inDevice, std::shared_ptr<VulkanWindowSurface> inWindowSurface, std::shared_ptr<VulkanRenderPass> inRenderPass, uint32_t inImageCount)
        : Instance(inInstance)
        , Device(inDevice)
        , Window(inWindowSurface)
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
            .DescriptorPool  = Device->GetMemoryManager()->GetDescriptorPoolHandle(),
            .MinImageCount   = 2,
            .ImageCount      = inImageCount,
            .MSAASamples     = VK_SAMPLE_COUNT_1_BIT,
            .CheckVkResultFn = CheckResult,
        };

        ImGui_ImplVulkan_Init(&ImGuiVulkanInfo, RenderPass->GetRenderPassHandle());

        VulkanCommandBuffer fontUploadCmdBuf(Device, Device->GetTransferQueue());
        fontUploadCmdBuf.Begin();
        ImGui_ImplVulkan_CreateFontsTexture(fontUploadCmdBuf.GetHandle());
        fontUploadCmdBuf.End();
        fontUploadCmdBuf.Submit();
        fontUploadCmdBuf.Wait();
        ImGui_ImplVulkan_DestroyFontUploadObjects();
    }

    GUI::~GUI()
    {
        ImGui::DestroyContext(Context);
        ImGui_ImplGlfw_Shutdown();
    }

    void GUI::NewFrame()
    {
        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void GUI::Render(VulkanCommandBuffer* inCmdBuffer)
    {
        ImGui::Render();
        ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), inCmdBuffer->GetHandle());
        inCmdBuffer->PossessObject(shared_from_this());
    }
}
