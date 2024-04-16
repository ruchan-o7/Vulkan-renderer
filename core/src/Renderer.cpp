#include "Renderer.h"

namespace vr {
Renderer* Renderer::s_Instance = nullptr;
Renderer& Renderer::Get() {
  if (s_Instance == nullptr) {
    s_Instance = new Renderer();
  }
  return *s_Instance;
}

void Renderer::Init(GLFWwindow* window) { m_Context.buildContext(window); }
void Renderer::Shutdown() { m_Context.Shutdown(); }
}  // namespace vr
