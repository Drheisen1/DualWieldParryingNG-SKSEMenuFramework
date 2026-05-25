#pragma once

#include <RE/Skyrim.h>

#include <cstdint>
#include <optional>
#include <string>

namespace DualWieldParryingNG::InputCode
{
	inline constexpr std::uint32_t kDisabledKey = 1'000'000;
	inline constexpr std::uint32_t kNoModifier = 300;
	inline constexpr std::uint32_t kInvalidKey = static_cast<std::uint32_t>(-1);

	[[nodiscard]] std::optional<std::uint32_t> FromButtonEvent(const RE::ButtonEvent& a_event);
	[[nodiscard]] std::optional<std::uint32_t> FromString(std::string_view a_value);
	[[nodiscard]] bool IsHoldable(std::uint32_t a_code);
	[[nodiscard]] std::string ToString(std::uint32_t a_code, bool a_isModifier);
	[[nodiscard]] bool IsPressed(std::uint32_t a_code);
}
