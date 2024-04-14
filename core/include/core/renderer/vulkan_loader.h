#pragma once
#include "vulkan/vulkan_core.h"
#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"

namespace vr {
void createVulkanInstance(VkInstance* instance);
void createSurface(VkInstance& instance, GLFWwindow* window,
                   VkSurfaceKHR& surface);
void pickPhysicalDevice(VkInstance& instance, VkPhysicalDevice& pDevice,
                        VkSurfaceKHR& surface);
void createLogicalDevice(VkPhysicalDevice& physicalDevice, VkDevice& device,
                         VkSurfaceKHR surface);

void DestroyDebugUtility(VkInstance instance);
}  // namespace vr
