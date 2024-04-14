#include "Application.h"

#include <vulkan/vulkan.h>

#include "Base.h"
#include "GLFW/glfw3.h"
#include "log.h"

namespace vr {
Application::Application() {
  Log::Init();  // TODO move to entrypoint
  if (!glfwInit()) {
    CORE_CRITICAL("GLFW can not initialized");
    const char* err;
    glfwGetError(&err);
    CORE_ERROR("\t %s", err);
  }
  CORE_TRACE("GLFW initalized");
  m_WindowHandle = glfwCreateWindow(300, 300, "Renderer", nullptr, nullptr);

  m_Renderer = CreateShared<Renderer>(m_WindowHandle);
}
Application::~Application() { glfwTerminate(); }
void Application::Run() {
  CORE_INFO("App running...");
  CORE_WARNING("App closing...");
}
}  // namespace vr
