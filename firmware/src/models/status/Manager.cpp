#include "Manager.h"
#include "core/Logger.h"
#include "api/event/EventsApi.h"

StatusState StatusManager::_state = {
    false,
    {
        StatusModule::None,
        StatusAction::None
    }
};

bool StatusManager::start(
    StatusModule module,
    StatusAction action
) {
    if (_state.running) {
        return false;
    }

    _state = {
        true,
        {
            module,
            action
        }
    };

    notify();

    return true;
}

bool StatusManager::stop(
    StatusModule module,
    StatusAction action
) {
    if (!_state.running) {
        return true;
    }

    if (
        _state.activeProcess.module != module ||
        _state.activeProcess.action != action
    ) {
        return false;
    }

    _state = {
        false,
        {
            StatusModule::None,
            StatusAction::None
        }
    };

    notify();

    return true;
}

bool StatusManager::isRunning() {
    return _state.running;
}

bool StatusManager::isRunning(
    StatusModule module,
    StatusAction action
) {
    return (
        _state.running &&
        _state.activeProcess.module == module &&
        _state.activeProcess.action == action
    );
}

const StatusState* StatusManager::getState() {
    return &_state;
}

void StatusManager::notify() {
    EventsApi::sendStatus(_state);
}

const char* StatusManager::moduleToString(
    StatusModule module
) {
    switch (module) {
        case StatusModule::Wifi:
            return "wifi";

        case StatusModule::Bluetooth:
            return "bluetooth";

        default:
            return "";
    }
}

const char* StatusManager::actionToString(
    StatusAction action
) {
    switch (action) {
        case StatusAction::WifiScan:
            return "scan";

        case StatusAction::WifiDeauth:
            return "deauth";

        case StatusAction::WifiBeacon:
            return "beacon";

        case StatusAction::WifiHandshake:
            return "handshake";

        case StatusAction::BluetoothScan:
            return "scan";

        case StatusAction::BluetoothJammer:
            return "jammer";

        default:
            return "";
    }
}