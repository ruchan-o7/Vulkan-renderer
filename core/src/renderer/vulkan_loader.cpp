#include "renderer/vulkan_loader.h"

#include <GLFW/glfw3.h>

#include <cassert>
#include <glm/glm.hpp>

#include "Base.h"
#include "log.h"
#include "renderer/Swapchain.h"
#include "renderer/VulkanCheckResult.h"
#include "vulkan/vulkan_core.h"

namespace vr {
struct QueueuFamilyIndices {
  std::optional<u32> graphicsFamily;
  std::optional<u32> presentFamily;
  bool isComplete() {
    return graphicsFamily.has_value() && presentFamily.has_value();
  }
};
#define APPNAME "Renderer"
static VkDebugUtilsMessengerEXT m_DebugMesenger;
static List<const char *> getRequiredExtension() {
  u32 glfwExtensionsCount = 0;
  const char **glfwExts;
  glfwExts = glfwGetRequiredInstanceExtensions(&glfwExtensionsCount);
  List<const char *> extensions(glfwExts, glfwExts + glfwExtensionsCount);
#ifdef VR_DEBUG
  extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif
  return extensions;
}

const List<const char *> validationLayers = {"VK_LAYER_KHRONOS_validation"};
const List<const char *> deviceExtension = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

static bool checkValidationLayerSupport() {
  u32 layerCount = -1;
  vkEnumerateInstanceLayerProperties(&layerCount, NULL);
  List<VkLayerProperties> availableLayers(layerCount);
  vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

  for (const char *layerName : validationLayers) {
    bool layerFound = false;
    for (const auto &layerProps : availableLayers) {
      if (strcmp(layerName, layerProps.layerName) == 0) {
        layerFound = true;
        break;
      }
    }
    if (!layerFound) {
      return false;
    }
  }
  return true;
}
VKAPI_ATTR VkBool32 VKAPI_CALL
debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
              VkDebugUtilsMessageTypeFlagsEXT messageType,
              const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
              void *pUserData) {
  if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) {
    CORE_ERROR("{}", pCallbackData->pMessage);
  } else if (messageSeverity >=
             VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
    CORE_WARNING("{}", pCallbackData->pMessage);

  } else {
    CORE_INFO("{}", pCallbackData->pMessage);
  }
  (void)messageType;
  (void)pUserData;

  return VK_FALSE;
}
static VkResult createDebugUtilsMessengerExt(
    VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
    const VkAllocationCallbacks *pAllocator,
    VkDebugUtilsMessengerEXT *pDebugMessenger) {
  assert(instance);
  assert(pCreateInfo);
  assert(pDebugMessenger);
  auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
      instance, "vkCreateDebugUtilsMessengerEXT");
  if (func != nullptr) {
    return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
  } else {
    return VK_ERROR_EXTENSION_NOT_PRESENT;
  }
}
static void createDebugMessengerCreateInfo(
    VkDebugUtilsMessengerCreateInfoEXT &i) {
  i = {};
  i.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
  i.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                      VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                      VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
  i.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                  VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                  VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
  i.pfnUserCallback = debugCallback;
  i.pUserData = nullptr;
}
void createVulkanInstance(VkInstance *instance) {
  VkApplicationInfo appInfo{};
  appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  appInfo.pApplicationName = APPNAME;
  appInfo.applicationVersion = VK_MAKE_API_VERSION(0, 0, 1, 0);
  appInfo.pEngineName = "No Engine";
  appInfo.engineVersion = VK_MAKE_API_VERSION(0, 0, 1, 0);
  appInfo.apiVersion = VK_API_VERSION_1_3;

  VkInstanceCreateInfo createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  createInfo.pApplicationInfo = &appInfo;

  auto extension = getRequiredExtension();
  createInfo.enabledExtensionCount = static_cast<u32>(extension.size());
  createInfo.ppEnabledExtensionNames = extension.data();
  createInfo.enabledLayerCount = 0;
  createInfo.pNext = nullptr;

#ifdef VR_DEBUG
  VkDebugUtilsMessengerCreateInfoEXT debugCi{};
  if (!checkValidationLayerSupport()) {
    CORE_WARNING("Validation layers requested but not supported");
  }
  createInfo.enabledLayerCount = static_cast<u32>(validationLayers.size());
  createInfo.ppEnabledLayerNames = validationLayers.data();
  createDebugMessengerCreateInfo(debugCi);
  createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT *)&debugCi;

