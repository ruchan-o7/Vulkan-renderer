#include "renderer/Shader.h"

#include "Renderer.h"
#include "file.h"
#include "log.h"
#include "renderer/VulkanCheckResult.h"
#include "vulkan/vulkan_core.h"

namespace vr {
void Shader::Init_() {
  auto code = vr::ReadFile(m_Path);
  if (code.empty()) {
    return;
  }
  VkShaderModuleCreateInfo createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
  createInfo.codeSize = code.size();
  createInfo.pCode = reinterpret_cast<const u32*>(code.data());
  VK_CALL(vkCreateShaderModule(DEVICE, &createInfo, nullptr, &m_Module));
}
Shader::Shader(const std::string& path)
    : m_Module(VK_NULL_HANDLE), m_Path(path) {
  Init_();
}
Shader::Shader() : m_Module(VK_NULL_HANDLE), m_Path("") {}
void Shader::Init() {
  if (m_Path == "" || m_Path.empty()) {
    CORE_ERROR("shader path is null or empty");
    return;
  }
  if (m_Module != VK_NULL_HANDLE) {
    CORE_WARNING("Shader module already created");
    return;
  }
  Init_();
}
void Shader::Load(const std::string& path) {
  m_Path = path;
  Init_();
}
Shader::~Shader() {
  if (m_Module != VK_NULL_HANDLE) {
    vkDestroyShaderModule(DEVICE, m_Module, nullptr);
  }
}
}  // namespace vr
