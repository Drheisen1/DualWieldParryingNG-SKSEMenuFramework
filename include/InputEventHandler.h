#pragma once

#include <RE/Skyrim.h>

#include "Settings.h"

namespace DualWieldParryingNG {
#pragma warning(push)
#pragma warning(disable : 4251)

    class InputEventHandler :
        public RE::BSTEventSink<RE::InputEvent*>,
        public RE::BSTEventSink<RE::MenuOpenCloseEvent> {
    public:
        RE::BSEventNotifyControl ProcessEvent(RE::InputEvent* const* a_event, 
                                              RE::BSTEventSource<RE::InputEvent*>* a_eventSource) override;
        RE::BSEventNotifyControl ProcessEvent(const RE::MenuOpenCloseEvent* a_event,
                                              RE::BSTEventSource<RE::MenuOpenCloseEvent>* a_eventSource) override;

        [[nodiscard]] static InputEventHandler& GetSingleton() noexcept;

    private:
        InputEventHandler() = default;
        InputEventHandler(const InputEventHandler&) = delete;
        InputEventHandler(InputEventHandler&&) = delete;
        ~InputEventHandler() = default;

        InputEventHandler& operator=(const InputEventHandler&) = delete;
        InputEventHandler& operator=(InputEventHandler&&) = delete;

        bool IsModifierKeyPressed(uint32_t modifierKey) const;
        void StopPluginBlock(std::string_view a_reason);
        void StopPluginBlockIfWeaponNotDrawn(std::string_view a_reason);

        std::atomic_bool pluginBlockingActive_{ false };
    };

#pragma warning(pop)
}  // namespace DualWieldParryingNG
