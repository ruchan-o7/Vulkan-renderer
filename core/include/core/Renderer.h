#pragma once
#include <GLFW/glfw3.h>

#include "renderer/Context.h"

namespace vr {
class Renderer {
 public:
  Renderer() = default;
  ~Renderer() = default;
  void Shutdown();
  static Renderer& Get();
  void Init(GLFWwindow* window);
  VulkanContext& GetContext() { return m_Context; }

 private:
  static Renderer* s_Instance;
  VulkanContext m_Context;
};
}  // namespace vr
