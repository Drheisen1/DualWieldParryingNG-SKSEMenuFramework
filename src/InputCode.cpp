#include "PCH.h"

#include "InputCode.h"

namespace DualWieldParryingNG::InputCode
{
	namespace
	{
		struct KeyAlias
		{
			std::string_view name;
			std::uint32_t code;
		};

		constexpr std::array kAliases{
			KeyAlias{ "esc", 1 },
			KeyAlias{ "1", 2 },
			KeyAlias{ "2", 3 },
			KeyAlias{ "3", 4 },
			KeyAlias{ "4", 5 },
			KeyAlias{ "5", 6 },
			KeyAlias{ "6", 7 },
			KeyAlias{ "7", 8 },
			KeyAlias{ "8", 9 },
			KeyAlias{ "9", 10 },
			KeyAlias{ "0", 11 },
			KeyAlias{ "-", 12 },
			KeyAlias{ "=", 13 },
			KeyAlias{ "backspace", 14 },
			KeyAlias{ "tab", 15 },
			KeyAlias{ "q", 16 },
			KeyAlias{ "w", 17 },
			KeyAlias{ "e", 18 },
			KeyAlias{ "r", 19 },
			KeyAlias{ "t", 20 },
			KeyAlias{ "y", 21 },
			KeyAlias{ "u", 22 },
			KeyAlias{ "i", 23 },
			KeyAlias{ "o", 24 },
			KeyAlias{ "p", 25 },
			KeyAlias{ "[", 26 },
			KeyAlias{ "]", 27 },
			KeyAlias{ "enter", 28 },
			KeyAlias{ "ctrl", 29 },
			KeyAlias{ "control", 29 },
			KeyAlias{ "a", 30 },
			KeyAlias{ "s", 31 },
			KeyAlias{ "d", 32 },
			KeyAlias{ "f", 33 },
			KeyAlias{ "g", 34 },
			KeyAlias{ "h", 35 },
			KeyAlias{ "j", 36 },
			KeyAlias{ "k", 37 },
			KeyAlias{ "l", 38 },
			KeyAlias{ ";", 39 },
			KeyAlias{ "'", 40 },
			KeyAlias{ "`", 41 },
			KeyAlias{ "shift", 42 },
			KeyAlias{ "\\", 43 },
			KeyAlias{ "z", 44 },
			KeyAlias{ "x", 45 },
			KeyAlias{ "c", 46 },
			KeyAlias{ "v", 47 },
			KeyAlias{ "b", 48 },
			KeyAlias{ "n", 49 },
			KeyAlias{ "m", 50 },
			KeyAlias{ ",", 51 },
			KeyAlias{ ".", 52 },
			KeyAlias{ "/", 53 },
			KeyAlias{ "rshift", 54 },
			KeyAlias{ "rightshift", 54 },
			KeyAlias{ "num*", 55 },
			KeyAlias{ "alt", 56 },
			KeyAlias{ "space", 57 },
			KeyAlias{ "capslock", 58 },
			KeyAlias{ "f1", 59 },
			KeyAlias{ "f2", 60 },
			KeyAlias{ "f3", 61 },
			KeyAlias{ "f4", 62 },
			KeyAlias{ "f5", 63 },
			KeyAlias{ "f6", 64 },
			KeyAlias{ "f7", 65 },
			KeyAlias{ "f8", 66 },
			KeyAlias{ "f9", 67 },
			KeyAlias{ "f10", 68 },
			KeyAlias{ "numlock", 69 },
			KeyAlias{ "scrolllock", 70 },
			KeyAlias{ "num7", 71 },
			KeyAlias{ "num8", 72 },
			KeyAlias{ "num9", 73 },
			KeyAlias{ "num-", 74 },
			KeyAlias{ "num4", 75 },
			KeyAlias{ "num5", 76 },
			KeyAlias{ "num6", 77 },
			KeyAlias{ "num+", 78 },
			KeyAlias{ "numplus", 78 },
			KeyAlias{ "num1", 79 },
			KeyAlias{ "num2", 80 },
			KeyAlias{ "num3", 81 },
			KeyAlias{ "num0", 82 },
			KeyAlias{ "numdel", 83 },
			KeyAlias{ "sysreq", 84 },
			KeyAlias{ "f11", 87 },
			KeyAlias{ "f12", 88 },
			KeyAlias{ "f13", 124 },
			KeyAlias{ "f14", 125 },
			KeyAlias{ "f15", 126 },
			KeyAlias{ "f16", 127 },
			KeyAlias{ "f17", 128 },
			KeyAlias{ "f18", 129 },
			KeyAlias{ "f19", 130 },
			KeyAlias{ "f20", 131 },
			KeyAlias{ "f21", 132 },
			KeyAlias{ "f22", 133 },
			KeyAlias{ "f23", 134 },
			KeyAlias{ "f24", 135 },
			KeyAlias{ "numenter", 156 },
			KeyAlias{ "rctrl", 157 },
			KeyAlias{ "rightctrl", 157 },
			KeyAlias{ "rightcontrol", 157 },
			KeyAlias{ "rcontrol", 157 },
			KeyAlias{ "num/", 181 },
			KeyAlias{ "printscreen", 183 },
			KeyAlias{ "printscrn", 183 },
			KeyAlias{ "ralt", 184 },
			KeyAlias{ "rightalt", 184 },
			KeyAlias{ "home", 199 },
			KeyAlias{ "up", 200 },
			KeyAlias{ "pageup", 201 },
			KeyAlias{ "left", 203 },
			KeyAlias{ "right", 205 },
			KeyAlias{ "end", 207 },
			KeyAlias{ "down", 208 },
			KeyAlias{ "pagedown", 209 },
			KeyAlias{ "insert", 210 },
			KeyAlias{ "ins", 210 },
			KeyAlias{ "del", 211 },
			KeyAlias{ "delete", 211 },
			KeyAlias{ "leftmousebutton", 256 },
			KeyAlias{ "lmb", 256 },
			KeyAlias{ "rightmousebutton", 257 },
			KeyAlias{ "rmb", 257 },
			KeyAlias{ "middlemousebutton", 258 },
			KeyAlias{ "mmb", 258 },
			KeyAlias{ "mouse3", 259 },
			KeyAlias{ "mouse4", 260 },
			KeyAlias{ "mouse5", 261 },
			KeyAlias{ "mouse6", 262 },
			KeyAlias{ "mouse7", 263 },
			KeyAlias{ "mousewheelup", 264 },
			KeyAlias{ "scrollwheelup", 264 },
			KeyAlias{ "mousewheeldown", 265 },
			KeyAlias{ "scrollwheeldown", 265 },
			KeyAlias{ "dpadup", 266 },
			KeyAlias{ "dpaddown", 267 },
			KeyAlias{ "dpadleft", 268 },
			KeyAlias{ "dpadright", 269 },
			KeyAlias{ "start", 270 },
			KeyAlias{ "back", 271 },
			KeyAlias{ "lthumb", 272 },
			KeyAlias{ "rthumb", 273 },
			KeyAlias{ "lshoulder", 274 },
			KeyAlias{ "rshoulder", 275 },
			KeyAlias{ "lbumper", 274 },
			KeyAlias{ "rbumper", 275 },
			KeyAlias{ "gamepada", 276 },
			KeyAlias{ "gamepadb", 277 },
			KeyAlias{ "gamepadx", 278 },
			KeyAlias{ "gamepady", 279 },
			KeyAlias{ "ltrigger", 280 },
			KeyAlias{ "rtrigger", 281 }
		};

