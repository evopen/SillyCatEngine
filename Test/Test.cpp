#include "Engine/Filesystem/FileUtil.h"
#include "Engine/Vulkan/VulkanUtil.h"


#include <Engine/Vulkan/VulkanDevice.h>
#include <Engine/Vulkan/VulkanInstance.h>
#include <Engine/Vulkan/VulkanShader.h>
#include <Engine/Vulkan/VulkanSwapchain.h>
#include <Engine/Vulkan/VulkanWindowSurface.h>
#include <gtest/gtest.h>
#include <spdlog/spdlog.h>

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

int main()
{
    VulkanInstance instance;
    instance.Init();
    auto [physicalDevice, deviceProp] = VulkanDevice::SelectPhysicalDevice(instance);
    spdlog::info("select: {}", deviceProp.deviceName);
    VulkanDevice Device(&instance, physicalDevice, false);
    Device.Init();
    VulkanWindowSurface WindowSurface(&instance, &Device, "numerous", 800, 600);
    VulkanSwapchain Swapchain(&instance, &Device, &WindowSurface);

    VulkanShader VertexShader(&Device, "Test/Shaders/shader.vert");

    VkGraphicsPipelineCreateInfo PipelineInfo = {};
    PipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    return 0;
}