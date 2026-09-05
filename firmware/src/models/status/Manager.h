#pragma once
#include "Arduino.h"
#include "types.h"

class StatusManager {
public:
    static bool start(
        StatusModule module,
        StatusAction action
    );

    static bool stop(
        StatusModule module,
        StatusAction action
    );

    static bool isRunning();

    static bool isRunning(
        StatusModule module,
        StatusAction action
    );

    static const StatusState* getState();

    static const char* moduleToString(
        StatusModule module
    );

    static const char* actionToString(
        StatusAction action
    );

private:
    static StatusState _state;

    static void notify();
};