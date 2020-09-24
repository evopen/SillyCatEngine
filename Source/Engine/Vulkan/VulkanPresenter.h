#pragma once

#include "Engine/pch.h"

#include "Engine/Platform/Platform.h"

class VulkanQueue;
class VulkanSwapchain;
class VulkanSemaphore;

class VulkanPresenter
{
public:
    API VulkanPresenter(VulkanQueue* InPresentQueue, VulkanSwapchain* InSwapchain);
    API void Present(std::vector<VkSemaphore> InWaitSemaphores) const;

private:
    VulkanQueue* PresentQueue;
    VulkanSwapchain* Swapchain;
};
