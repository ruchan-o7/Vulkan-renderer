#pragma once
#include "Base.h"
#include "renderer/Device.h"
#include "renderer/Renderpass.h"
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "renderer/Swapchain.h"

struct GLFWwindow;
namespace vr {
class Renderer {
 public:
  Renderer() = default;
  ~Renderer() = default;
  void Shutdown();
  static Renderer& Get() { return *s_Instance; }
  void Init(GLFWwindow* window);

  VkSurfaceKHR& GetSurface() { return m_Surface; }
  Device& GetDevice() { return m_Device; }
  VkInstance& GetVk() { return m_VkInstance; }

 private:
  static Renderer* s_Instance;

 private:
  VkInstance m_VkInstance;
  VkSurfaceKHR m_Surface;
  Swapchain m_Swapchain;
  Renderpass m_Renderpass;
  Device m_Device;
};
}  // namespace vr
