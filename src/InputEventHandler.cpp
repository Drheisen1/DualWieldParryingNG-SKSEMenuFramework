#include "InputEventHandler.h"

#include "InputCode.h"

using namespace DualWieldParryingNG;

InputEventHandler& InputEventHandler::GetSingleton() noexcept {
    static InputEventHandler instance;
    return instance;
}

RE::BSEventNotifyControl InputEventHandler::ProcessEvent(RE::InputEvent* const* a_event,
    RE::BSTEventSource<RE::InputEvent*>* a_eventSource) {
	
    (void)a_eventSource;

    if (a_event) {
        StopPluginBlockIfWeaponNotDrawn("weapon no longer drawn");

        const auto ui = RE::UI::GetSingleton();
        const auto settings = Settings::GetSingleton();

        if (ui && !ui->GameIsPaused() && !ui->IsApplicationMenuOpen() && !ui->IsItemMenuOpen() && 
            (!ui->IsMenuOpen(RE::InterfaceStrings::GetSingleton()->dialogueMenu) || settings->dualWieldParryingSettings.allowBlockingDuringDialogue)) {
            const auto controlMap = RE::ControlMap::GetSingleton();
            const auto playerCharacter = RE::PlayerCharacter::GetSingleton();
            const auto playerControls = RE::PlayerControls::GetSingleton();

            if (controlMap && playerCharacter && playerControls && playerControls->attackBlockHandler) {
                if (controlMap->IsFightingControlsEnabled() &&
                    playerControls->attackBlockHandler->inputEventHandlingEnabled) {
                    const auto playerState = playerCharacter->AsActorState();
                    auto playerAI = playerCharacter->GetActorRuntimeData().currentProcess;

                    if (playerState && playerState->GetWeaponState() == RE::WEAPON_STATE::kDrawn &&
                        playerState->GetSitSleepState() == RE::SIT_SLEEP_STATE::kNormal &&
                        playerState->GetKnockState() == RE::KNOCK_STATE_ENUM::kNormal &&
                        playerState->GetFlyState() == RE::FLY_STATE::kNone && 
                        !playerCharacter->IsInKillMove() &&
                        playerAI) {

                        // Bow / crossbow checks necessary to stop the block button from zooming in
                        // when we have the perk for that
                        auto playerEquippedLeft = playerAI->GetEquippedLeftHand();
                        auto playerLeftWeap = playerEquippedLeft == nullptr ? nullptr : playerEquippedLeft->As<RE::TESObjectWEAP>();
                        auto leftHandSpell = playerCharacter->GetActorRuntimeData().selectedSpells[RE::Actor::SlotTypes::kLeftHand];

                        if (playerLeftWeap == nullptr || (playerLeftWeap->GetWeaponType() != RE::WEAPON_TYPE::kBow &&
                                                          playerLeftWeap->GetWeaponType() != RE::WEAPON_TYPE::kCrossbow)) {
                            bool isBlocking = false;
                            if (playerCharacter->GetGraphVariableBool("IsBlocking", isBlocking)) {
                                // We managed to successfully read the graph variable
                                const auto parryKey = settings->dualWieldParryingSettings.parryKey;
                                const auto parryKey2 = settings->dualWieldParryingSettings.parryKey2;
                                const auto modifierKey = settings->dualWieldParryingSettings.modifier;
                                const auto modifierKey2 = settings->dualWieldParryingSettings.modifier2;

                                for (auto ev = *a_event; ev != nullptr; ev = ev->next) {
                                    if (ev && ev->eventType == RE::INPUT_EVENT_TYPE::kButton) {
                                        const auto buttonEvent = ev->AsButtonEvent();
                                        if (buttonEvent) {
                                            const auto eventName =
                                                controlMap->GetUserEventName(buttonEvent->GetIDCode(), buttonEvent->device.get());

                                            if (pluginBlockingActive_.load() &&
                                                eventName == RE::UserEvents::GetSingleton()->readyWeapon &&
                                                (buttonEvent->IsDown() || buttonEvent->IsHeld())) {
                                                StopPluginBlock("ready weapon pressed");
                                                continue;
                                            }

                                            const auto keyCode = InputCode::FromButtonEvent(*buttonEvent);
                                            if (!keyCode) {
                                                continue;
                                            }

                                            bool mainKeyPressed = false;
                                            uint32_t requiredModifierKey = 10000;
                                            if (*keyCode == parryKey) {
                                                mainKeyPressed = true;
                                                requiredModifierKey = modifierKey;
                                            } else if (*keyCode == parryKey2) {
                                                mainKeyPressed = true;
                                                requiredModifierKey = modifierKey2;
                                            }

                                            if (mainKeyPressed) {
                                                if (eventName == RE::UserEvents::GetSingleton()->leftAttack) {
                                                    // Special case for when the Dual Wield Parrying block key is exactly
                                                    // the same as the left-attack key for the game engine.
                                                    if (leftHandSpell &&
                                                        leftHandSpell->GetSpellType() !=
                                                            RE::MagicSystem::SpellType::kPoison &&
                                                        leftHandSpell->GetSpellType() !=
                                                            RE::MagicSystem::SpellType::kEnchantment) {
                                                        // Have spell in left hand, so just allow casting instead of blocking
                                                        return RE::BSEventNotifyControl::kContinue;
                                                    } else if (playerLeftWeap && playerLeftWeap->GetWeaponType() == RE::WEAPON_TYPE::kStaff) {
                                                        // Also allow casting spells in left hand
                                                        return RE::BSEventNotifyControl::kContinue;
                                                    }
                                                }

                                                // Event for parry key
                                                if (buttonEvent->IsHeld() &&
                                                    IsModifierKeyPressed(requiredModifierKey)) {
                                                    // Player wants to block
                                                    playerState->actorState2.wantBlocking = 1;
                                                    if (!isBlocking) {
                                                        playerCharacter->NotifyAnimationGraph("blockStart");
                                                        pluginBlockingActive_.store(true);
                                                    }
                                                } else if (buttonEvent->IsUp()) {
                                                    // Player wants to stop blocking
                                                    playerState->actorState2.wantBlocking = 0;
                                                    if (isBlocking) {
                                                        playerCharacter->NotifyAnimationGraph("blockStop");
                                                    }
                                                    pluginBlockingActive_.store(false);
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    // Let the game continue handling this event
    return RE::BSEventNotifyControl::kContinue;
}

RE::BSEventNotifyControl InputEventHandler::ProcessEvent(
    const RE::MenuOpenCloseEvent* a_event,
    RE::BSTEventSource<RE::MenuOpenCloseEvent>* a_eventSource)
{
    (void)a_eventSource;

    if (!a_event || !a_event->opening || !pluginBlockingActive_.load()) {
        return RE::BSEventNotifyControl::kContinue;
    }

    const auto* interfaceStrings = RE::InterfaceStrings::GetSingleton();
    if (interfaceStrings && a_event->menuName == interfaceStrings->dialogueMenu) {
        return RE::BSEventNotifyControl::kContinue;
    }

    StopPluginBlock(std::format("menu opened: {}", a_event->menuName.c_str()));
    return RE::BSEventNotifyControl::kContinue;
}

bool InputEventHandler::IsModifierKeyPressed(uint32_t modifierKey) const {
    return InputCode::IsPressed(modifierKey);
}

void InputEventHandler::StopPluginBlock(std::string_view a_reason)
{
    pluginBlockingActive_.store(false);

    const auto playerCharacter = RE::PlayerCharacter::GetSingleton();
    if (!playerCharacter) {
        logger::trace("Cleared Dual Wield Parrying block state without player: {}", a_reason);
        return;
    }

    const auto playerState = playerCharacter->AsActorState();
    if (playerState) {
        playerState->actorState2.wantBlocking = 0;
    }

    bool isBlocking = false;
    if (playerCharacter->GetGraphVariableBool("IsBlocking", isBlocking) && isBlocking) {
        playerCharacter->NotifyAnimationGraph("blockStop");
    }

    logger::trace("Cleared Dual Wield Parrying block state: {}", a_reason);
}

void InputEventHandler::StopPluginBlockIfWeaponNotDrawn(std::string_view a_reason)
{
    if (!pluginBlockingActive_.load()) {
        return;
    }

    const auto playerCharacter = RE::PlayerCharacter::GetSingleton();
    const auto playerState = playerCharacter ? playerCharacter->AsActorState() : nullptr;
    if (!playerState) {
        return;
    }

    if (playerState->GetWeaponState() != RE::WEAPON_STATE::kDrawn) {
        StopPluginBlock(a_reason);
    }
}
