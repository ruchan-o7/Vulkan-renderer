#include "renderer/Image.h"

#include "Renderer.h"
#include "renderer/VulkanCheckResult.h"

namespace vr {
Image::Image(const VkImage& image) : handle(image) {}
ImageView::ImageView(const Image& image, VkFormat format,
                     VkImageAspectFlags aspectFlags)
    : m_Image(image), m_Format(format), m_AspectFlags(aspectFlags) {
  VkImageViewCreateInfo createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
  createInfo.image = m_Image.handle;
  createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
  createInfo.format = m_Format;
  createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
  createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
  createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
  createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
  createInfo.subresourceRange.aspectMask = aspectFlags;
  createInfo.subresourceRange.baseMipLevel = 0;
  createInfo.subresourceRange.levelCount = 1;
  createInfo.subresourceRange.baseArrayLayer = 0;
  createInfo.subresourceRange.layerCount = 1;
  VK_CALL(vkCreateImageView(DEVICE, &createInfo, nullptr, &handle));
}
}  // namespace vr
