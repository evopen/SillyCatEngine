#include "pch.h"

#include "Engine/Vulkan/VulkanCommandBuffer.h"
#include "Engine/Vulkan/VulkanFence.h"
#include "Engine/Vulkan/VulkanFramebuffer.h"
#include "Engine/Vulkan/VulkanPipeline.h"
#include "Engine/Vulkan/VulkanPipelineLayout.h"
#include "Engine/Vulkan/VulkanPipelineState.h"
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
        VulkanRenderTargetLayout RTLayout;
        VulkanRenderPass Renderpass(&Device, &RTLayout);
        VulkanGraphicsPipelineLayout PipelineLayout(&Device);
        VulkanGraphicsPipeline(&Device, &PipelineLayout, &Renderpass, &TrianglePipelineState);
        VulkanFramebuffer Framebuffer(&Device, &Renderpass, 800, 600);
        VulkanSemaphore WaitImageSemaphore(&Device);
        VulkanFence WaitFence(&Device);
        VulkanCommandBuffer CmdBuffer(&Device, Device.GetGraphicsQueue());


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
                CmdBuffer.BeginRenderPass(&Renderpass, &Framebuffer);
                CmdBuffer.EndRenderPass();


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
            std::vector<VkSwapchainKHR> Swapchains = {Swapchain.GetSwapchainHandle()};
            VkPresentInfoKHR PresentInfo           = {
                .sType          = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
                .swapchainCount = 1,
                .pSwapchains    = Swapchains.data(),
                .pImageIndices  = &ImageIndex,
            };
            vkQueuePresentKHR(Device.GetPresentQueue()->GetHandle(), &PresentInfo);
        }
    }
    catch (std::exception& e)
    {
        spdlog::critical(e.what());
    }

    return 0;
}