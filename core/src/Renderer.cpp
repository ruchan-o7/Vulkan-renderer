#include "Renderer.h"

#include "Base.h"
#include "GLFW/glfw3.h"
#include "log.h"
#include "renderer/vulkan_loader.h"
#include "vulkan/vulkan_core.h"

namespace vr {
Renderer* Renderer::s_Instance = nullptr;
void Renderer::Init(GLFWwindow* window) {
  createVulkanInstance(&m_VkInstance);
  if (m_VkInstance == nullptr) {
    CORE_CRITICAL("Vulkan instance could not create!");
    return;
  }
  createSurface(m_VkInstance, window, m_Surface);
  pickPhysicalDevice(m_VkInstance, m_Pdevice, m_Surface);
  createLogicalDevice(m_Pdevice, m_Device, m_Surface);
  createSwapchain(m_Pdevice, m_Device, m_Surface, window, m_Swapchain);
  m_Renderpass =
      CreateShared<Renderpass>(m_Device, m_Pdevice, m_Swapchain.format);
}

void Renderer::Shutdown() {
  m_Renderpass->CleanUp();
  m_Swapchain.Cleanup(m_Device);
  vkDestroyDevice(m_Device, nullptr);
#ifdef VR_DEBUG
  DestroyDebugUtility(m_VkInstance);
#endif
  vkDestroySurfaceKHR(m_VkInstance, m_Surface, nullptr);
  vkDestroyInstance(m_VkInstance, nullptr);
}
}  // namespace vr
