#include "Engine/pch.h"

#include "VulkanPresenter.h"
#include "VulkanQueue.h"
#include "VulkanSemaphore.h"
#include "VulkanSwapchain.h"

VulkanPresenter::VulkanPresenter(VulkanQueue* InPresentQueue, VulkanSwapchain* InSwapchain)
    : PresentQueue(InPresentQueue)
    , Swapchain(InSwapchain)
{
}

void VulkanPresenter::Present(std::vector<std::shared_ptr<VulkanSemaphore>> InWaitSemaphores) const
{
    std::vector<VkSwapchainKHR> Swapchains = {Swapchain->GetSwapchainHandle()};
    std::vector<uint32_t> Indexes          = {Swapchain->GetRenderIndex()};
    std::vector<VkSemaphore> WaitSemaphores(InWaitSemaphores.size());
    for (uint32_t i = 0; i < WaitSemaphores.size(); i++)
    {
        WaitSemaphores[i] = InWaitSemaphores[i]->GetHandle();
    }

    VkPresentInfoKHR PresentInfo = {
        .sType              = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
        .waitSemaphoreCount = static_cast<uint32_t>(WaitSemaphores.size()),
        .pWaitSemaphores    = WaitSemaphores.data(),
        .swapchainCount     = 1,
        .pSwapchains        = Swapchains.data(),
        .pImageIndices      = Indexes.data(),
    };
    spdlog::info("presenting image no.{}", Swapchain->GetRenderIndex());
    vkQueuePresentKHR(PresentQueue->GetHandle(), &PresentInfo);
}
