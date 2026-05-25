#pragma once

#include "RE/Skyrim.h"
#include "SKSE/SKSE.h"

#ifdef NDEBUG
#	include <spdlog/sinks/basic_file_sink.h>
#else
#	include <spdlog/sinks/msvc_sink.h>
#endif

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <ClibUtil/simpleINI.hpp>

#include <algorithm>
#include <array>
#include <atomic>
#include <cctype>
#include <cstdint>
#include <filesystem>
#include <format>
#include <limits>
#include <memory>
#include <numeric>
#include <optional>
#include <ranges>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

using namespace std::literals;

namespace logger = SKSE::log;

namespace util
{
	using SKSE::stl::report_and_fail;
}

#include "Plugin.h"

using uint = std::uint32_t;
