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
  static VulkanContext& GetContext();
  void Init(GLFWwindow* window, u32 width, u32 height);
  static void Draw();
  static void EndDraw();
  static void BeginDraw();
  static void DrawTriangle();

 public:
  void ContextOnEvent(Event& e);

 private:
  static Renderer* s_Instance;
  static VulkanContext* s_Context;
};
}  // namespace vr
