#pragma once
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

#include "renderer/Device.h"
#include "renderer/GraphicPipeline.h"
#include "renderer/Renderpass.h"
#include "renderer/Swapchain.h"
#include "vulkan/vulkan_core.h"

namespace vr {
class VulkanContext {
 public:
  VulkanContext() = default;
  ~VulkanContext() = default;
  void buildContext(GLFWwindow* window);
  void Shutdown();
  VkSurfaceKHR& GetSurface() { return m_Surface; }
  Device& GetDevice() { return m_Device; }
  VkInstance& GetVk() { return instance; }
  Renderpass& GetRenderpass() { return m_Renderpass; }

  VkImage colorImage;
  VkDeviceMemory colorImageMem;
  VkImageView colorImageView;

 private:
  VkInstance instance;
  VkSurfaceKHR m_Surface;
  Swapchain m_Swapchain;
  Renderpass m_Renderpass;
  Device m_Device;
  VkDescriptorSetLayout m_DescriptorSetLayout;
  GraphicPipeline m_Pipeline;
  VkCommandPool m_CommandPool;
};

}  // namespace vr
