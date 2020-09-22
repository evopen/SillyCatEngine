#include "pch.h"


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
        VulkanMemoryManager MemManager(&Device, &Instance);
        VulkanWindowSurface WindowSurface(&Instance, &Device, "numerous", 800, 600);
        VulkanSwapchain Swapchain(&Instance, &Device, &WindowSurface);

        auto VertexShader   = std::make_shared<VulkanVertexShader>(&Device, "Test/Shaders/Camera/shader.vert");
        auto FragmentShader = std::make_shared<VulkanPixelShader>(&Device, "Test/Shaders/VertexInput/shader.frag");
        VulkanGraphicsShaderProgram TriangleProgram(&Device, VertexShader, FragmentShader);
        VulkanGraphicsPipelineState TrianglePipelineState(&TriangleProgram);
        VulkanRenderTargetLayout RTLayout(1);
        VulkanRenderPass RenderPass(&Device, &RTLayout);
        VulkanGraphicsPipeline Pipeline(&Device, &RenderPass, &TrianglePipelineState);
        std::shared_ptr<VulkanSemaphore> RenderFinished = std::make_shared<VulkanSemaphore>(&Device);
        std::shared_ptr<VulkanSemaphore> ImageAvailable = std::make_shared<VulkanSemaphore>(&Device);
        VulkanCommandBuffer CmdBuffer(&Device, Device.GetGraphicsQueue());
        VulkanPresenter Presenter(Device.GetPresentQueue(), &Swapchain);
        std::shared_ptr<Model> Triangle = std::make_shared<Model>("Test/Resources/Triangle/Triangle2D.obj");
        Sce::Camera Camera(glm::vec3(10, 10, 10), glm::vec3(0, 0, 0));


        World MyWorld;
        MyWorld.AddModel(Triangle, {0, 0, 0}, {0, 0, 0}, {1, 1, 1});


        while (!glfwWindowShouldClose(WindowSurface.GetWindowHandle()))
        {
            glfwPollEvents();

            std::vector<std::shared_ptr<VulkanImageView>> SwapchainImageViews;
            SwapchainImageViews.reserve(Swapchain.GetImageCount());
            std::vector<std::shared_ptr<VulkanFramebuffer>> Framebuffers;
            Framebuffers.reserve(Swapchain.GetImageCount());

            for (uint32_t i = 0; i < Swapchain.GetImageCount(); i++)
            {
                SwapchainImageViews.emplace_back(std::make_shared<VulkanImageView>(&Device, Swapchain.GetImage(i)));
                Framebuffers.emplace_back(std::make_shared<VulkanFramebuffer>(&Device, &RenderPass, std::vector<std::shared_ptr<VulkanImageView>>{SwapchainImageViews[i]}, WindowSurface.GetWidth(), WindowSurface.GetHeight()));
            }

            Swapchain.AcquireNextImage(ImageAvailable.get()->GetHandle(), VK_NULL_HANDLE);


            CmdBuffer.Wait();

            {
                CmdBuffer.Begin();
                CmdBuffer.BeginRenderPass(&RenderPass, Framebuffers[Swapchain.GetRenderIndex()]);


                vkCmdBindPipeline(CmdBuffer.GetHandle(), VK_PIPELINE_BIND_POINT_GRAPHICS, Pipeline.GetPipelineHandle());
                VkBuffer VertexBuffer               = Triangle->GetVertexBuffer(&MemManager);
                VkBuffer ColorBuffer                = Triangle->GetColorBuffer(&MemManager);
                std::vector<VkBuffer> VertexBuffers = {VertexBuffer, ColorBuffer};

                std::vector<VkDeviceSize> Offsets(2, 0);
                vkCmdBindVertexBuffers(CmdBuffer.GetHandle(), 0, static_cast<uint32_t>(VertexBuffers.size()), VertexBuffers.data(), Offsets.data());

                VkViewport Viewport = {
                    .x        = 0,
                    .y        = static_cast<float>(WindowSurface.GetHeight()),
                    .width    = static_cast<float>(WindowSurface.GetWidth()),
                    .height   = -static_cast<float>(WindowSurface.GetHeight()),
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

                CmdBuffer.EndRenderPass();

                Swapchain.CmdTransitImagePresentSrc(&CmdBuffer);

                CmdBuffer.End();
            }

            CmdBuffer.Submit({ImageAvailable.get()->GetHandle()}, {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT}, {RenderFinished});
            Presenter.Present({RenderFinished});
        }
    }
    catch (std::exception& e)
    {
        spdlog::critical(e.what());
    }

    return 0;
}