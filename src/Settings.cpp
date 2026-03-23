#include "Settings.h"

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

	detail::get_value(
		a_ini,
		parryKey,
		section,
		"ParryKey",
		";Code of key to use for parrying. See https://www.creationkit.com/index.php?title=Input_Script#DXScanCodes");

	detail::get_value(
		a_ini,
		parryKey2,
		section,
		"ParryKey2",
		";Optional second keybinding. Use a big number like 1000000 if you don't want a second keybinding.");

	detail::get_value(
		a_ini,
		modifier,
		section,
		"Modifier",
		";Optional modifier, which must be held in addition to pressing the Parry key. Use a big number "
		"(anything greater than or equal to 300) if you don't want a modifier key.");

	detail::get_value(
		a_ini,
		modifier2,
		section,
		"Modifier2",
		";Optional modifier, which must be held in addition to pressing the second Parry key. Use a big number "
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
		";Code of key to use for parrying. See https://www.creationkit.com/index.php?title=Input_Script#DXScanCodes");

	a_ini.SetValue(
		section,
		"ParryKey2",
		std::to_string(parryKey2).c_str(),
		";Optional second keybinding. Use a big number like 1000000 if you don't want a second keybinding.");

	a_ini.SetValue(
		section,
		"Modifier",
		std::to_string(modifier).c_str(),
		";Optional modifier, which must be held in addition to pressing the Parry key. Use a big number "
		"(anything greater than or equal to 300) if you don't want a modifier key.");

	a_ini.SetValue(
		section,
		"Modifier2",
		std::to_string(modifier2).c_str(),
		";Optional modifier, which must be held in addition to pressing the second Parry key. Use a big number "
		"(anything greater than or equal to 300) if you don't want a modifier key.");

	a_ini.SetBoolValue(
		section,
		"AllowBlockingDuringDialogue",
		allowBlockingDuringDialogue,
		";Allow blocking during dialogue? Primarily for users of the Dialogue Movement Enabler mod.");
}