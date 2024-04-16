#pragma once
#include <vulkan/vulkan.h>

#include "pch.h"

namespace vr {
class Shader {
 public:
  Shader(const std::string& path);
  Shader();
  ~Shader();
  VkShaderModule GetModule() const { return m_Module; }
  void Load(const std::string& path);
  void Init();

 private:
  VkShaderModule m_Module;
  std::string m_Path;
  inline void Init_();
};
}  // namespace vr
