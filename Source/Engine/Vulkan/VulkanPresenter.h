#pragma once

#include "Engine/pch.h"

#include "Engine/Platform/Platform.h"

class VulkanQueue;
class VulkanSwapchain;

class VulkanPresenter
{
public:
    API VulkanPresenter(VulkanQueue* InPresentQueue, VulkanSwapchain* InSwapchain);
    API void Present() const;

private:
    VulkanQueue* PresentQueue;
    VulkanSwapchain* Swapchain;
};
