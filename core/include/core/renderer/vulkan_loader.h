#pragma once
#include "vulkan/vulkan_core.h"
#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"

namespace vr {
struct Swapchain;
void createVulkanInstance(VkInstance* instance);
void createSurface(VkInstance& instance, GLFWwindow* window,
                   VkSurfaceKHR& surface);
void pickPhysicalDevice(VkInstance& instance, VkPhysicalDevice& pDevice);
void createLogicalDevice(VkPhysicalDevice& physicalDevice, VkDevice& device,
                         VkSurfaceKHR surface);
void createSwapchain();
void DestroyDebugUtility(VkInstance instance);
void createSwapchain(VkPhysicalDevice& pDevice, VkDevice& device,
                     VkSurfaceKHR& surface, GLFWwindow* window,
                     Swapchain& swapchain);
VkImageView createImageView(VkImage image, VkFormat format,
                            VkImageAspectFlags aspectFlags, u32 mipLevels,
                            VkDevice device);
}  // namespace vr
