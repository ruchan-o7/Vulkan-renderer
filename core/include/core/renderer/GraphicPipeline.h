#pragma once
#include <vulkan/vulkan.h>

#include "vulkan/vulkan_core.h"

namespace vr {

class GraphicPipeline {
 public:
  GraphicPipeline() = default;
  ~GraphicPipeline() = default;
  void Init(VkDescriptorSetLayout layout);
  void Cleanup();

 private:
  VkPipeline m_Pipeline;
  VkPipelineLayout m_Layout;
  VkViewport m_Viewport;
  VkExtent2D m_Extent;
  VkRect2D m_Scissor;
  VkDescriptorSetLayout m_DescriptorSetLayout;
};
}  // namespace vr
