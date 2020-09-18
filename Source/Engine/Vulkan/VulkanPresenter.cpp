#include "Engine/pch.h"

#include "VulkanPresenter.h"
#include "VulkanQueue.h"
#include "VulkanSwapchain.h"

VulkanPresenter::VulkanPresenter(VulkanQueue* InPresentQueue, VulkanSwapchain* InSwapchain)
    : PresentQueue(InPresentQueue)
    , Swapchain(InSwapchain)
{
}

void VulkanPresenter::Present() const
{
    std::vector<VkSwapchainKHR> Swapchains = {Swapchain->GetSwapchainHandle()};
    std::vector<uint32_t> Indexes          = {Swapchain->GetPresentIndex()};

    VkPresentInfoKHR PresentInfo = {
        .sType          = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
        .swapchainCount = 1,
        .pSwapchains    = Swapchains.data(),
        .pImageIndices  = Indexes.data(),
    };
    spdlog::debug("presenting image no.{}", Swapchain->GetPresentIndex());
    vkQueuePresentKHR(PresentQueue->GetHandle(), &PresentInfo);
}
