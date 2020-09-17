#include "pch.h"

#include "Engine/Vulkan/VulkanFramebuffer.h"
#include "Engine/Vulkan/VulkanPipeline.h"
#include "Engine/Vulkan/VulkanPipelineLayout.h"
#include "Engine/Vulkan/VulkanPipelineState.h"
#include "Engine/Vulkan/VulkanRenderPass.h"
#include "Engine/Vulkan/VulkanRenderTargetLayout.h"


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
        VulkanFramebuffer(&Device, &Renderpass, 800, 600);
    }
    catch (std::exception& e)
    {
        spdlog::critical(e.what());
    }

    return 0;
}