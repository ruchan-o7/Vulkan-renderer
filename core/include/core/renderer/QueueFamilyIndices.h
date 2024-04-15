#pragma once
namespace vr {
#include "pch.h"
struct QueueuFamilyIndices {
  std::optional<u32> graphicsFamily;
  std::optional<u32> presentFamily;
  bool isComplete() {
    return graphicsFamily.has_value() && presentFamily.has_value();
  }
};
}  // namespace vr
