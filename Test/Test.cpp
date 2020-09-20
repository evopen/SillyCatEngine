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

struct Vertex
{
    glm::vec2 pos;
    glm::vec3 color;
};

const std::vector<Vertex> vertices = {
    {{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
    {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
    {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}};

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
        std::shared_ptr<VulkanSemaphore> RenderFinished = std::make_shared<VulkanSemaphore>(&Device);
        std::shared_ptr<VulkanSemaphore> ImageAvailable = std::make_shared<VulkanSemaphore>(&Device);
        VulkanFence WaitFence(&Device);
        VulkanCommandBuffer CmdBuffer(&Device, Device.GetGraphicsQueue());
        VulkanPresenter Presenter(Device.GetPresentQueue(), &Swapchain);
        std::shared_ptr<Model> Triangle = std::make_shared<Model>("Test/Resources/Triangle/Triangle.obj");
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
                Framebuffers.emplace_back(std::make_shared<VulkanFramebuffer>(&Device, &Renderpass, std::vector<std::shared_ptr<VulkanImageView>>{SwapchainImageViews[i]}, WindowSurface.GetWidth(), WindowSurface.GetHeight()));
            }

            Swapchain.AcquireNextImage(ImageAvailable.get()->GetHandle(), VK_NULL_HANDLE);


            WaitFence.Wait();
            WaitFence.Reset();

            {
                CmdBuffer.Begin();
                CmdBuffer.BeginRenderPass(&Renderpass, Framebuffers[Swapchain.GetRenderIndex()]);


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

                CmdBuffer.EndRenderPass();

                Swapchain.CmdTransitImagePresentSrc(&CmdBuffer);

                CmdBuffer.End();
            }

            CmdBuffer.Submit({ImageAvailable.get()->GetHandle()}, {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT}, {RenderFinished}, &WaitFence);
            Presenter.Present({RenderFinished});
        }
    }
    catch (std::exception& e)
    {
        spdlog::critical(e.what());
    }

    return 0;
}