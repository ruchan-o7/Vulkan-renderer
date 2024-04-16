#pragma once
#include <vulkan/vulkan.h>

#include "renderer/Image.h"

namespace vr {

struct SwapChainSupportDetails {
  VkSurfaceCapabilitiesKHR capabilities;
  List<VkSurfaceFormatKHR> formats;
  List<VkPresentModeKHR> presentModes;
};

class Swapchain {
 public:
  void CreateImageViews();
  void Init();
  void Cleanup();
  VkFormat GetSwapchainImageFormat() const { return format; }

  VkSwapchainKHR swapchain;
  List<Image> images;
  List<ImageView> imageViews;
  VkFormat format;
  VkExtent2D extent;
};
}  // namespace vr
