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
  void Init(GLFWwindow* window, u32 width, u32 height);
  void Draw();

 public:
  VulkanContext* context;
  void ContextOnEvent(Event& e);

 private:
  static Renderer* s_Instance;
};
}  // namespace vr