		[[nodiscard]] std::optional<std::uint32_t> FromGamepadKey(std::uint32_t a_keyCode)
		{
			switch (static_cast<RE::BSWin32GamepadDevice::Key>(a_keyCode)) {
			case RE::BSWin32GamepadDevice::Key::kUp:
				return 266;
			case RE::BSWin32GamepadDevice::Key::kDown:
				return 267;
			case RE::BSWin32GamepadDevice::Key::kLeft:
				return 268;
			case RE::BSWin32GamepadDevice::Key::kRight:
				return 269;
			case RE::BSWin32GamepadDevice::Key::kStart:
				return 270;
			case RE::BSWin32GamepadDevice::Key::kBack:
				return 271;
			case RE::BSWin32GamepadDevice::Key::kLeftThumb:
				return 272;
			case RE::BSWin32GamepadDevice::Key::kRightThumb:
				return 273;
			case RE::BSWin32GamepadDevice::Key::kLeftShoulder:
				return 274;
			case RE::BSWin32GamepadDevice::Key::kRightShoulder:
				return 275;
			case RE::BSWin32GamepadDevice::Key::kA:
				return 276;
			case RE::BSWin32GamepadDevice::Key::kB:
				return 277;
			case RE::BSWin32GamepadDevice::Key::kX:
				return 278;
			case RE::BSWin32GamepadDevice::Key::kY:
				return 279;
			case RE::BSWin32GamepadDevice::Key::kLeftTrigger:
				return 280;
			case RE::BSWin32GamepadDevice::Key::kRightTrigger:
				return 281;
			default:
				return std::nullopt;
			}
		}

