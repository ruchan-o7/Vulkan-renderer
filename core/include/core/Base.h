#pragma once
#include <array>
#include <cstddef>
#include <memory>
#include <vector>
namespace vr {
#define u32 uint32_t
#define i32 int32_t
template <typename T>
using List = std::vector<T>;
template <typename T, size_t S>
using Array = std::array<T, S>;

template <typename T>
using Shared = std::shared_ptr<T>;
template <typename T, typename... Args>
constexpr Shared<T> CreateShared(Args&&... args) {
  return std::make_shared<T>(std::forward<Args>(args)...);
}
template <typename T>
using Unique = std::unique_ptr<T>;
template <typename T, typename... Args>
constexpr Unique<T> CreateUnique(Args&&... args) {
  return std::make_unique<T>(std::forward<Args>(args)...);
}
}  // namespace vr
