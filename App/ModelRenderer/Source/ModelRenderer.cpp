#include "pch.h"

#include <imgui_impl_glfw.h>
#include <imgui_impl_vulkan.h>

int main()
{
    try
    {
        VulkanInstance instance(false, true);
        auto [physicalDevice, physicalDeviceProperties] = VulkanDevice::SelectPhysicalDevice(&instance);
        VulkanDevice device(&instance, physicalDevice, false);
        VulkanWindowSurface windowSurface(&instance, &device, "SillyCatRenderer", 800, 600);
        VulkanSwapchain swapchain(&instance, &device, &windowSurface);
        VulkanPresenter presenter(device.GetPresentQueue(), &swapchain);
        VulkanCommandBuffer cmdBuffer(&device, device.GetGraphicsQueue());
        auto vertexShader = std::make_shared<VulkanVertexShader>(&device, "App/ModelRenderer/Source/Shader/BaseColor.vert");
        auto pixelShader  = std::make_shared<VulkanPixelShader>(&device, "App/ModelRenderer/Source/Shader/BaseColor.frag");
        VulkanGraphicsShaderProgram shaderProgram(&device, vertexShader, pixelShader);
        VulkanGraphicsPipelineState pipelineState(&shaderProgram);
        VulkanRenderTargetLayout RTLayout(1);
        auto renderPass = std::make_shared<VulkanRenderPass>(&device, &RTLayout);
        VulkanGraphicsPipeline pipeline(&device, renderPass, &pipelineState);
        VulkanSemaphore ImageAvailable(&device);
        VulkanSemaphore RenderFinish(&device);
        VulkanMemoryManager MemoryManager(&device, &instance);

        {
            ImGui::CreateContext();

            ImGuiIO& io = ImGui::GetIO();
            ImGui::StyleColorsDark();
            ImGui_ImplGlfw_InitForVulkan(windowSurface.GetWindowHandle(), true);

            ImGui_ImplVulkan_InitInfo ImGuiVulkanInfo = {
                .Instance        = instance.GetInstanceHandle(),
                .PhysicalDevice  = device.GetPhysicalDeviceHandle(),
                .Device          = device.GetDeviceHandle(),
                .QueueFamily     = device.GetGraphicsQueue()->GetFamilyIndex(),
                .Queue           = device.GetGraphicsQueue()->GetHandle(),
                .PipelineCache   = nullptr,
                .DescriptorPool  = MemoryManager.GetDescriptorPoolHandle(),
                .MinImageCount   = 2,
                .ImageCount      = swapchain.GetImageCount(),
                .MSAASamples     = VK_SAMPLE_COUNT_1_BIT,
                .CheckVkResultFn = CheckResult,
            };

            ImGui_ImplVulkan_Init(&ImGuiVulkanInfo, renderPass->GetRenderPassHandle());

            VulkanCommandBuffer fontUploadCmdBuf(&device, device.GetTransferQueue());
            fontUploadCmdBuf.Begin();
            ImGui_ImplVulkan_CreateFontsTexture(fontUploadCmdBuf.GetHandle());
            fontUploadCmdBuf.End();
            fontUploadCmdBuf.Submit();
            fontUploadCmdBuf.Wait();
            ImGui_ImplVulkan_DestroyFontUploadObjects();
        }

        bool bShowDemoWindow = true;

        while (!glfwWindowShouldClose(windowSurface.GetWindowHandle()))
        {
            glfwPollEvents();

            ImGui_ImplVulkan_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
            ImGui::ShowDemoWindow(&bShowDemoWindow);
            ImGui::Render();

            std::vector<std::shared_ptr<VulkanFramebuffer>> framebuffers;
            std::vector<std::shared_ptr<VulkanImageView>> swapchainImageViews;
            for (uint32_t i = 0; i < swapchain.GetImageCount(); i++)
            {
                swapchainImageViews.emplace_back(std::make_shared<VulkanImageView>(&device, swapchain.GetImage(i)));
                framebuffers.emplace_back(std::make_shared<VulkanFramebuffer>(&device, renderPass, std::vector<std::shared_ptr<VulkanImageView>>{swapchainImageViews[i]}, windowSurface.GetWidth(), windowSurface.GetHeight()));
            }

            swapchain.AcquireNextImage(ImageAvailable.GetHandle(), VK_NULL_HANDLE);

            cmdBuffer.Wait();
            cmdBuffer.Begin();
            cmdBuffer.BeginRenderPass(renderPass, framebuffers[swapchain.GetRenderIndex()]);
            VkViewport Viewport = windowSurface.GetSurfaceViewport();
            VkRect2D Scissor    = windowSurface.GetSurfaceScissor();
            vkCmdSetViewport(cmdBuffer.GetHandle(), 0, 1, &Viewport);
            vkCmdSetScissor(cmdBuffer.GetHandle(), 0, 1, &Scissor);
            //ImGui::Text("Hello, world %d", 123);


            ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), cmdBuffer.GetHandle());

            cmdBuffer.EndRenderPass();
            swapchain.CmdTransitImagePresentSrc(&cmdBuffer);
            cmdBuffer.End();
            cmdBuffer.Submit({ImageAvailable.GetHandle()}, {VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT}, {RenderFinish.GetHandle()});
            presenter.Present({RenderFinish.GetHandle()});
        }

        return 0;
    }
    catch (std::exception& e)
    {
        spdlog::critical(e.what());
    }
}