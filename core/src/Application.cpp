#include "Application.h"

#include <functional>

#include "Events/Event.h"
#include "Events/WindowResizeEvent.h"
#include "Renderer.h"
#include "log.h"

namespace vr {
Application* Application::s_Instance = nullptr;
Application& Application::Get() { return *s_Instance; }
Application::Application() {
  if (s_Instance != nullptr) {
    CORE_CRITICAL("CAN NOT CREATE TWO INSTANCE OF APPLICATION");
    return;
  }
  if (s_Instance == nullptr) {
    s_Instance = this;
  }
  eventCallback = BIND_EVENT(Application::OnEvent);
  Log::Init();  // TODO move to entrypoint
  if (!glfwInit()) {
    const char* err;
    glfwGetError(&err);
    CORE_CRITICAL("GLFW can not initialized");
    CORE_ERROR("\t %s", err);
  }
  CORE_TRACE("GLFW initalized");
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  m_WindowHandle = glfwCreateWindow(300, 300, "Renderer", nullptr, nullptr);
  glfwSetWindowUserPointer(m_WindowHandle, this);
  glfwSetFramebufferSizeCallback(m_WindowHandle, [](GLFWwindow* window,
                                                    int width, int height) {
    auto app = reinterpret_cast<Application*>(glfwGetWindowUserPointer(window));
    app->framebufferResized = true;
    WindowResizeEvent event{(u32)width, (u32)height};
    app->OnEvent(event);
    Renderer::Get().ContextOnEvent(event);
  });

  Renderer::Get().Init(m_WindowHandle, 300, 300);
}
void Application::OnEvent(Event& e) {
  EventDispatcher dispatcher{e};
  dispatcher.Dispatch<WindowResizeEvent>(
      BIND_EVENT(Application::OnWindowResize));
  CORE_INFO(e.ToString());
}

bool Application::OnWindowResize(WindowResizeEvent& e) {
  CORE_TRACE("Screen resized");
  return true;
}
Application::~Application() {
  Renderer::Get().Shutdown();
  glfwTerminate();
}
void Application::Run() {
  CORE_INFO("App running...");
  auto renderer = Renderer::Get();
  while (!glfwWindowShouldClose(m_WindowHandle)) {
    Renderer::BeginDraw();
    { Renderer::DrawTriangle(); }
    Renderer::EndDraw();
    // Renderer::Get().Draw();
    glfwPollEvents();
  }
  CORE_WARNING("App closing...");
}
}  // namespace vr
