#include "log.h"

#include "Base.h"
#include "spdlog/common.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/spdlog.h"

namespace vr {
Shared<spdlog::logger> Log::s_CoreLogger;
void Log::Init() {
  List<spdlog::sink_ptr> logSinks;
  logSinks.emplace_back(CreateShared<spdlog::sinks::stdout_color_sink_mt>());
  logSinks[0]->set_pattern("%^[%T] %n: %v%$");
  s_CoreLogger = CreateShared<spdlog::logger>("Vulkan Renderer",
                                              begin(logSinks), end(logSinks));
  spdlog::register_logger(s_CoreLogger);
  s_CoreLogger->set_level(spdlog::level::trace);
  s_CoreLogger->flush_on(spdlog::level::trace);
}

}  // namespace vr
