#include "renderer/context.h"

#include "Base.h"
#include "Renderer.h"
#include "log.h"
#include "renderer/Image.h"
#include "renderer/QueueFamilyIndices.h"
#include "renderer/VulkanCheckResult.h"
#include "renderer/vulkan_loader.h"
#include "vulkan/vulkan_core.h"

namespace vr {

static void createCommandPool(VkCommandPool* pool,
                              QueueuFamilyIndices indices) {
  VkCommandPoolCreateInfo poolInfo{};
  poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
  poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
  poolInfo.queueFamilyIndex = indices.graphicsFamily.value();
  if (vkCreateCommandPool(DEVICE, &poolInfo, nullptr, pool)) {
    throw std::runtime_error("failed to create command pool!");
  }
}

u32 findMemoryType(u32 typeFilter, VkMemoryPropertyFlags properties) {
  VkPhysicalDeviceMemoryProperties memProps{};
  vkGetPhysicalDeviceMemoryProperties(
      Renderer::Get().GetContext().GetDevice().GetPhysicalDevice(), &memProps);
  for (u32 i = 0; i < memProps.memoryTypeCount; i++) {
    if ((typeFilter & (i << 1)) &&
        (memProps.memoryTypes[i].propertyFlags & properties) == properties) {
      return i;
    }
  }
  throw std::runtime_error("Failed to find suitable memory tpye!");
}
void createImage(u32 width, u32 height, u32 mipLevels,
                 VkSampleCountFlagBits numSamples, VkFormat format,
                 VkImageTiling tiling, VkImageUsageFlags usage,
                 VkMemoryPropertyFlags properties, VkImage& image,
                 VkDeviceMemory& imageMemory) {
  VkImageCreateInfo imageInfo{};
  imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
  imageInfo.imageType = VK_IMAGE_TYPE_2D;
  imageInfo.extent.width = static_cast<u32>(width);
  imageInfo.extent.height = static_cast<u32>(height);
  imageInfo.extent.depth = 1;
  imageInfo.mipLevels = mipLevels;
  imageInfo.arrayLayers = 1;
  imageInfo.format = format;
  imageInfo.tiling = tiling;
  imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  imageInfo.usage = usage;
  imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
  imageInfo.samples = numSamples;
  VK_CALL(vkCreateImage(DEVICE, &imageInfo, nullptr, &image));
  VkMemoryRequirements memReqs;
  vkGetImageMemoryRequirements(DEVICE, image, &memReqs);
  VkMemoryAllocateInfo allocInfo{};
  allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
  allocInfo.allocationSize = memReqs.size;
  allocInfo.memoryTypeIndex =
      findMemoryType(memReqs.memoryTypeBits, properties);
  VK_CALL(vkAllocateMemory(DEVICE, &allocInfo, nullptr, &imageMemory));
  vkBindImageMemory(DEVICE, image, imageMemory, 0);
}

void createColorResources(VkFormat swapchainImageFormat, VkExtent2D extent,
                          VkImage& image, VkDeviceMemory& mem,
                          ImageView& imageView) {
  auto colorFormat = swapchainImageFormat;
  createImage(extent.width, extent.height, 1, VK_SAMPLE_COUNT_2_BIT,
              colorFormat, VK_IMAGE_TILING_OPTIMAL,
              VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT |
                  VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
              VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, image, mem);
  imageView = ImageView(image, colorFormat, VK_IMAGE_ASPECT_COLOR_BIT);
  // createImageView(image, colorFormat, VK_IMAGE_ASPECT_COLOR_BIT, 1);
}
void VulkanContext::buildContext(GLFWwindow* window) {
  createVulkanInstance(&instance);
  if (instance == nullptr) {
    CORE_CRITICAL("Vulkan instance could not create!");
    return;
  }
  createSurface(instance, window, m_Surface);

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

  createCommandPool(&m_CommandPool, m_Device.GetFI());
  // createColorResources(m_Swapchain.format, m_Swapchain.extent, colorImage,
  //                      colorImageMem, colorImageView);
}

void VulkanContext::Shutdown() {
  vkDestroyImageView(DEVICE, colorImageView, nullptr);
  vkDestroyImage(DEVICE, colorImage, nullptr);
  vkFreeMemory(DEVICE, colorImageMem, nullptr);
  vkDestroyDescriptorSetLayout(DEVICE, m_DescriptorSetLayout, nullptr);
  vkDestroyCommandPool(DEVICE, m_CommandPool, nullptr);
  m_Pipeline.Cleanup();
  m_Renderpass.CleanUp();
  m_Swapchain.Cleanup();
  vkDestroyDevice(DEVICE, nullptr);
#ifdef VR_DEBUG
  DestroyDebugUtility(instance);
#endif
  vkDestroySurfaceKHR(instance, m_Surface, nullptr);
  vkDestroyInstance(instance, nullptr);
}
}  // namespace vr
