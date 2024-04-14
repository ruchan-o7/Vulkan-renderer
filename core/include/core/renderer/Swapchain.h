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
  List<VkImageView> imageViews;
  VkFormat format;
  VkExtent2D extent;
  void Cleanup(VkDevice device) {
    for (u32 i = 0; i < imageViews.size(); i++) {
      vkDestroyImageView(device, imageViews[i], nullptr);
    }
    vkDestroySwapchainKHR(device, swapchain, nullptr);
  }
};
}  // namespace vr
