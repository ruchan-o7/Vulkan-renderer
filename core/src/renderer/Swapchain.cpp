#include "renderer/Swapchain.h"

#include <glm/glm.hpp>

#include "Application.h"
#include "Renderer.h"
#include "renderer/VulkanCheckResult.h"

namespace vr {

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

static inline VkExtent2D ChooseSwapExtent(
    const VkSurfaceCapabilitiesKHR &capabilities) {
  auto window = Application::Get().GetWindowHandle();
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
void Swapchain::Init() {
  auto device = Renderer::Get().GetDevice();
  auto indices = device.GetFI();
  auto swapChainSupport = device.GetQSP();
  VkSurfaceFormatKHR sf = ChooseSwapSurfaceFormat(swapChainSupport.formats);
  VkPresentModeKHR pm =
      VK_PRESENT_MODE_FIFO_KHR;  // ChooseSwapPresentMode(swapChainSupport.presentModes);
  VkExtent2D selectedExtent = ChooseSwapExtent(swapChainSupport.capabilities);
  u32 imageCount = swapChainSupport.capabilities.minImageCount + 1;
  if (swapChainSupport.capabilities.maxImageCount > 0 &&
      imageCount > swapChainSupport.capabilities.maxImageCount) {
    imageCount = swapChainSupport.capabilities.maxImageCount;
  }
  VkSwapchainCreateInfoKHR createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
  createInfo.surface = Renderer::Get().GetSurface();
  createInfo.minImageCount = imageCount;
  createInfo.imageFormat = sf.format;
  createInfo.imageColorSpace = sf.colorSpace;
  createInfo.imageExtent = selectedExtent;
  createInfo.imageArrayLayers = 1;
  createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
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
  VK_CALL(vkCreateSwapchainKHR(device.GetLogicalDevicel(), &createInfo, nullptr,
                               &swapchain));

  VK_CALL(vkGetSwapchainImagesKHR(device.GetLogicalDevicel(), swapchain,
                                  &imageCount, nullptr));
  images.resize(imageCount);
  VK_CALL(vkGetSwapchainImagesKHR(device.GetLogicalDevicel(), swapchain,
                                  &imageCount, images.data()));
  format = sf.format;
  this->extent = selectedExtent;
  imageViews.resize(imageCount);
  for (u32 i = 0; i < imageCount; i++) {
    imageViews[i] =
        createImageView(images[i], format, VK_IMAGE_ASPECT_COLOR_BIT, 1,
                        device.GetLogicalDevicel());
  }
}
void Swapchain::Cleanup() {
  auto device = Renderer::Get().GetDevice();
  for (u32 i = 0; i < imageViews.size(); i++) {
    vkDestroyImageView(device.GetLogicalDevicel(), imageViews[i], nullptr);
  }
  vkDestroySwapchainKHR(device.GetLogicalDevicel(), swapchain, nullptr);
}
}  // namespace vr
