//
// Created by chk on 3/20/2024.
//

#ifndef CHK_PIX_CORE_LOG_HPP
#define CHK_PIX_CORE_LOG_HPP

#include <core/core.hpp>

#include <source_location>
#include <spdlog/spdlog.h>

namespace logging {
using source_location = std::source_location;

[[nodiscard]] constexpr auto
get_log_source_location(const source_location &location) {
  return spdlog::source_loc{location.file_name(),
                            static_cast<std::int32_t>(location.line()),
                            location.function_name()};
}
} // namespace logging

struct StringWithSourceLoc {
  StringView value;
  spdlog::source_loc loc;

  template <typename S> // NOLINTNEXTLINE
  [[maybe_unused]] StringWithSourceLoc(
      const S &s,
      const logging::source_location &loc = logging::source_location::current())
      : value{s}, loc{logging::get_log_source_location(loc)} {}
};

namespace Log {
template <typename... Args> void log(StringWithSourceLoc fmt, Args &&...args) {
  spdlog::default_logger_raw()->log(fmt.loc, spdlog::level::trace,
                                    spdlog::fmt_lib::runtime(fmt.value),
                                    std::forward<Args>(args)...);
}

template <typename... Args> void info(StringWithSourceLoc fmt, Args &&...args) {
  spdlog::default_logger_raw()->log(fmt.loc, spdlog::level::info,
                                    spdlog::fmt_lib::runtime(fmt.value),
                                    std::forward<Args>(args)...);
}

template <typename... Args> void warn(StringWithSourceLoc fmt, Args &&...args) {
  spdlog::default_logger_raw()->log(fmt.loc, spdlog::level::warn,
                                    spdlog::fmt_lib::runtime(fmt.value),
                                    std::forward<Args>(args)...);
}

template <typename... Args> void err(StringWithSourceLoc fmt, Args &&...args) {
  spdlog::default_logger_raw()->log(fmt.loc, spdlog::level::err,
                                    spdlog::fmt_lib::runtime(fmt.value),
                                    std::forward<Args>(args)...);
}

} // namespace Log

#endif // CHK_PIX_CORE_LOG_HPP
