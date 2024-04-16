#include "Renderer.h"

#include "Base.h"
#include "log.h"
#include "renderer/vulkan_loader.h"

namespace vr {
Renderer* Renderer::s_Instance = nullptr;
Renderer& Renderer::Get() {
  if (s_Instance == nullptr) {
    s_Instance = new Renderer();
  }
  return *s_Instance;
}
void Renderer::Init(GLFWwindow* window) {
  createVulkanInstance(&m_VkInstance);
  if (m_VkInstance == nullptr) {
    CORE_CRITICAL("Vulkan instance could not create!");
    return;
  }
  createSurface(m_VkInstance, window, m_Surface);

  m_Device.Init();
  m_Swapchain.Init();
<<<<<<< HEAD
=======
  // createSwapchain(m_Pdevice, m_Device, m_Surface, window, m_Swapchain);
>>>>>>> 044fc8eb9e97da7d797c0014937ba7e240c3717e
  m_Renderpass.Init(m_Swapchain.GetSwapchainImageFormat());
}

void Renderer::Shutdown() {
  m_Renderpass.CleanUp();
  m_Swapchain.Cleanup();
  vkDestroyDevice(m_Device.GetLogicalDevicel(), nullptr);
#ifdef VR_DEBUG
  DestroyDebugUtility(m_VkInstance);
#endif
  vkDestroySurfaceKHR(m_VkInstance, m_Surface, nullptr);
  vkDestroyInstance(m_VkInstance, nullptr);
}
}  // namespace vr
