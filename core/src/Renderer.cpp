#include "Renderer.h"

#include "log.h"
#include "renderer/vulkan_loader.h"
#include "vulkan/vulkan_core.h"

namespace vr {

Renderer::Renderer(GLFWwindow* window) {
  createVulkanInstance(&m_VkInstance);
  if (m_VkInstance == nullptr) {
    CORE_CRITICAL("Vulkan instance could not create!");
    return;
  }
  createSurface(m_VkInstance, window, m_Surface);
  pickPhysicalDevice(m_VkInstance, m_Pdevice, m_Surface);
  createLogicalDevice(m_Pdevice, m_Device, m_Surface);
}

void Renderer::CleanUp() {
  vkDestroyDevice(m_Device, nullptr);
#ifdef VR_DEBUG
  DestroyDebugUtility(m_VkInstance);
#endif
  vkDestroySurfaceKHR(m_VkInstance, m_Surface, nullptr);
  vkDestroyInstance(m_VkInstance, nullptr);
}
void Renderer::Init() {}
}  // namespace vr
