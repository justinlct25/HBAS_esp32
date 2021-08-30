#include "wifi_c.h"

const char *ssid = "REPLACE_WITH_YOUR_SSID";
const char *password = "REPLACE_WITH_YOUR_PASSWORD";

void wifi_init()
{
    WiFi.begin(ssid, password);
    WiFi.setAutoReconnect(true);
    WiFi.persistent(true);
}

bool wifi_stat() // connected will return true
{
    bool state = false;
    if (WiFi.status() == WL_CONNECTED)
    {
        state = true;
    }
    return state;
}

void wifi_reconnect() //please give this function sometime to run before rerun
{
    WiFi.disconnect();
    WiFi.begin(ssid, password);
}

int wifi_strength()
{
    int rssi = WiFi.RSSI();
    return rssi;
}