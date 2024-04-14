#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

struct GLFWwindow;
namespace vr {
class Renderer {
 public:
  Renderer(GLFWwindow* window);
  ~Renderer();

 private:
  void Init();
  void CleanUp();
  VkInstance m_VkInstance;
};
}  // namespace vr
