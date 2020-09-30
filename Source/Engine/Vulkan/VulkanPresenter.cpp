#include "Engine/pch.h"

#include "Engine/Logger/Logger.h"
#include "VulkanPresenter.h"
#include "VulkanQueue.h"
#include "VulkanSemaphore.h"
#include "VulkanSwapchain.h"


VulkanPresenter::VulkanPresenter(VulkanQueue* InPresentQueue, VulkanSwapchain* InSwapchain)
    : PresentQueue(InPresentQueue)
    , Swapchain(InSwapchain)
{
}

void VulkanPresenter::Present(std::vector<VkSemaphore> InWaitSemaphores) const
{
    std::vector<VkSwapchainKHR> Swapchains = {Swapchain->GetSwapchainHandle()};
    std::vector<uint32_t> Indexes          = {Swapchain->GetRenderIndex()};

    VkPresentInfoKHR PresentInfo = {
        .sType              = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
        .waitSemaphoreCount = static_cast<uint32_t>(InWaitSemaphores.size()),
        .pWaitSemaphores    = InWaitSemaphores.data(),
        .swapchainCount     = 1,
        .pSwapchains        = Swapchains.data(),
        .pImageIndices      = Indexes.data(),
    };
    Sce::Logger::Get()->debug("presenting image no.{}", Swapchain->GetRenderIndex());
    vkQueuePresentKHR(PresentQueue->GetHandle(), &PresentInfo);
}
