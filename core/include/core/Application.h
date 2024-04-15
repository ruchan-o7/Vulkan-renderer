#pragma once
#include "Base.h"
#include "Renderer.h"
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
  Shared<Renderer> m_Renderer;
};

}  // namespace vr
