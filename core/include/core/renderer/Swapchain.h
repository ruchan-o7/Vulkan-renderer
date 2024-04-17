#pragma once
#include <vulkan/vulkan.h>
namespace vr {
struct SwapChainSupportDetails {
  VkSurfaceCapabilitiesKHR capabilities;
  List<VkSurfaceFormatKHR> formats;
  List<VkPresentModeKHR> presentModes;
};
}  // namespace vr