		[[nodiscard]] std::optional<std::uint32_t> ToGamepadKey(std::uint32_t a_code)
		{
			switch (a_code) {
			case 266:
				return RE::BSWin32GamepadDevice::Key::kUp;
			case 267:
				return RE::BSWin32GamepadDevice::Key::kDown;
			case 268:
				return RE::BSWin32GamepadDevice::Key::kLeft;
			case 269:
				return RE::BSWin32GamepadDevice::Key::kRight;
			case 270:
				return RE::BSWin32GamepadDevice::Key::kStart;
			case 271:
				return RE::BSWin32GamepadDevice::Key::kBack;
			case 272:
				return RE::BSWin32GamepadDevice::Key::kLeftThumb;
			case 273:
				return RE::BSWin32GamepadDevice::Key::kRightThumb;
			case 274:
				return RE::BSWin32GamepadDevice::Key::kLeftShoulder;
			case 275:
				return RE::BSWin32GamepadDevice::Key::kRightShoulder;
			case 276:
				return RE::BSWin32GamepadDevice::Key::kA;
			case 277:
				return RE::BSWin32GamepadDevice::Key::kB;
			case 278:
				return RE::BSWin32GamepadDevice::Key::kX;
			case 279:
				return RE::BSWin32GamepadDevice::Key::kY;
			case 280:
				return RE::BSWin32GamepadDevice::Key::kLeftTrigger;
			case 281:
				return RE::BSWin32GamepadDevice::Key::kRightTrigger;
			default:
				return std::nullopt;
			}
		}

		[[nodiscard]] std::optional<std::uint32_t> FromMouseKey(std::uint32_t a_keyCode)
		{
			if (a_keyCode > RE::BSWin32MouseDevice::Key::kWheelDown) {
				return std::nullopt;
			}

			return a_keyCode + 256;
		}

		[[nodiscard]] std::optional<std::uint32_t> ToMouseKey(std::uint32_t a_code)
		{
			if (a_code >= 256 && a_code <= 265) {
				return a_code - 256;
			}

			return std::nullopt;
		}
	}

	std::optional<std::uint32_t> FromButtonEvent(const RE::ButtonEvent& a_event)
	{
		const auto keyCode = a_event.GetIDCode();

		switch (a_event.device.get()) {
		case RE::INPUT_DEVICE::kKeyboard:
			return keyCode;
		case RE::INPUT_DEVICE::kMouse:
			return FromMouseKey(keyCode);
		case RE::INPUT_DEVICE::kGamepad:
			return FromGamepadKey(keyCode);
		default:
			return std::nullopt;
		}
	}

	std::optional<std::uint32_t> FromString(std::string_view a_value)
	{
		const auto first = std::ranges::find_if_not(a_value, [](unsigned char ch) {
			return std::isspace(ch) != 0;
		});
		const auto last = std::find_if_not(a_value.rbegin(), a_value.rend(), [](unsigned char ch) {
			return std::isspace(ch) != 0;
		}).base();

		if (first >= last) {
			return std::nullopt;
		}

		std::string normalized(first, last);
		std::ranges::transform(normalized, normalized.begin(), [](unsigned char ch) {
			return static_cast<char>(std::tolower(ch));
		});

		try {
			std::size_t parsed{};
			const auto code = std::stoul(normalized, std::addressof(parsed), 10);
			if (parsed == normalized.size() && code <= (std::numeric_limits<std::uint32_t>::max)()) {
				return static_cast<std::uint32_t>(code);
			}
		} catch (const std::exception&) {
		}

		for (const auto& alias : kAliases) {
			if (normalized == alias.name) {
				return alias.code;
			}
		}

		return std::nullopt;
	}

