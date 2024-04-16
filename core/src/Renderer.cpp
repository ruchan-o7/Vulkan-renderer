#include "Renderer.h"

#include "Base.h"
#include "log.h"
#include "renderer/VulkanCheckResult.h"
#include "renderer/vulkan_loader.h"
#include "vulkan/vulkan_core.h"

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
  m_Renderpass.Init(m_Swapchain.GetSwapchainImageFormat());

  {
    VkDescriptorSetLayoutBinding uboLayoutBinding{};
    uboLayoutBinding.binding = 0;
    uboLayoutBinding.descriptorCount = 1;
    uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
    VkDescriptorSetLayoutBinding samplerLayoutBinding{};
    samplerLayoutBinding.binding = 1;
    samplerLayoutBinding.descriptorCount = 1;
    samplerLayoutBinding.descriptorType =
        VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    samplerLayoutBinding.pImmutableSamplers = nullptr;
    samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
    std::array<VkDescriptorSetLayoutBinding, 2> bindings = {
        uboLayoutBinding, samplerLayoutBinding};

    VkDescriptorSetLayoutCreateInfo layoutInfo{};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = static_cast<u32>(bindings.size());
    layoutInfo.pBindings = bindings.data();
    VK_CALL(vkCreateDescriptorSetLayout(DEVICE, &layoutInfo, nullptr,
                                        &m_DescriptorSetLayout));
  }
  m_Pipeline.Init(m_DescriptorSetLayout);
}

void Renderer::Shutdown() {
  vkDestroyDescriptorSetLayout(DEVICE, m_DescriptorSetLayout, nullptr);
  m_Pipeline.Cleanup();
  m_Renderpass.CleanUp();
  m_Swapchain.Cleanup();
  vkDestroyDevice(DEVICE, nullptr);
#ifdef VR_DEBUG
  DestroyDebugUtility(m_VkInstance);
#endif
  vkDestroySurfaceKHR(m_VkInstance, m_Surface, nullptr);
  vkDestroyInstance(m_VkInstance, nullptr);
}
}  // namespace vr
