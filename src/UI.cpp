#include "PCH.h"

#include "UI.h"

#include "InputCode.h"
#include "SKSEMenuFramework.h"
#include "Settings.h"

namespace
{
	enum class CaptureTarget
	{
		None,
		ParryKey,
		ParryKey2,
		Modifier,
		Modifier2
	};

	struct MenuState
	{
		Settings::DualWieldParrying pending{};
		bool initialized{ false };
	};

	MenuState g_menuState{};
	std::atomic<CaptureTarget> g_captureTarget{ CaptureTarget::None };
	SKSEMenuFramework::Model::InputEvent* g_inputEvent{ nullptr };

	void SyncFromSettings()
	{
		g_menuState.pending = Settings::GetSingleton()->dualWieldParryingSettings;
		g_captureTarget.store(CaptureTarget::None);
	}

	void ApplyToSettings()
	{
		Settings::GetSingleton()->dualWieldParryingSettings = g_menuState.pending;
	}

	void ResetToDefaults(Settings::DualWieldParrying& config)
	{
		config.parryKey = 47;
		config.parryKey2 = DualWieldParryingNG::InputCode::kDisabledKey;
		config.modifier = DualWieldParryingNG::InputCode::kNoModifier;
		config.modifier2 = DualWieldParryingNG::InputCode::kNoModifier;
		config.allowBlockingDuringDialogue = false;
		g_captureTarget.store(CaptureTarget::None);
	}

	void SetCapturedCode(CaptureTarget target, std::uint32_t code)
	{
		switch (target) {
		case CaptureTarget::ParryKey:
			g_menuState.pending.parryKey = code;
			break;
		case CaptureTarget::ParryKey2:
			g_menuState.pending.parryKey2 = code;
			break;
		case CaptureTarget::Modifier:
			g_menuState.pending.modifier = code;
			break;
		case CaptureTarget::Modifier2:
			g_menuState.pending.modifier2 = code;
			break;
		case CaptureTarget::None:
		default:
			break;
		}
	}

	bool __stdcall OnInput(RE::InputEvent* event)
	{
		const auto target = g_captureTarget.load();
		if (target == CaptureTarget::None) {
			return false;
		}

		for (auto* input = event; input; input = input->next) {
			if (input->eventType != RE::INPUT_EVENT_TYPE::kButton) {
				continue;
			}

			const auto* button = input->AsButtonEvent();
			if (!button || !button->IsDown()) {
				continue;
			}

			const auto code = DualWieldParryingNG::InputCode::FromButtonEvent(*button);
			if (!code || !DualWieldParryingNG::InputCode::IsHoldable(*code)) {
				continue;
			}

			if (button->device.get() == RE::INPUT_DEVICE::kKeyboard && *code == 1) {
				g_captureTarget.store(CaptureTarget::None);
				return true;
			}

			SetCapturedCode(target, *code);
			g_captureTarget.store(CaptureTarget::None);
			return true;
		}

		return false;
	}

	void DrawKeybindCapture(
		const char* label,
		const char* id,
		CaptureTarget target,
		std::uint32_t& value,
		bool isModifier)
	{
		const auto activeTarget = g_captureTarget.load();
		const bool isActive = activeTarget == target;
		const std::string currentValue = DualWieldParryingNG::InputCode::ToString(value, isModifier);

		ImGuiMCP::TextUnformatted(label);
		ImGuiMCP::SameLine(180.0f);
		ImGuiMCP::TextUnformatted(currentValue.c_str());
		ImGuiMCP::SameLine(360.0f);

		if (isActive) {
			ImGuiMCP::TextDisabled("Waiting...");
			ImGuiMCP::SameLine();

			const std::string cancelLabel = std::string("Cancel##") + id;
			if (ImGuiMCP::Button(cancelLabel.c_str())) {
				g_captureTarget.store(CaptureTarget::None);
			}
			return;
		}

		const std::string buttonLabel = std::string("Select Keybind##") + id;
		if (ImGuiMCP::Button(buttonLabel.c_str())) {
			g_captureTarget.store(target);
		}
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

		if (!g_inputEvent) {
			g_inputEvent = SKSEMenuFramework::AddInputEvent(OnInput);
		}

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
			DrawKeybindCapture("Parry Key", "ParryKey", CaptureTarget::ParryKey, config.parryKey, false);

			ImGuiMCP::Spacing();

			ImGuiMCP::TextUnformatted("Secondary Keybind");
			DrawKeybindCapture("Parry Key 2", "ParryKey2", CaptureTarget::ParryKey2, config.parryKey2, false);

			bool secondKeyDisabled = (config.parryKey2 >= DualWieldParryingNG::InputCode::kDisabledKey);
			if (ImGuiMCP::Checkbox("Disable second keybind", &secondKeyDisabled)) {
				if (secondKeyDisabled) {
					config.parryKey2 = DualWieldParryingNG::InputCode::kDisabledKey;
					g_captureTarget.store(CaptureTarget::None);
				} else if (config.parryKey2 >= DualWieldParryingNG::InputCode::kDisabledKey) {
					config.parryKey2 = 47;
				}
			}

			ImGuiMCP::TextDisabled("Enable this if you want only one parry key.");

			ImGuiMCP::Spacing();
			ImGuiMCP::Separator();
			ImGuiMCP::Spacing();

			ImGuiMCP::TextUnformatted("Modifiers");
			DrawKeybindCapture("Modifier", "Modifier", CaptureTarget::Modifier, config.modifier, true);
			DrawKeybindCapture("Modifier 2", "Modifier2", CaptureTarget::Modifier2, config.modifier2, true);

			bool noModifier1 = (config.modifier >= DualWieldParryingNG::InputCode::kNoModifier);
			if (ImGuiMCP::Checkbox("No modifier for primary key", &noModifier1)) {
				config.modifier = noModifier1 ? DualWieldParryingNG::InputCode::kNoModifier : 56;
				g_captureTarget.store(CaptureTarget::None);
			}

			bool noModifier2 = (config.modifier2 >= DualWieldParryingNG::InputCode::kNoModifier);
			if (ImGuiMCP::Checkbox("No modifier for secondary key", &noModifier2)) {
				config.modifier2 = noModifier2 ? DualWieldParryingNG::InputCode::kNoModifier : 56;
				g_captureTarget.store(CaptureTarget::None);
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
