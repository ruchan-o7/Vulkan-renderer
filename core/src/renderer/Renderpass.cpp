#include "renderer/Renderpass.h"

#include "Renderer.h"
#include "log.h"
#include "renderer/VulkanCheckResult.h"
#include "vulkan/vulkan_core.h"

namespace vr {

VkFormat FindSupportedFormat(const List<VkFormat>& candidates,
                             VkImageTiling tiling,
                             VkFormatFeatureFlags features,
                             VkPhysicalDevice& pDevice) {
  for (VkFormat format : candidates) {
    VkFormatProperties props;
    vkGetPhysicalDeviceFormatProperties(pDevice, format, &props);
    for (VkFormat format : candidates) {
      VkFormatProperties props;
      vkGetPhysicalDeviceFormatProperties(pDevice, format, &props);

      if (tiling == VK_IMAGE_TILING_LINEAR &&
          (props.linearTilingFeatures & features) == features) {
        return format;
      } else if (tiling == VK_IMAGE_TILING_OPTIMAL &&
                 (props.optimalTilingFeatures & features) == features) {
        return format;
      }
    }
  }
  CORE_ERROR("failed to find supported format!");
  return VK_FORMAT_B8G8R8A8_SINT;
}
VkFormat findDepthFormat(VkPhysicalDevice& device) {
  return FindSupportedFormat(
      {VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT,
       VK_FORMAT_D24_UNORM_S8_UINT},
      VK_IMAGE_TILING_OPTIMAL, VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT,
      device);
}
void Renderpass::Init(VkFormat format) {
  this->format = format;
  auto pDevice = Renderer::Get().GetContext().GetDevice().GetPhysicalDevice();
  VkAttachmentDescription colorAttachment;
  // VkAttachmentDescription depthAttachment;
  // VkAttachmentDescription colorAttachmentResolve;
  colorAttachment.format = format;
  colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
  colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
  colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
  colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  colorAttachment.finalLayout =
      VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;  // VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

  // depthAttachment.format = findDepthFormat(pDevice);
  // depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
  // depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
  // depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  // depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  // depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  // depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  // depthAttachment.finalLayout =
  //     VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
  //
  // colorAttachmentResolve.format = format;
  // colorAttachmentResolve.samples = VK_SAMPLE_COUNT_1_BIT;
  // colorAttachmentResolve.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  // colorAttachmentResolve.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
  // colorAttachmentResolve.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  // colorAttachmentResolve.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  // colorAttachmentResolve.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  // colorAttachmentResolve.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
  VkAttachmentReference colorAttachmentRef{};
  colorAttachmentRef.attachment = 0;
  colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

  // VkAttachmentReference depthAttachmentRef{};
  // depthAttachmentRef.attachment = 1;
  // depthAttachmentRef.layout =
  // VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
  //
  // VkAttachmentReference colorAttachmentResolveRef{};
  // colorAttachmentResolveRef.attachment = 2;
  // colorAttachmentResolveRef.layout =
  // VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

  VkSubpassDescription subpass{};
  subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
  subpass.colorAttachmentCount = 1;
  subpass.pColorAttachments = &colorAttachmentRef;
  // subpass.pDepthStencilAttachment = &depthAttachmentRef;
  // subpass.pResolveAttachments = &colorAttachmentResolveRef;
  VkSubpassDependency dependency{};
  dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
  dependency.dstSubpass = 0;
  dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
  dependency.srcAccessMask = 0;
  dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
  dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

  // std::array<VkAttachmentDescription, 3> attachments = {
  //     colorAttachment, depthAttachment, colorAttachmentResolve};

  std::array<VkAttachmentDescription, 1> attachments = {colorAttachment};

  VkRenderPassCreateInfo renderPassInfo{};
  renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
  renderPassInfo.attachmentCount = static_cast<u32>(attachments.size());
  renderPassInfo.pAttachments = attachments.data();
  renderPassInfo.subpassCount = 1;
  renderPassInfo.pSubpasses = &subpass;
  renderPassInfo.dependencyCount = 1;
  renderPassInfo.pDependencies = &dependency;
  VK_CALL(vkCreateRenderPass(DEVICE, &renderPassInfo, nullptr, &renderPass));
}
void Renderpass::CleanUp() { vkDestroyRenderPass(DEVICE, renderPass, nullptr); }
}  // namespace vr
