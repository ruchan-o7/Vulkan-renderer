#include "Application.h"

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
  Renderer::Get().Init(m_WindowHandle);
}
Application::~Application() {
  Renderer::Get().Shutdown();
  glfwTerminate();
}
void Application::Run() {
  CORE_INFO("App running...");
  while (!glfwWindowShouldClose(m_WindowHandle)) {
    glfwPollEvents();
  }
  CORE_WARNING("App closing...");
}
}  // namespace vr
