#pragma once
#include <vulkan/vulkan.h>

#include "vulkan/vulkan_core.h"

namespace vr {
class Renderpass {
 public:
  Renderpass() = default;
  void Init(VkFormat format);
  void CleanUp();
  VkFormat GetFormat() const { return format; }
  VkRenderPass GetRP() const { return renderPass; }

 private:
  VkRenderPass renderPass;
  VkFormat format;
};
}  // namespace vr
