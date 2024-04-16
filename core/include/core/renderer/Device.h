#pragma once
#include <vulkan/vulkan.h>

#include "renderer/QueueFamilyIndices.h"
#include "renderer/Swapchain.h"
namespace vr {
class Device {
 public:
  VkDevice& GetLogicalDevice() { return m_Device; }
  VkPhysicalDevice& GetPhysicalDevice() { return m_Pdevice; }
  QueueuFamilyIndices GetFI() { return m_LastQueueFamilyIndices; }
  SwapChainSupportDetails GetQSP() { return m_QuerySwapchainSupport; }
  void Init();

 private:
  VkDevice m_Device;
  VkPhysicalDevice m_Pdevice;
  QueueuFamilyIndices m_LastQueueFamilyIndices;
  SwapChainSupportDetails m_QuerySwapchainSupport;

 private:
  bool checkDeviceExtensionSupport(VkPhysicalDevice& device);
  QueueuFamilyIndices findQueueFamilies(VkPhysicalDevice d);
  SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice& device);

  bool isDeviceSuitable(VkPhysicalDevice device);
};
}  // namespace vr
