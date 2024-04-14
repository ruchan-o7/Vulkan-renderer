#pragma once
#include "vulkan/vulkan_core.h"
#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"

namespace vr {
struct Swapchain;
void createVulkanInstance(VkInstance* instance);
void createSurface(VkInstance& instance, GLFWwindow* window,
                   VkSurfaceKHR& surface);
void pickPhysicalDevice(VkInstance& instance, VkPhysicalDevice& pDevice,
                        VkSurfaceKHR& surface);
void createLogicalDevice(VkPhysicalDevice& physicalDevice, VkDevice& device,
                         VkSurfaceKHR surface);
void createSwapchain();
void DestroyDebugUtility(VkInstance instance);
void createSwapchain(VkPhysicalDevice& pDevice, VkDevice& device,
                     VkSurfaceKHR& surface, GLFWwindow* window,
                     Swapchain& swapchain);
}  // namespace vr