	bool IsHoldable(std::uint32_t a_code)
	{
		return a_code != 264 && a_code != 265;
	}

	std::string ToString(std::uint32_t a_code, bool a_isModifier)
	{
		if (!a_isModifier && a_code >= kDisabledKey) {
			return "Disabled";
		}

		if (a_isModifier && a_code >= kNoModifier) {
			return "None";
		}

		switch (a_code) {
		case 1:
			return "Keyboard Escape";
		case 2:
			return "Keyboard 1";
		case 3:
			return "Keyboard 2";
		case 4:
			return "Keyboard 3";
		case 5:
			return "Keyboard 4";
		case 6:
			return "Keyboard 5";
		case 7:
			return "Keyboard 6";
		case 8:
			return "Keyboard 7";
		case 9:
			return "Keyboard 8";
		case 10:
			return "Keyboard 9";
		case 11:
			return "Keyboard 0";
		case 12:
			return "Keyboard -";
		case 13:
			return "Keyboard =";
		case 14:
			return "Keyboard Backspace";
		case 15:
			return "Keyboard Tab";
		case 16:
			return "Keyboard Q";
		case 17:
			return "Keyboard W";
		case 18:
			return "Keyboard E";
		case 19:
			return "Keyboard R";
		case 20:
			return "Keyboard T";
		case 21:
			return "Keyboard Y";
		case 22:
			return "Keyboard U";
		case 23:
			return "Keyboard I";
		case 24:
			return "Keyboard O";
		case 25:
			return "Keyboard P";
		case 26:
			return "Keyboard [";
		case 27:
			return "Keyboard ]";
		case 28:
			return "Keyboard Enter";
		case 29:
			return "Keyboard Left Ctrl";
		case 30:
			return "Keyboard A";
		case 31:
			return "Keyboard S";
		case 32:
			return "Keyboard D";
		case 33:
			return "Keyboard F";
		case 34:
			return "Keyboard G";
		case 35:
			return "Keyboard H";
		case 36:
			return "Keyboard J";
		case 37:
			return "Keyboard K";
		case 38:
			return "Keyboard L";
		case 39:
			return "Keyboard ;";
		case 40:
			return "Keyboard '";
		case 41:
			return "Keyboard `";
		case 42:
			return "Keyboard Left Shift";
		case 43:
			return "Keyboard \\";
		case 44:
			return "Keyboard Z";
		case 45:
			return "Keyboard X";
		case 46:
			return "Keyboard C";
		case 47:
			return "Keyboard V";
		case 48:
			return "Keyboard B";
		case 49:
			return "Keyboard N";
		case 50:
			return "Keyboard M";
		case 51:
			return "Keyboard ,";
		case 52:
			return "Keyboard .";
		case 53:
			return "Keyboard /";
		case 54:
			return "Keyboard Right Shift";
		case 55:
			return "Keyboard Num *";
		case 56:
			return "Keyboard Left Alt";
		case 57:
			return "Keyboard Space";
		case 58:
			return "Keyboard Caps Lock";
		case 59:
			return "Keyboard F1";
		case 60:
			return "Keyboard F2";
		case 61:
			return "Keyboard F3";
		case 62:
			return "Keyboard F4";
		case 63:
			return "Keyboard F5";
		case 64:
			return "Keyboard F6";
		case 65:
			return "Keyboard F7";
		case 66:
			return "Keyboard F8";
		case 67:
			return "Keyboard F9";
		case 68:
			return "Keyboard F10";
		case 69:
			return "Keyboard Num Lock";
		case 70:
			return "Keyboard Scroll Lock";
		case 71:
			return "Keyboard Num 7";
		case 72:
			return "Keyboard Num 8";
		case 73:
			return "Keyboard Num 9";
		case 74:
			return "Keyboard Num -";
		case 75:
			return "Keyboard Num 4";
		case 76:
			return "Keyboard Num 5";
		case 77:
			return "Keyboard Num 6";
		case 78:
			return "Keyboard Num +";
		case 79:
			return "Keyboard Num 1";
		case 80:
			return "Keyboard Num 2";
		case 81:
			return "Keyboard Num 3";
		case 82:
			return "Keyboard Num 0";
		case 83:
			return "Keyboard Num Del";
		case 84:
			return "Keyboard SysRq";
		case 87:
			return "Keyboard F11";
		case 88:
			return "Keyboard F12";
		case 124:
			return "Keyboard F13";
		case 125:
			return "Keyboard F14";
		case 126:
			return "Keyboard F15";
		case 127:
			return "Keyboard F16";
		case 128:
			return "Keyboard F17";
		case 129:
			return "Keyboard F18";
		case 130:
			return "Keyboard F19";
		case 131:
			return "Keyboard F20";
		case 132:
			return "Keyboard F21";
		case 133:
			return "Keyboard F22";
		case 134:
			return "Keyboard F23";
		case 135:
			return "Keyboard F24";
		case 156:
			return "Keyboard Num Enter";
		case 157:
			return "Keyboard Right Ctrl";
		case 181:
			return "Keyboard Num /";
		case 183:
			return "Keyboard Print Screen";
		case 184:
			return "Keyboard Right Alt";
		case 199:
			return "Keyboard Home";
		case 200:
			return "Keyboard Up";
		case 201:
			return "Keyboard Page Up";
		case 203:
			return "Keyboard Left";
		case 205:
			return "Keyboard Right";
		case 207:
			return "Keyboard End";
		case 208:
			return "Keyboard Down";
		case 209:
			return "Keyboard Page Down";
		case 210:
			return "Keyboard Insert";
		case 211:
			return "Keyboard Delete";

		case 256:
			return "Mouse Left";
		case 257:
			return "Mouse Right";
		case 258:
			return "Mouse Middle";
		case 259:
			return "Mouse Button 3";
		case 260:
			return "Mouse Button 4";
		case 261:
			return "Mouse Button 5";
		case 262:
			return "Mouse Button 6";
		case 263:
			return "Mouse Button 7";
		case 264:
			return "Mouse Wheel Up";
		case 265:
			return "Mouse Wheel Down";

		case 266:
			return "Gamepad D-Pad Up";
		case 267:
			return "Gamepad D-Pad Down";
		case 268:
			return "Gamepad D-Pad Left";
		case 269:
			return "Gamepad D-Pad Right";
		case 270:
			return "Gamepad Start";
		case 271:
			return "Gamepad Back";
		case 272:
			return "Gamepad Left Thumb";
		case 273:
			return "Gamepad Right Thumb";
		case 274:
			return "Gamepad Left Shoulder";
		case 275:
			return "Gamepad Right Shoulder";
		case 276:
			return "Gamepad A";
		case 277:
			return "Gamepad B";
		case 278:
			return "Gamepad X";
		case 279:
			return "Gamepad Y";
		case 280:
			return "Gamepad Left Trigger";
		case 281:
			return "Gamepad Right Trigger";

		default:
			return "Code " + std::to_string(a_code);
		}
	}

	bool IsPressed(std::uint32_t a_code)
	{
		if (a_code >= kNoModifier) {
			return true;
		}

		const auto inputDeviceManager = RE::BSInputDeviceManager::GetSingleton();
		if (!inputDeviceManager) {
			return true;
		}

		if (a_code <= 255) {
			const auto keyboard = inputDeviceManager->GetKeyboard();
			return !keyboard || keyboard->IsPressed(a_code);
		}

		if (const auto mouseKey = ToMouseKey(a_code)) {
			const auto mouse = inputDeviceManager->GetMouse();
			return !mouse || mouse->IsPressed(*mouseKey);
		}

		if (const auto gamepadKey = ToGamepadKey(a_code)) {
			const auto gamepad = inputDeviceManager->GetGamepad();
			return !gamepad || gamepad->IsPressed(*gamepadKey);
		}

		return false;
	}
}
