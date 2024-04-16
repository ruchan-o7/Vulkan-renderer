#pragma once
#include <GLFW/glfw3.h>

#include "renderer/Device.h"
#include "renderer/GraphicPipeline.h"
#include "renderer/Renderpass.h"
#include "renderer/Swapchain.h"

namespace vr {

class Renderer {
 public:
  Renderer() = default;
  ~Renderer() = default;
  void Shutdown();
  static Renderer& Get();
  void Init(GLFWwindow* window);

  VkSurfaceKHR& GetSurface() { return m_Surface; }
  Device& GetDevice() { return m_Device; }
  VkInstance& GetVk() { return m_VkInstance; }
  Renderpass& GetRenderpass() { return m_Renderpass; }

 private:
  static Renderer* s_Instance;

 private:
  VkInstance m_VkInstance;
  VkSurfaceKHR m_Surface;
  Swapchain m_Swapchain;
  Renderpass m_Renderpass;
  Device m_Device;
  VkDescriptorSetLayout m_DescriptorSetLayout;
  GraphicPipeline m_Pipeline;
};
}  // namespace vr
