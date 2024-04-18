#include "Renderer.h"

#include "renderer/VulkanContext.h"

namespace vr {
Renderer* Renderer::s_Instance     = nullptr;
VulkanContext* Renderer::s_Context = nullptr;
Renderer& Renderer::Get() {
  if (s_Instance == nullptr) {
    s_Instance = new Renderer();
  }
  return *s_Instance;
}
Renderer::Renderer() { s_Context = new VulkanContext(); }
VulkanContext& Renderer::GetContext() { return *s_Context; }

void Renderer::ContextOnEvent(Event& e) { s_Context->OnEvent(e); }
void Renderer::Init(GLFWwindow* window, u32 width, u32 height) {
  s_Context->initVulkan(window, width, height);
}
void Renderer::Shutdown() { s_Context->cleanup(); }
void Renderer::DrawTriangle() { s_Context->DrawTriangle(); }
void Renderer::BeginDraw() { s_Context->BeginDraw(); }
void Renderer::EndDraw() { s_Context->EndDraw(); }
void Renderer::Draw() {
  // context->drawFrame();
}
}  // namespace vr
