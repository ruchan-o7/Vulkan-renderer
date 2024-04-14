#pragma once
#include <vulkan/vulkan.h>
namespace vr {

struct SwapChainSupportDetails {
  VkSurfaceCapabilitiesKHR capabilities;
  List<VkSurfaceFormatKHR> formats;
  List<VkPresentModeKHR> presentModes;
};
struct Swapchain {
  VkSwapchainKHR swapchain;
  List<VkImage> images;
  VkFormat format;
  VkExtent2D extent;
  void Cleanup(VkDevice device) {
    vkDestroySwapchainKHR(device, swapchain, nullptr);
  }
};
}  // namespace vr
