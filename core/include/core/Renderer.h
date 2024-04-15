#pragma once
#include "Base.h"
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

 private:
  static Renderer* s_Instance;

 private:
  VkInstance m_VkInstance;
  VkSurfaceKHR m_Surface;
  VkPhysicalDevice m_Pdevice;
  VkDevice m_Device;
  Swapchain m_Swapchain;
  Shared<Renderpass> m_Renderpass;
};
}  // namespace vr
