#pragma once
#include <vulkan/vulkan.h>

#include "Events/Event.h"
#include "Events/WindowResizeEvent.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "pch.h"

namespace vr {
void DestroyDebugUtilsMessengerEXT(VkInstance instance,
                                   VkDebugUtilsMessengerEXT debugMessenger,
                                   const VkAllocationCallbacks* pAllocator);
struct QueueFamilyIndices {
  std::optional<uint32_t> graphicsFamily;
  std::optional<uint32_t> presentFamily;

  bool isComplete() {
    return graphicsFamily.has_value() && presentFamily.has_value();
  }
};
struct SwapChainSupportDetails {
  VkSurfaceCapabilitiesKHR capabilities;
  std::vector<VkSurfaceFormatKHR> formats;
  std::vector<VkPresentModeKHR> presentModes;
};
class VulkanContext {
 public:
  VkInstance instance;
  VkDebugUtilsMessengerEXT debugMessenger;
  VkSurfaceKHR surface;
  VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
  VkDevice device;
  VkQueue graphicsQueue;
  VkQueue presentQueue;
  VkSwapchainKHR swapChain;
  List<VkImage> swapChainImages;
  VkFormat swapChainImageFormat;
  VkExtent2D swapChainExtent;
  List<VkImageView> swapChainImageViews;
  List<VkFramebuffer> swapChainFramebuffers;
  VkRenderPass renderPass;
  VkPipelineLayout pipelineLayout;
  VkPipeline graphicsPipeline;
  VkCommandPool commandPool;
  VkBuffer vertexBuffer;
  VkDeviceMemory vertexBufferMemory;
  VkBuffer indexBuffer;
  VkDeviceMemory indexBufferMemory;
  List<VkCommandBuffer> commandBuffers;
  List<VkSemaphore> imageAvailableSemaphores;
  List<VkSemaphore> renderFinishedSemaphores;
  List<VkFence> inFlightFences;
  uint32_t currentFrame   = 0;
  bool framebufferResized = false;
  void initVulkan(GLFWwindow* window, u32 width, u32 height);
  void cleanupSwapChain();
  void cleanup();
  void recreateSwapChain(int width, int height);
  void createInstance();
  void populateDebugMessengerCreateInfo(
      VkDebugUtilsMessengerCreateInfoEXT& createInfo);
  void setupDebugMessenger();
  void createSurface(GLFWwindow* window);
  void pickPhysicalDevice();
  void createLogicalDevice();
  void createSwapChain(u32 width, u32 height);
  void createImageViews();
  void createRenderPass();
  void createGraphicsPipeline();
  void createFramebuffers();
  void createCommandPool();
  void createVertexBuffer();
  void createIndexBuffer();
  void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage,
                    VkMemoryPropertyFlags properties, VkBuffer& buffer,
                    VkDeviceMemory& bufferMemory);
  void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
  uint32_t findMemoryType(uint32_t typeFilter,
                          VkMemoryPropertyFlags properties);
  void createCommandBuffers();
  void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);
  void createSyncObjects();
  void drawFrame();
  VkShaderModule createShaderModule(const List<char>& code);
  VkSurfaceFormatKHR chooseSwapSurfaceFormat(
      const List<VkSurfaceFormatKHR>& availableFormats);
  VkPresentModeKHR chooseSwapPresentMode(
      const List<VkPresentModeKHR>& availablePresentModes);
  VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities,
                              u32 width, u32 height);
  bool isDeviceSuitable(VkPhysicalDevice device);
  bool checkDeviceExtensionSupport(VkPhysicalDevice device);
  QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
  List<const char*> getRequiredExtensions();
  bool checkValidationLayerSupport();

  using EventCallbackFn = std::function<void(Event&)>;
  EventCallbackFn eventCallback;
  void OnEvent(Event& e);
  bool OnWindowResize(WindowResizeEvent& e);

};  // namespace vr

}  // namespace vr
