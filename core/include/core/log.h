#pragma once
#include <spdlog/spdlog.h>

#include "pch.h"

namespace vr {
class Log {
 public:
  static void Init();
  static Shared<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }

 private:
  static Shared<spdlog::logger> s_CoreLogger;
};
#define CORE_TRACE(...) ::vr::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define CORE_INFO(...) ::vr::Log::GetCoreLogger()->info(__VA_ARGS__)
#define CORE_WARNING(...) ::vr::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define CORE_ERROR(...) ::vr::Log::GetCoreLogger()->error(__VA_ARGS__)
#define CORE_CRITICAL(...) ::vr::Log::GetCoreLogger()->critical(__VA_ARGS__)
}  // namespace vr
