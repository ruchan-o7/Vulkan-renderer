#include "renderer/Device.h"

#include "Renderer.h"
#include "log.h"
#include "renderer/VulkanCheckResult.h"

namespace vr {

const List<const char *> validationLayers = {"VK_LAYER_KHRONOS_validation"};
const List<const char *> deviceExtension = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
bool Device::checkDeviceExtensionSupport(VkPhysicalDevice &device) {
  u32 extensionCount;
  vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount,
                                       nullptr);
  List<VkExtensionProperties> availableExtensions(extensionCount);
  vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount,
                                       availableExtensions.data());
  std::set<std::string> requiredExts(deviceExtension.begin(),
                                     deviceExtension.end());
  for (const auto &ext : availableExtensions) {
    requiredExts.erase(ext.extensionName);
  }
  return requiredExts.empty();
}
QueueuFamilyIndices Device::findQueueFamilies(VkPhysicalDevice d) {
  QueueuFamilyIndices indices;
  auto surface = Renderer::Get().GetSurface();
  u32 queueFamilyCount = 0;
  vkGetPhysicalDeviceQueueFamilyProperties(d, &queueFamilyCount, nullptr);
  List<VkQueueFamilyProperties> families(queueFamilyCount);
  vkGetPhysicalDeviceQueueFamilyProperties(d, &queueFamilyCount,
                                           families.data());
  VkBool32 presentSupport = false;
  int i = 0;
  for (const auto &qf : families) {
    vkGetPhysicalDeviceSurfaceSupportKHR(d, i, surface, &presentSupport);
    if (presentSupport) {
      indices.presentFamily = i;
    }
    if (qf.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
      indices.graphicsFamily = i;
    }
    if (indices.isComplete()) {
      break;
    }
    i++;
  }
  return indices;
}
SwapChainSupportDetails Device::querySwapChainSupport(
    VkPhysicalDevice &device) {
  SwapChainSupportDetails details{};
  auto surface = Renderer::Get().GetSurface();
  vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface,
                                            &details.capabilities);
  u32 formatCount;
  vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);
  if (formatCount != 0) {
    details.formats.resize(formatCount);
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount,
                                         details.formats.data());
  }
  u32 presentModeCount = 0;
  vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount,
                                            NULL);
  if (presentModeCount != 0) {
    details.presentModes.resize(presentModeCount);
    vkGetPhysicalDeviceSurfacePresentModesKHR(
        device, surface, &presentModeCount, details.presentModes.data());
  }
  return details;
}
bool Device::isDeviceSuitable(VkPhysicalDevice device) {
  m_LastQueueFamilyIndices = findQueueFamilies(device);
  bool extensionSupported = checkDeviceExtensionSupport(device);
  bool swapChainAdequate = false;
  if (extensionSupported) {
    SwapChainSupportDetails swapChainDetails = querySwapChainSupport(device);
    swapChainAdequate = !swapChainDetails.formats.empty() &&
                        !swapChainDetails.presentModes.empty();
  }
  return m_LastQueueFamilyIndices.isComplete() && extensionSupported &&
         swapChainAdequate;
}
void Device::Init() {
  VkInstance instance = Renderer::Get().GetVk();
  u32 deviceCount = 0;
  vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
  if (deviceCount == 0) {
    CORE_ERROR("Failed to find GPUs with Vulkan support!");
  }
  List<VkPhysicalDevice> devices(deviceCount);
  vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());
  for (const auto &device : devices) {
    if (isDeviceSuitable(device)) {
      m_Pdevice = device;
    }
  }
  if (m_Pdevice == VK_NULL_HANDLE) {
    CORE_ERROR("Failed to find GPUs with Vulkan support!");
  }

  List<VkDeviceQueueCreateInfo> queueCreateInfos;
  std::set<u32> uniqueQueueFamilies = {
      m_LastQueueFamilyIndices.graphicsFamily.value(),
      m_LastQueueFamilyIndices.presentFamily.value()};
  float queuePriority = 1.0f;
  for (u32 qF : uniqueQueueFamilies) {
    VkDeviceQueueCreateInfo qCf{};
    qCf.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    qCf.queueFamilyIndex = qF;
    qCf.queueCount = 1;
    qCf.pQueuePriorities = &queuePriority;
    queueCreateInfos.push_back(qCf);
  }
  VkPhysicalDeviceFeatures deviceFeatures{};
  deviceFeatures.samplerAnisotropy = VK_TRUE;
  deviceFeatures.sampleRateShading = VK_TRUE;
  VkDeviceCreateInfo ci{};
  ci.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
  ci.queueCreateInfoCount = static_cast<u32>(queueCreateInfos.size());
  ci.pQueueCreateInfos = queueCreateInfos.data();
  ci.pEnabledFeatures = &deviceFeatures;
  ci.enabledExtensionCount = static_cast<u32>(deviceExtension.size());
  ci.ppEnabledExtensionNames = deviceExtension.data();
  ci.enabledLayerCount = 0;
#ifdef VR_DEBUG
  ci.enabledLayerCount = static_cast<u32>(validationLayers.size());
  ci.ppEnabledLayerNames = validationLayers.data();
#endif

  VK_CALL(vkCreateDevice(m_Pdevice, &ci, nullptr, &m_Device));
}
}  // namespace vr
