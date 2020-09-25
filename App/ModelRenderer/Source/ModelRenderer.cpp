#include "pch.h"


int main()
{
    try
    {
        auto instance = std::make_shared<VulkanInstance>(false, true);
        auto [physicalDevice, physicalDeviceProperties] = VulkanDevice::SelectPhysicalDevice(instance);
        VulkanDevice device(instance, physicalDevice, false);
        VulkanWindowSurface windowSurface(instance, &device, "SillyCatRenderer", 800, 600);
        VulkanSwapchain swapchain(instance, &device, &windowSurface);
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
        VulkanMemoryManager MemoryManager(&device, instance);
        Sce::GUI gui(instance, &device, &MemoryManager, renderPass, swapchain.GetImageCount(), true);

        while (!glfwWindowShouldClose(windowSurface.GetWindowHandle()))
        {
            glfwPollEvents();

            

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