#endif
  VK_CALL(vkCreateInstance(&createInfo, nullptr, instance));
#ifdef VR_DEBUG

  VK_CALL(createDebugUtilsMessengerExt(*instance, &debugCi, nullptr,
                                       &m_DebugMesenger));

#endif
}
void createSurface(VkInstance &instance, GLFWwindow *window,
                   VkSurfaceKHR &surface) {
  auto res = glfwCreateWindowSurface(instance, window, nullptr, &surface);

  VK_CALL(res);
}

SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice &device,
                                              VkSurfaceKHR &surface) {
  SwapChainSupportDetails details{};
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
bool checkDeviceExtensionSupport(VkPhysicalDevice &device) {
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
static QueueuFamilyIndices findQueueFamilies(VkPhysicalDevice d,
                                             VkSurfaceKHR surface) {
  QueueuFamilyIndices indices;
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
bool isDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR &surface) {
  QueueuFamilyIndices indices = findQueueFamilies(device, surface);
  bool extensionSupported = checkDeviceExtensionSupport(device);
  bool swapChainAdequate = false;
  if (extensionSupported) {
    SwapChainSupportDetails swapChainDetails =
        querySwapChainSupport(device, surface);
    swapChainAdequate = !swapChainDetails.formats.empty() &&
                        !swapChainDetails.presentModes.empty();
  }
  return indices.isComplete() && extensionSupported && swapChainAdequate;
}
void pickPhysicalDevice(VkInstance &instance, VkPhysicalDevice &pDevice,
                        VkSurfaceKHR &surface) {
  u32 deviceCount = 0;
  vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
  if (deviceCount == 0) {
    CORE_ERROR("Failed to find GPUs with Vulkan support!");
  }
  List<VkPhysicalDevice> devices(deviceCount);
  vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());
  for (const auto &device : devices) {
    if (isDeviceSuitable(device, surface)) {
      pDevice = device;
    }
  }
  if (pDevice == VK_NULL_HANDLE) {
    CORE_ERROR("Failed to find GPUs with Vulkan support!");
  }
}
void createLogicalDevice(VkPhysicalDevice &physicalDevice, VkDevice &device,
                         VkSurfaceKHR surface) {
  QueueuFamilyIndices indices = findQueueFamilies(physicalDevice, surface);
  List<VkDeviceQueueCreateInfo> queueCreateInfos;
  std::set<u32> uniqueQueueFamilies = {indices.graphicsFamily.value(),
                                       indices.presentFamily.value()};
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

  VK_CALL(vkCreateDevice(physicalDevice, &ci, nullptr, &device));
}

static void destroyDebugUtilsMessengerEXT(
    VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger,
    const VkAllocationCallbacks *pAllocator) {
  auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
      instance, "vkDestroyDebugUtilsMessengerEXT");
  if (func != nullptr) {
    func(instance, debugMessenger, pAllocator);
  }
}
void DestroyDebugUtility(VkInstance instance) {
  destroyDebugUtilsMessengerEXT(instance, m_DebugMesenger, nullptr);
}

static SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice &device,
                                                     VkSurfaceKHR &surface) {
  SwapChainSupportDetails details{};
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

static inline VkSurfaceFormatKHR ChooseSwapSurfaceFormat(
    const List<VkSurfaceFormatKHR> &availableFormats) {
  for (const auto &aF : availableFormats) {
    if (aF.format == VK_FORMAT_B8G8R8A8_SRGB &&
        aF.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
      return aF;
    }
  }
  return availableFormats[0];
}
static inline VkPresentModeKHR ChooseSwapPresentMode(
    const List<VkPresentModeKHR> &availablePresentModes) {
  (void)availablePresentModes;
  return VK_PRESENT_MODE_FIFO_KHR;
}
static inline VkExtent2D ChooseSwapExtent(
    const VkSurfaceCapabilitiesKHR &capabilities, GLFWwindow *window) {
  if (capabilities.currentExtent.width != std::numeric_limits<u32>::max()) {
    return capabilities.currentExtent;
  } else {
    int w, h;
    glfwGetFramebufferSize(window, &w, &h);
    VkExtent2D actualExtend = {static_cast<uint32_t>(w),
                               static_cast<uint32_t>(h)};
    actualExtend.width =
        glm::clamp(actualExtend.width, capabilities.minImageExtent.width,
                   capabilities.maxImageExtent.width);

    actualExtend.height =
        glm::clamp(actualExtend.height, capabilities.minImageExtent.height,
                   capabilities.maxImageExtent.height);
    return actualExtend;
  }
}
void createSwapchain(VkPhysicalDevice &pDevice, VkDevice &device,
                     VkSurfaceKHR &surface, GLFWwindow *window,
                     Swapchain &swapchain) {
  SwapChainSupportDetails swapChainSupport =
      QuerySwapChainSupport(pDevice, surface);
  VkSurfaceFormatKHR sf = ChooseSwapSurfaceFormat(swapChainSupport.formats);
  VkPresentModeKHR pm = ChooseSwapPresentMode(swapChainSupport.presentModes);
  VkExtent2D extent = ChooseSwapExtent(swapChainSupport.capabilities, window);
  u32 imageCount = swapChainSupport.capabilities.minImageCount + 1;
  if (swapChainSupport.capabilities.maxImageCount > 0 &&
      imageCount > swapChainSupport.capabilities.maxImageCount) {
    imageCount = swapChainSupport.capabilities.maxImageCount;
  }
  VkSwapchainCreateInfoKHR createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
  createInfo.surface = surface;
  createInfo.minImageCount = imageCount;
  createInfo.imageFormat = sf.format;
  createInfo.imageColorSpace = sf.colorSpace;
  createInfo.imageExtent = extent;
  createInfo.imageArrayLayers = 1;
  createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
  QueueuFamilyIndices indices = findQueueFamilies(pDevice, surface);
  u32 queueFamilyIndicies[] = {indices.graphicsFamily.value(),
                               indices.presentFamily.value()};
  if (indices.graphicsFamily != indices.presentFamily) {
    createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
    createInfo.queueFamilyIndexCount = 2;
    createInfo.pQueueFamilyIndices = queueFamilyIndicies;
  } else {
    createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    createInfo.queueFamilyIndexCount = 0;
    createInfo.pQueueFamilyIndices = nullptr;
  }
  createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
  createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
  createInfo.presentMode = pm;
  createInfo.clipped = VK_TRUE;
  VK_CALL(
      vkCreateSwapchainKHR(device, &createInfo, nullptr, &swapchain.swapchain));

  VK_CALL(vkGetSwapchainImagesKHR(device, swapchain.swapchain, &imageCount,
                                  nullptr));
  swapchain.images.resize(imageCount);
  VK_CALL(vkGetSwapchainImagesKHR(device, swapchain.swapchain, &imageCount,
                                  swapchain.images.data()));
  swapchain.format = sf.format;
  swapchain.extent = extent;
  u32 swapchainImageCount = swapchain.images.size();
  swapchain.imageViews.resize(swapchainImageCount);
  for (u32 i = 0; i < swapchainImageCount; i++) {
    swapchain.imageViews[i] =
        createImageView(swapchain.images[i], swapchain.format,
                        VK_IMAGE_ASPECT_COLOR_BIT, 1, device);
  }
}
VkImageView createImageView(VkImage image, VkFormat format,
                            VkImageAspectFlags aspectFlags, u32 mipLevels,
                            VkDevice device) {
  VkImageViewCreateInfo viewInfo{};
  viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
  viewInfo.image = image;
  viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
  viewInfo.format = format;
  viewInfo.subresourceRange.aspectMask = aspectFlags;
  viewInfo.subresourceRange.baseMipLevel = 0;
  viewInfo.subresourceRange.levelCount = mipLevels;
  viewInfo.subresourceRange.baseArrayLayer = 0;
  viewInfo.subresourceRange.layerCount = 1;
  VkImageView imageView;
  VK_CALL(vkCreateImageView(device, &viewInfo, nullptr, &imageView));
  return imageView;
}
}  // namespace vr
