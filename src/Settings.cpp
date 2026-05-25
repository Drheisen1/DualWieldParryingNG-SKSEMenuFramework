#include "Settings.h"

#include "InputCode.h"

namespace
{
	void LoadKeyCode(
		CSimpleIniA& a_ini,
		std::uint32_t& a_value,
		const char* a_section,
		const char* a_key,
		const char* a_comment)
	{
		const auto* rawValue = a_ini.GetValue(a_section, a_key, std::to_string(a_value).c_str());
		if (const auto parsed = DualWieldParryingNG::InputCode::FromString(rawValue ? rawValue : "")) {
			a_value = *parsed;
		} else {
			logger::warn("Invalid key code value '{}' for {}.{}, keeping {}", rawValue ? rawValue : "", a_section, a_key, a_value);
		}

		a_ini.SetValue(a_section, a_key, std::to_string(a_value).c_str(), a_comment);
	}
}

Settings* Settings::GetSingleton()
{
	static Settings singleton;
	return std::addressof(singleton);
}

void Settings::Load()
{
	constexpr auto path = SETTINGS_INI_FILE;

	CSimpleIniA ini;
	ini.SetUnicode();

	ini.LoadFile(path);

	dualWieldParryingSettings.Load(ini);

	// Save back so missing keys/comments/defaults are written out
	ini.SaveFile(path);
}

void Settings::Save()
{
	constexpr auto path = SETTINGS_INI_FILE;

	CSimpleIniA ini;
	ini.SetUnicode();

	ini.LoadFile(path);

	dualWieldParryingSettings.Save(ini);

	const auto result = ini.SaveFile(path);
	if (result < 0) {
		logger::error("Failed to save INI");
	} else {
		logger::info("Saved DualWieldParryingNG.ini");
	}
}

void Settings::DualWieldParrying::Load(CSimpleIniA& a_ini)
{
	static const char* section = "DualWieldParrying";

	LoadKeyCode(
		a_ini,
		parryKey,
		section,
		"ParryKey",
		";Code or alias of key to use for parrying. See https://www.nexusmods.com/skyrimspecialedition/articles/7704");

	LoadKeyCode(
		a_ini,
		parryKey2,
		section,
		"ParryKey2",
		";Optional second keybinding. Use a big number like 1000000 if you don't want a second keybinding.");

	LoadKeyCode(
		a_ini,
		modifier,
		section,
		"Modifier",
		";Optional modifier code or alias, which must be held in addition to pressing the Parry key. Use a big number "
		"(anything greater than or equal to 300) if you don't want a modifier key.");

	LoadKeyCode(
		a_ini,
		modifier2,
		section,
		"Modifier2",
		";Optional modifier code or alias, which must be held in addition to pressing the second Parry key. Use a big number "
		"(anything greater than or equal to 300) if you don't want a modifier key.");

	detail::get_value(
		a_ini,
		allowBlockingDuringDialogue,
		section,
		"AllowBlockingDuringDialogue",
		";Allow blocking during dialogue? Primarily for users of the Dialogue Movement Enabler mod.");
}

void Settings::DualWieldParrying::Save(CSimpleIniA& a_ini) const
{
	static const char* section = "DualWieldParrying";

	a_ini.SetValue(
		section,
		"ParryKey",
		std::to_string(parryKey).c_str(),
		";Code or alias of key to use for parrying. See https://www.nexusmods.com/skyrimspecialedition/articles/7704");

	a_ini.SetValue(
		section,
		"ParryKey2",
		std::to_string(parryKey2).c_str(),
		";Optional second keybinding. Use a big number like 1000000 if you don't want a second keybinding.");

	a_ini.SetValue(
		section,
		"Modifier",
		std::to_string(modifier).c_str(),
		";Optional modifier code or alias, which must be held in addition to pressing the Parry key. Use a big number "
		"(anything greater than or equal to 300) if you don't want a modifier key.");

	a_ini.SetValue(
		section,
		"Modifier2",
		std::to_string(modifier2).c_str(),
		";Optional modifier code or alias, which must be held in addition to pressing the second Parry key. Use a big number "
		"(anything greater than or equal to 300) if you don't want a modifier key.");

	a_ini.SetBoolValue(
		section,
		"AllowBlockingDuringDialogue",
		allowBlockingDuringDialogue,
		";Allow blocking during dialogue? Primarily for users of the Dialogue Movement Enabler mod.");
}
