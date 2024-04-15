#pragma once
#include <vulkan/vulkan.h>
namespace vr {
class Renderpass {
 public:
  Renderpass();
  void Init();
  void CleanUp();

 private:
  VkRenderPass renderPass;
  VkFormat format;
};
}  // namespace vr
