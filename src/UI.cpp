#include "PCH.h"
#include "UI.h"
#include "SKSEMenuFramework.h"
#include "Settings.h"

namespace
{
	constexpr std::uint32_t kDisabledKey = 1'000'000;
	constexpr std::uint32_t kNoModifier = 300;

	struct KeyOption
	{
		const char* label;
		std::uint32_t code;
	};

	struct MenuState
	{
		Settings::DualWieldParrying pending{};
		bool initialized{ false };
	};

	MenuState g_menuState{};

	void SyncFromSettings()
	{
		g_menuState.pending = Settings::GetSingleton()->dualWieldParryingSettings;
	}

	void ApplyToSettings()
	{
		Settings::GetSingleton()->dualWieldParryingSettings = g_menuState.pending;
	}

	const std::vector<KeyOption>& GetParryKeyOptions()
	{
		static const std::vector<KeyOption> options{
			{ "Keyboard V", 47 },
			{ "Keyboard Left Alt", 56 },
			{ "Keyboard Left Ctrl", 29 },
			{ "Keyboard Left Shift", 42 },
			{ "Keyboard Right Shift", 54 },
			{ "Keyboard Q", 16 },
			{ "Keyboard E", 18 },
			{ "Keyboard R", 19 },
			{ "Keyboard F", 33 },
			{ "Keyboard Z", 44 },
			{ "Keyboard X", 45 },
			{ "Keyboard C", 46 },

			{ "Mouse Left", 256 },
			{ "Mouse Right", 257 },
			{ "Mouse Middle", 258 },
			{ "Mouse Button 3", 259 },
			{ "Mouse Button 4", 260 },
			{ "Mouse Button 5", 261 },
			{ "Mouse Button 6", 262 },
			{ "Mouse Button 7", 263 },

			{ "Gamepad Left Shoulder", 274 },
			{ "Gamepad Right Shoulder", 275 },
			{ "Gamepad A", 276 },
			{ "Gamepad B", 277 },
			{ "Gamepad X", 278 },
			{ "Gamepad Y", 279 },
			{ "Gamepad Left Trigger", 280 },
			{ "Gamepad Right Trigger", 281 },

			{ "Disabled", kDisabledKey }
		};

		return options;
	}

	const std::vector<KeyOption>& GetModifierOptions()
	{
		static const std::vector<KeyOption> options{
			{ "None", kNoModifier },

			{ "Keyboard Left Alt", 56 },
			{ "Keyboard Left Ctrl", 29 },
			{ "Keyboard Left Shift", 42 },
			{ "Keyboard Right Shift", 54 },
			{ "Keyboard Q", 16 },
			{ "Keyboard E", 18 },
			{ "Keyboard R", 19 },
			{ "Keyboard F", 33 },
			{ "Keyboard Z", 44 },
			{ "Keyboard X", 45 },
			{ "Keyboard C", 46 },

			{ "Mouse Left", 257 },
			{ "Mouse Right", 258 },
			{ "Mouse Middle", 259 },
			{ "Mouse Button 4", 260 },
			{ "Mouse Button 5", 261 },
			{ "Mouse Button 6", 262 },
			{ "Mouse Button 7", 263 },

			{ "Gamepad Left Shoulder", 274 },
			{ "Gamepad Right Shoulder", 275 },
			{ "Gamepad A", 276 },
			{ "Gamepad B", 277 },
			{ "Gamepad X", 278 },
			{ "Gamepad Y", 279 },
			{ "Gamepad Left Trigger", 280 },
			{ "Gamepad Right Trigger", 281 }
		};

		return options;
	}

	std::string KeyCodeToString(std::uint32_t keyCode, bool isModifier)
	{
		if (!isModifier && keyCode >= kDisabledKey) {
			return "Disabled";
		}

		if (isModifier && keyCode >= kNoModifier) {
			return "None";
		}

		switch (keyCode) {
		case 16:
			return "Keyboard Q";
		case 18:
			return "Keyboard E";
		case 19:
			return "Keyboard R";
		case 29:
			return "Keyboard Left Ctrl";
		case 33:
			return "Keyboard F";
		case 42:
			return "Keyboard Left Shift";
		case 44:
			return "Keyboard Z";
		case 45:
			return "Keyboard X";
		case 46:
			return "Keyboard C";
		case 47:
			return "Keyboard V";
		case 54:
			return "Keyboard Right Shift";
		case 56:
			return "Keyboard Left Alt";

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
			return "Code " + std::to_string(keyCode);
		}
	}

