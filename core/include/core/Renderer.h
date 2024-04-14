#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "renderer/Swapchain.h"


struct GLFWwindow;
namespace vr {
class Renderer {
 public:
  Renderer(GLFWwindow* window);
  ~Renderer() = default;
  void CleanUp();

 private:
  void Init();

 private:
  VkInstance m_VkInstance;
  VkSurfaceKHR m_Surface;
  VkPhysicalDevice m_Pdevice;
  VkDevice m_Device;
  Swapchain m_Swapchain;
};
}  // namespace vr
