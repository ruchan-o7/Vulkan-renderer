#pragma once
#include <vulkan/vulkan.h>

namespace vr {
struct SwapChainSupportDetails {
  VkSurfaceCapabilitiesKHR capabilities;
  List<VkSurfaceFormatKHR> formats;
  List<VkPresentModeKHR> presentModes;
};
class Swapchain {
 public:
  void Init();
  void Cleanup();
  VkFormat GetSwapchainImageFormat() const { return format; }

 private:
  VkSwapchainKHR swapchain;
  List<VkImage> images;
  List<VkImageView> imageViews;
  VkFormat format;
  VkExtent2D extent;
};
}  // namespace vr
