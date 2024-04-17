#pragma once
#include "GLFW/glfw3.h"
namespace vr {
class Application {
 public:
  Application();
  ~Application();
  static Application& Get();
  void Run();
  GLFWwindow* GetWindowHandle() const { return m_WindowHandle; }

 private:
  static Application* s_Instance;
  GLFWwindow* m_WindowHandle;
  bool framebufferResized = false;
};

}  // namespace vr
