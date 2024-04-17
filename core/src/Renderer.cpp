#include "Renderer.h"

#include "renderer/VulkanCheckResult.h"
#include "renderer/VulkanContext.h"
#include "vulkan/vulkan_core.h"

namespace vr {
Renderer* Renderer::s_Instance = nullptr;
Renderer& Renderer::Get() {
  if (s_Instance == nullptr) {
    s_Instance = new Renderer();
  }
  return *s_Instance;
}
Renderer::Renderer() { context = new VulkanContext(); }
void Renderer::Init(GLFWwindow* window) { context->initVulkan(window); }
void Renderer::Shutdown() { context->cleanup(); }
void Renderer::Draw() {
  context->drawFrame();
  // u32 imageIndex = 0;
  // VK_CALL(vkAcquireNextImageKHR(context->GetDevice().GetLogicalDevice(),
  //                               context->swapchain.swapchainHandle,
  //                               UINT64_MAX, context->semaphore, nullptr,
  //                               &imageIndex));
  // VkPresentInfoKHR presentInfo{};
  // presentInfo.sType          = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
  // presentInfo.swapchainCount = 1;
  // presentInfo.pSwapchains    = &context->swapchain.swapchainHandle;
  // VK_CALL(vkQueuePresentKHR(context->graphicQueue, &presentInfo));
  // VK_CALL(vkDeviceWaitIdle(context->GetDevice().GetLogicalDevice()));
}
}  // namespace vr
