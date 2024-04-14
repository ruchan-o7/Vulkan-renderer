#pragma once
#include <vulkan/vulkan.h>
namespace vr {
struct Renderpass {
  VkRenderPass renderPass;
  Renderpass(VkDevice& device, VkPhysicalDevice pDevice, VkFormat& format);
  void CleanUp();

 private:
  VkDevice device;
  VkFormat format;
};
}  // namespace vr
