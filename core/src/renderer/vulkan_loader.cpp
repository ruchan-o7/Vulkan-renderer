#include "renderer/vulkan_loader.h"

#include <GLFW/glfw3.h>

#include <cassert>
#include <glm/glm.hpp>

#include "Base.h"
#include "log.h"
#include "renderer/VulkanCheckResult.h"
#include "vulkan/vulkan_core.h"

namespace vr {
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
}  // namespace vr
