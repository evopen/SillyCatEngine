#include "pch.h"

#include "Engine/Vulkan/VulkanCommandBuffer.h"
#include "Engine/Vulkan/VulkanFence.h"
#include "Engine/Vulkan/VulkanFramebuffer.h"
#include "Engine/Vulkan/VulkanImageView.h"
#include "Engine/Vulkan/VulkanPipeline.h"
#include "Engine/Vulkan/VulkanPipelineLayout.h"
#include "Engine/Vulkan/VulkanPipelineState.h"
#include "Engine/Vulkan/VulkanPresenter.h"
#include "Engine/Vulkan/VulkanQueue.h"
#include "Engine/Vulkan/VulkanRenderPass.h"
#include "Engine/Vulkan/VulkanRenderTargetLayout.h"
#include "Engine/Vulkan/VulkanSemaphore.h"


TEST(VulkanInstanceTest, Initialization)
{
    VulkanInstance instance(false, true);
    instance.Init();
}

TEST(VulkanInstanceTest, Destruction)
{
    VulkanInstance instance;
    instance.Init();
    instance.Destroy();
}

TEST(VulkanDeviceTest, SelectDiscreteGPU)
{
    VulkanInstance instance;
    instance.Init();
    auto [physicalDevice, deviceProp] = VulkanDevice::SelectPhysicalDevice(instance);
    EXPECT_NE(physicalDevice == VK_NULL_HANDLE, true);
    spdlog::info("Select Device: {}", deviceProp.deviceName);
}

TEST(VulkanDeviceTest, CreateDevice)
{
    VulkanInstance instance(false, true);
    instance.Init();
    auto [physicalDevice, deviceProp] = VulkanDevice::SelectPhysicalDevice(instance);
    EXPECT_NE(physicalDevice == VK_NULL_HANDLE, true);
    spdlog::info("Select Device: {}", deviceProp.deviceName);
    VulkanDevice device(&instance, physicalDevice, false);
    device.Init();
}


TEST(SelfTest, EQUAL)
{
    EXPECT_EQ(1, 2 - 1);
}

// int main(int argc, char* argv[])
//{
//    ::testing::InitGoogleTest(&argc, argv);
//    return RUN_ALL_TESTS();
//}

using namespace std::chrono_literals;


int main()
{
    try
    {
        VulkanInstance Instance;
        Instance.Init();
        auto [physicalDevice, deviceProp] = VulkanDevice::SelectPhysicalDevice(Instance);
        spdlog::info("select: {}", deviceProp.deviceName);
        VulkanDevice Device(&Instance, physicalDevice, false);
        Device.Init();
        VulkanWindowSurface WindowSurface(&Instance, &Device, "numerous", 800, 600);
        VulkanSwapchain Swapchain(&Instance, &Device, &WindowSurface);

        VulkanVertexShader VertexShader(&Device, "Test/Shaders/shader.vert");
        VulkanPixelShader FragmentShader(&Device, "Test/Shaders/shader.frag");
        VulkanGraphicsShaderProgram TriangleProgram(&VertexShader, &FragmentShader);
        VulkanGraphicsPipelineState TrianglePipelineState(&TriangleProgram);
        VulkanRenderTargetLayout RTLayout(1);
        VulkanRenderPass Renderpass(&Device, &RTLayout);
        VulkanGraphicsPipelineLayout PipelineLayout(&Device);
        VulkanGraphicsPipeline Pipeline(&Device, &PipelineLayout, &Renderpass, &TrianglePipelineState);
        VulkanSemaphore WaitImageSemaphore(&Device);
        VulkanFence WaitFence(&Device);
        VulkanCommandBuffer CmdBuffer(&Device, Device.GetGraphicsQueue());
        VulkanPresenter Presenter(Device.GetPresentQueue(), &Swapchain);

        std::vector<std::shared_ptr<VulkanImageView>> SwapchainImageViews;
        SwapchainImageViews.reserve(Swapchain.GetImageCount());
        std::vector<std::shared_ptr<VulkanFramebuffer>> Framebuffers;
        Framebuffers.reserve(Swapchain.GetImageCount());

        for (uint32_t i = 0; i < Swapchain.GetImageCount(); i++)
        {
            SwapchainImageViews.emplace_back(std::make_shared<VulkanImageView>(&Device, Swapchain.GetImage(i)));

            Framebuffers.emplace_back(std::make_shared<VulkanFramebuffer>(&Device, &Renderpass, std::vector<std::shared_ptr<VulkanImageView>>{SwapchainImageViews[i]}, 800, 600));
        }


        while (!glfwWindowShouldClose(WindowSurface.GetWindowHandle()))
        {


            glfwPollEvents();
            uint32_t ImageIndex;
            vkAcquireNextImageKHR(Device.GetDeviceHandle(),
                Swapchain.GetSwapchainHandle(),
                std::numeric_limits<uint32_t>::max(),
                VK_NULL_HANDLE,
                WaitFence.GetHandle(),
                &ImageIndex);
            std::vector<VkFence> FencesToWait{WaitFence.GetHandle()};
            WaitFence.Wait();
            WaitFence.Reset();
            spdlog::info(ImageIndex);

            {
                CmdBuffer.Begin();
                CmdBuffer.BeginRenderPass(&Renderpass, Framebuffers[ImageIndex]);


                vkCmdBindPipeline(CmdBuffer.GetHandle(), VK_PIPELINE_BIND_POINT_GRAPHICS, Pipeline.GetPipelineHandle());
                VkViewport Viewport = {
                    .x        = 0,
                    .y        = 0,
                    .width    = static_cast<float>(WindowSurface.GetWidth()),
                    .height   = static_cast<float>(WindowSurface.GetHeight()),
                    .minDepth = 0.f,
                    .maxDepth = 1.f,
                };
                VkRect2D Scissor = {
                    .offset = {
                        .x = 0,
                        .y = 0,
                    },
                    .extent = {
                        .width  = WindowSurface.GetWidth(),
                        .height = WindowSurface.GetHeight(),
                    },
                };
                vkCmdSetViewport(CmdBuffer.GetHandle(), 0, 1, &Viewport);
                vkCmdSetScissor(CmdBuffer.GetHandle(), 0, 1, &Scissor);


                vkCmdDraw(CmdBuffer.GetHandle(), 3, 1, 0, 0);


                VkImageMemoryBarrier ImgMemBarrier = {
                    .sType            = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
                    .srcAccessMask    = 0,
                    .dstAccessMask    = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT,
                    .oldLayout        = VK_IMAGE_LAYOUT_UNDEFINED,
                    .newLayout        = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
                    .image            = Swapchain.GetImage(ImageIndex),
                    .subresourceRange = {
                        .aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT,
                        .baseMipLevel   = 0,
                        .levelCount     = 1,
                        .baseArrayLayer = 0,
                        .layerCount     = 1,
                    },
                };


                CmdBuffer.EndRenderPass();

                vkCmdPipelineBarrier(CmdBuffer.GetHandle(),
                    VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
                    0,
                    0, nullptr,
                    0, nullptr,
                    1, &ImgMemBarrier);

                CmdBuffer.End();
            }

            CmdBuffer.Submit({}, {}, {}, &WaitFence);
            WaitFence.Wait();
            WaitFence.Reset();
            Presenter.Present();
        }
    }
    catch (std::exception& e)
    {
        spdlog::critical(e.what());
    }

    return 0;
}