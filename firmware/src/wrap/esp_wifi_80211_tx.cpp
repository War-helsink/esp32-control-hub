#include "Arduino.h"
#include "esp_wifi.h"

extern "C" {
    esp_err_t ieee80211_raw_frame_sanity_check(int32_t arg, int32_t arg2, int32_t arg3) {
        return ESP_OK;
    }

    esp_err_t __real_esp_wifi_80211_tx(
        wifi_interface_t interface,
        const void* buffer,
        int length,
        bool useSystemSequence
    );

    esp_err_t __wrap_esp_wifi_80211_tx(
        wifi_interface_t interface,
        const void* buffer,
        int length,
        bool useSystemSequence
    ) {
        return __real_esp_wifi_80211_tx(
            interface,
            buffer,
            length,
            useSystemSequence
        );
    }
}

