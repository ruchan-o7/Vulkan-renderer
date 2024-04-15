#pragma once
#include "Base.h"
#include "GLFW/glfw3.h"
namespace vr {
class Application {
 public:
  Application();
  static Application& Get();
  void Run();

 private:
  ~Application();
  static Application* s_Instance;
  GLFWwindow* m_WindowHandle;
};

}  // namespace vr
