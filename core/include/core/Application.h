#pragma once
#include <functional>

#include "Events/Event.h"
#include "Events/WindowResizeEvent.h"
#include "GLFW/glfw3.h"

namespace vr {
class Application {
 public:
  Application();
  ~Application();
  static Application& Get();
  void Run();
  GLFWwindow* GetWindowHandle() const { return m_WindowHandle; }

  using EventCallbackFn = std::function<void(Event&)>;
  EventCallbackFn eventCallback;
  void OnEvent(Event& e);

  bool OnWindowResize(WindowResizeEvent& e);

 private:
  static Application* s_Instance;
  GLFWwindow* m_WindowHandle;
  bool framebufferResized = false;
};

}  // namespace vr
