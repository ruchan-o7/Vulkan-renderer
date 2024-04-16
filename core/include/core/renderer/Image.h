#pragma once
#include <vulkan/vulkan.h>

#include "Base.h"
#include "vulkan/vulkan_core.h"

namespace vr {

class Image {
 public:
  Image(const VkImage& handle);
  Image() = default;
  VkImage handle;
};
class ImageView {
 public:
  ImageView(const Image& image, VkFormat format,
            VkImageAspectFlags aspectFlags);
  ImageView() = default;
  VkImageView handle;

 private:
  Image m_Image;
  VkFormat m_Format;
  VkImageAspectFlags m_AspectFlags;
};
}  // namespace vr