	void DrawCombo(const char* label, std::uint32_t& value, const std::vector<KeyOption>& options, bool isModifier)
	{
		ImGuiMCP::TextUnformatted(label);
		ImGuiMCP::SameLine(260.0f);

		const std::string preview = KeyCodeToString(value, isModifier);

		if (ImGuiMCP::BeginCombo(label, preview.c_str())) {
			for (const auto& option : options) {
				const bool selected = (value == option.code);
				if (ImGuiMCP::Selectable(option.label, selected)) {
					value = option.code;
				}
				if (selected) {
					ImGuiMCP::SetItemDefaultFocus();
				}
			}
			ImGuiMCP::EndCombo();
		}
	}

	void ResetToDefaults(Settings::DualWieldParrying& config)
	{
		config.parryKey = 47;
		config.parryKey2 = kDisabledKey;
		config.modifier = kNoModifier;
		config.modifier2 = kNoModifier;
		config.allowBlockingDuringDialogue = false;
	}
}

namespace UI
{
	void Register()
	{
		if (!SKSEMenuFramework::IsInstalled()) {
			logger::warn("SKSE Menu Framework not detected, skipping menu registration.");
			return;
		}

		SKSEMenuFramework::SetSection("Dual Wield Parrying");
		SKSEMenuFramework::AddSectionItem("Settings", DualWieldParrying::Render);

		logger::info("Registered Dual Wield Parrying page with SKSE Menu Framework.");
	}

	namespace DualWieldParrying
	{
		void __stdcall Render()
		{
			auto* settings = Settings::GetSingleton();

			if (!g_menuState.initialized) {
				SyncFromSettings();
				g_menuState.initialized = true;
			}

			auto& config = g_menuState.pending;

			ImGuiMCP::TextUnformatted("Dual Wield Parrying");
			ImGuiMCP::Separator();
			ImGuiMCP::TextWrapped("This menu edits the INI only. Gameplay input handling remains in the NG runtime event handler.");

			ImGuiMCP::Spacing();
			ImGuiMCP::Separator();
			ImGuiMCP::Spacing();

			ImGuiMCP::TextUnformatted("Primary Keybind");
			DrawCombo("Parry Key", config.parryKey, GetParryKeyOptions(), false);

			ImGuiMCP::Spacing();

			ImGuiMCP::TextUnformatted("Secondary Keybind");
			DrawCombo("Parry Key 2", config.parryKey2, GetParryKeyOptions(), false);

			bool secondKeyDisabled = (config.parryKey2 >= kDisabledKey);
			if (ImGuiMCP::Checkbox("Disable second keybind", &secondKeyDisabled)) {
				if (secondKeyDisabled) {
					config.parryKey2 = kDisabledKey;
				} else if (config.parryKey2 >= kDisabledKey) {
					config.parryKey2 = 47;
				}
			}

			ImGuiMCP::TextDisabled("Enable this if you want only one parry key.");

			ImGuiMCP::Spacing();
			ImGuiMCP::Separator();
			ImGuiMCP::Spacing();

			ImGuiMCP::TextUnformatted("Modifiers");
			DrawCombo("Modifier", config.modifier, GetModifierOptions(), true);
			DrawCombo("Modifier 2", config.modifier2, GetModifierOptions(), true);

			bool noModifier1 = (config.modifier >= kNoModifier);
			if (ImGuiMCP::Checkbox("No modifier for primary key", &noModifier1)) {
				config.modifier = noModifier1 ? kNoModifier : 56;
			}

			bool noModifier2 = (config.modifier2 >= kNoModifier);
			if (ImGuiMCP::Checkbox("No modifier for secondary key", &noModifier2)) {
				config.modifier2 = noModifier2 ? kNoModifier : 56;
			}

			ImGuiMCP::TextDisabled("Enable this if the parry key should work by itself.");

			ImGuiMCP::Spacing();
			ImGuiMCP::Separator();
			ImGuiMCP::Spacing();

			ImGuiMCP::Checkbox("Allow blocking during dialogue", &config.allowBlockingDuringDialogue);

			ImGuiMCP::Spacing();
			ImGuiMCP::Separator();
			ImGuiMCP::Spacing();

			if (ImGuiMCP::Button("Reset to Defaults")) {
				ResetToDefaults(config);
			}

			ImGuiMCP::SameLine();

			if (ImGuiMCP::Button("Apply")) {
				ApplyToSettings();
			}

			ImGuiMCP::SameLine();

			if (ImGuiMCP::Button("Save INI")) {
				ApplyToSettings();
				settings->Save();
			}

			ImGuiMCP::SameLine();

			if (ImGuiMCP::Button("Reload INI")) {
				settings->Load();
				SyncFromSettings();
			}
		}
	}
}