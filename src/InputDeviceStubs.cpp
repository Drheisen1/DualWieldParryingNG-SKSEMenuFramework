#include "PCH.h"

#include "RE/B/BSIInputDevice.h"
#include "RE/B/BSInputDevice.h"
#include "RE/B/BSKeyboardDevice.h"
#include "RE/B/BSWin32KeyboardDevice.h"

namespace RE
{
	BSIInputDevice::~BSIInputDevice() = default;
	BSInputDevice::~BSInputDevice() = default;
	BSKeyboardDevice::~BSKeyboardDevice() = default;
	BSWin32KeyboardDevice::~BSWin32KeyboardDevice() = default;

	bool BSInputDevice::GetButtonNameFromID([[maybe_unused]] std::int32_t a_id, [[maybe_unused]] BSFixedString& a_buttonName)
	{
		return false;
	}

	std::uint32_t BSInputDevice::GetMappingKey([[maybe_unused]] BSFixedString a_mapping)
	{
		return 0;
	}

	bool BSInputDevice::GetKeyCodeFromID([[maybe_unused]] std::int32_t a_id, [[maybe_unused]] std::uint32_t& a_keyCode)
	{
		return false;
	}

	bool BSInputDevice::IsEnabled() const
	{
		return true;
	}

	void BSWin32KeyboardDevice::Initialize() {}

	void BSWin32KeyboardDevice::Poll([[maybe_unused]] float a_timeDelta) {}

	void BSWin32KeyboardDevice::Shutdown() {}

	void BSWin32KeyboardDevice::ClearInputState() {}

	void BSWin32KeyboardDevice::Unk_09() {}

	void BSWin32KeyboardDevice::Unk_0A() {}
}
