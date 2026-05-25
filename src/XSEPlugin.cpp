#include "PCH.h"

#include "InputEventHandler.h"
#include "UI.h"

void InitializeLog([[maybe_unused]] spdlog::level::level_enum a_level = spdlog::level::info)
{
#ifndef NDEBUG
	auto sink = std::make_shared<spdlog::sinks::msvc_sink_mt>();
#else
	auto path = logger::log_directory();
	if (!path) {
		util::report_and_fail("Failed to find standard logging directory"sv);
	}

	*path /= std::format("{}.log"sv, Plugin::NAME);
	auto sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(path->string(), true);
#endif

	const auto level = a_level;

	auto log = std::make_shared<spdlog::logger>("global log"s, std::move(sink));
	log->set_level(level);
	log->flush_on(spdlog::level::info);

	spdlog::set_default_logger(std::move(log));
	spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%l] [%t] [%s:%#] %v");
}

/**
* Initialize the event handler.
*/
void InitializeEventHandler()
{
	spdlog::trace("Initializing event sink...");

	auto inputDeviceManager = RE::BSInputDeviceManager::GetSingleton();
	if (inputDeviceManager) {
		auto& inputEventHandler = DualWieldParryingNG::InputEventHandler::GetSingleton();
		inputDeviceManager->AddEventSink(static_cast<RE::BSTEventSink<RE::InputEvent*>*>(std::addressof(inputEventHandler)));

		if (const auto ui = RE::UI::GetSingleton()) {
			ui->AddEventSink<RE::MenuOpenCloseEvent>(
				static_cast<RE::BSTEventSink<RE::MenuOpenCloseEvent>*>(std::addressof(inputEventHandler)));
		} else {
			util::report_and_fail("Failed to initialize menu event sink.");
		}

		spdlog::trace("Event sinks initialized.");
	} else {
		util::report_and_fail("Failed to initialize event sink.");
	}
}

void MessageHandler(SKSE::MessagingInterface::Message* a_msg)
{
	if (a_msg->type == SKSE::MessagingInterface::kDataLoaded) {
		InitializeEventHandler();
		UI::Register();
	}
}

SKSEPluginLoad(const SKSE::LoadInterface* a_skse)
{
	InitializeLog();
	logger::info("Loaded plugin {} {}", Plugin::NAME, Plugin::VERSION.string());
	SKSE::Init(a_skse);
	try {
		Settings::GetSingleton()->Load();
	} catch (...) {
		logger::error("Exception caught when loading settings! Default settings will be used");
	}
	const auto messaging = SKSE::GetMessagingInterface();
	if (!messaging || !messaging->RegisterListener(MessageHandler)) {
		logger::critical("Failed to register SKSE messaging listener.");
		return false;
	}

	return true;
}
