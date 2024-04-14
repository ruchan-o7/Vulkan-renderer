#pragma once
#include "Base.h"
#include "Renderer.h"
namespace vr {
class Application {
 public:
  Application();
  ~Application();
  void Run();

 private:
  GLFWwindow* m_WindowHandle;
  Shared<Renderer> m_Renderer;
};

}  // namespace vr
