#include "pch.h"


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
        auto vertexShader = std::make_shared<VulkanVertexShader>(&device, "App/ModelRenderer/Source/Shader/Shader.vert");
        auto pixelShader  = std::make_shared<VulkanPixelShader>(&device, "App/ModelRenderer/Source/Shader/Shader.frag");
        VulkanGraphicsShaderProgram shaderProgram(&device, vertexShader, pixelShader);
        VulkanGraphicsPipelineState pipelineState(&shaderProgram);
        VulkanRenderTargetLayout RTLayout(1);
        VulkanRenderPass renderPass(&device, &RTLayout);
        VulkanGraphicsPipeline pipeline(&device, &renderPass, &pipelineState);
        VulkanSemaphore ImageAvailable(&device);
        VulkanSemaphore RenderFinish(&device);


        while (!glfwWindowShouldClose(windowSurface.GetWindowHandle()))
        {
            glfwPollEvents();

            std::vector<std::shared_ptr<VulkanFramebuffer>> framebuffers;
            std::vector<std::shared_ptr<VulkanImageView>> swapchainImageViews;
            for (uint32_t i = 0; i < swapchain.GetImageCount(); i++)
            {
                swapchainImageViews.emplace_back(&device, swapchain.GetImage(i));
                framebuffers.emplace_back(std::make_shared<VulkanFramebuffer>(&device, &renderPass, std::vector<std::shared_ptr<VulkanImageView>>{swapchainImageViews[i]}, windowSurface.GetWidth(), windowSurface.GetHeight()));
            }

            swapchain.AcquireNextImage(ImageAvailable.GetHandle(), VK_NULL_HANDLE);

            cmdBuffer.Begin();
            cmdBuffer.BeginRenderPass(&renderPass, framebuffers[swapchain.GetRenderIndex()]);
            VkViewport Viewport = {
                .x        = 0,
                .y        = static_cast<float>(windowSurface.GetHeight()),
                .width    = static_cast<float>(windowSurface.GetWidth()),
                .height   = -static_cast<float>(windowSurface.GetHeight()),
                .minDepth = 0.f,
                .maxDepth = 1.f,
            };
            VkRect2D Scissor = {
                .offset = {
                    .x = 0,
                    .y = 0,
                },
                .extent = {
                    .width  = windowSurface.GetWidth(),
                    .height = windowSurface.GetHeight(),
                },
            };
            vkCmdSetViewport(cmdBuffer.GetHandle(), 0, 1, &Viewport);
            vkCmdSetScissor(cmdBuffer.GetHandle(), 0, 1, &Scissor);
            cmdBuffer.EndRenderPass();
            cmdBuffer.End();
            cmdBuffer.Submit({ImageAvailable.GetHandle()}, {VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT}, {RenderFinish.GetHandle()});
            presenter.Present()
        }

        return 0;
    }
    catch (std::exception& e)
    {
        spdlog::critical(e.what());
    }
}