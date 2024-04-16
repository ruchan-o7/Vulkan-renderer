#include "renderer/Graphicpipeline.h"

#include "Renderer.h"
#include "renderer/Shader.h"
#include "renderer/Vertex.h"
#include "renderer/VulkanCheckResult.h"
#include "vulkan/vulkan_core.h"

namespace vr {
void GraphicPipeline::Init(VkDescriptorSetLayout layout) {
  Shader vert("./shaders/vert.spv");
  Shader frag("./shaders/frag.spv");
  auto vertShaderModule = vert.GetModule();
  auto fragShaderModule = frag.GetModule();

  VkPipelineShaderStageCreateInfo verShaderStageInfo{};
  verShaderStageInfo.sType =
      VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;

  verShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
  verShaderStageInfo.module = vertShaderModule;
  verShaderStageInfo.pName = "main";  // entrypoint
  VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
  fragShaderStageInfo.sType =
      VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;

  fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
  fragShaderStageInfo.module = fragShaderModule;
  fragShaderStageInfo.pName = "main";  // entrypoint

  VkPipelineShaderStageCreateInfo shaderStages[] = {verShaderStageInfo,
                                                    fragShaderStageInfo};

  VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
  vertexInputInfo.sType =
      VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

  auto bindingDescription = Vertex::GetBindingDescription();
  auto attributeDescriptions = Vertex::GetAttributeDescriptions();

  vertexInputInfo.vertexBindingDescriptionCount = 1;
  vertexInputInfo.vertexAttributeDescriptionCount =
      static_cast<u32>(attributeDescriptions.size());
  vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
  vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

  List<VkDynamicState> dynamicStates = {VK_DYNAMIC_STATE_VIEWPORT,
                                        VK_DYNAMIC_STATE_SCISSOR};
  VkPipelineDynamicStateCreateInfo dynamicState{};
  dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
  dynamicState.dynamicStateCount = static_cast<u32>(dynamicStates.size());
  dynamicState.pDynamicStates = dynamicStates.data();

  VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
  inputAssembly.sType =
      VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
  inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
  inputAssembly.primitiveRestartEnable = VK_FALSE;

  m_Viewport.x = 0.0f;
  m_Viewport.y = 0.0f;
  m_Viewport.width = (float)m_Extent.width;
  m_Viewport.height = (float)m_Extent.height;
  m_Viewport.minDepth = 0.0f;
  m_Viewport.maxDepth = 1.0f;

  m_Scissor.offset = {0, 0};
  m_Scissor.extent = m_Extent;

  VkPipelineViewportStateCreateInfo viewportState{};
  viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
  viewportState.viewportCount = 1;
  viewportState.scissorCount = 1;
  viewportState.pViewports = &m_Viewport;
  viewportState.pScissors = &m_Scissor;

  VkPipelineRasterizationStateCreateInfo rasterizer{};
  rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
  rasterizer.depthBiasClamp = VK_FALSE;
  rasterizer.rasterizerDiscardEnable = VK_FALSE;
  rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
  rasterizer.lineWidth = 1.0f;
  rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
  rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
  rasterizer.depthBiasEnable = VK_FALSE;

  VkPipelineMultisampleStateCreateInfo multiSampling{};
  multiSampling.sType =
      VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
  multiSampling.rasterizationSamples = VK_SAMPLE_COUNT_2_BIT;
  multiSampling.pSampleMask = nullptr;
  multiSampling.sampleShadingEnable = VK_TRUE;
  multiSampling.minSampleShading = .2f;

  VkPipelineColorBlendAttachmentState colorBlendAttachment{};
  colorBlendAttachment.colorWriteMask =
      VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
      VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;

  colorBlendAttachment.blendEnable = VK_FALSE;

  VkPipelineColorBlendStateCreateInfo colorBlending{};
  colorBlending.sType =
      VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
  colorBlending.logicOpEnable = VK_FALSE;
  colorBlending.logicOp = VK_LOGIC_OP_COPY;  // Optional
  colorBlending.attachmentCount = 1;
  colorBlending.pAttachments = &colorBlendAttachment;

  VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
  pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
  pipelineLayoutInfo.setLayoutCount = 1;
  pipelineLayoutInfo.pSetLayouts = &layout;

  VK_CALL(
      vkCreatePipelineLayout(DEVICE, &pipelineLayoutInfo, nullptr, &m_Layout));
  VkPipelineDepthStencilStateCreateInfo depthStencil{};
  depthStencil.sType =
      VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
  depthStencil.depthTestEnable = VK_TRUE;
  depthStencil.depthWriteEnable = VK_TRUE;
  depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
  depthStencil.depthBoundsTestEnable = VK_FALSE;
  depthStencil.stencilTestEnable = VK_FALSE;
  VkGraphicsPipelineCreateInfo pipelineInfo{};
  pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
  pipelineInfo.stageCount = 2;
  pipelineInfo.pStages = shaderStages;
  pipelineInfo.pVertexInputState = &vertexInputInfo;
  pipelineInfo.pInputAssemblyState = &inputAssembly;
  pipelineInfo.pViewportState = &viewportState;
  pipelineInfo.pRasterizationState = &rasterizer;
  pipelineInfo.pMultisampleState = &multiSampling;
  pipelineInfo.pDepthStencilState = &depthStencil;
  pipelineInfo.pColorBlendState = &colorBlending;
  pipelineInfo.pDynamicState = &dynamicState;
  auto renderpass = Renderer::Get().GetRenderpass();
  pipelineInfo.layout = m_Layout;
  pipelineInfo.renderPass = renderpass.GetRP();
  pipelineInfo.subpass = 0;
  VK_CALL(vkCreateGraphicsPipelines(DEVICE, VK_NULL_HANDLE, 1, &pipelineInfo,
                                    nullptr, &m_Pipeline));
}
void GraphicPipeline::Cleanup() {
  vkDestroyPipelineLayout(DEVICE, m_Layout, nullptr);
  vkDestroyPipeline(DEVICE, m_Pipeline, nullptr);
}
}  // namespace vr
