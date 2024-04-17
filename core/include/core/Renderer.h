#pragma once
#include <GLFW/glfw3.h>

#include "renderer/VulkanContext.h"

namespace vr {
class Renderer {
 public:
  Renderer();
  ~Renderer() = default;
  void Shutdown();
  static Renderer& Get();
  void Init(GLFWwindow* window);
  void Draw();

 public:
  VulkanContext* context;

 private:
  static Renderer* s_Instance;
};
}  // namespace vr
