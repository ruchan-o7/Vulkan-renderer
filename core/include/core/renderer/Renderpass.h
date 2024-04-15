#pragma once
#include <vulkan/vulkan.h>
namespace vr {
class Renderpass {
 public:
  Renderpass() = default;
  void Init(VkFormat format);
  void CleanUp();
  VkFormat GetFormat() const { return format; }

 private:
  VkRenderPass renderPass;
  VkFormat format;
};
}  // namespace vr
