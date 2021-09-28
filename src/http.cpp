#include "http.h"

char version_url[1300] = "http://172.20.10.5:88/version";
String latestVersion="";
String result = "NC"; // NC = not connected to WiFi
String payload = "NULL";
int httpResponseCode = -1;


String http_version_check()
{
    result = "NC"; // NC = not connected to WiFi
    payload = "NULL";
    httpResponseCode =1;
    if (WiFi.status() == WL_CONNECTED)
    {
        WiFiClient client;
        HTTPClient http;
        http.begin(client, version_url); // Your Domain name with URL path or IP address with path
        httpResponseCode = http.GET();
        result = "";
        result += String(httpResponseCode);
        result += ",";
        if (httpResponseCode > 0)
        {
            // Serial.print("HTTP Response code: ");
            // Serial.println(httpResponseCode);
            payload = http.getString();
            latestVersion = payload;
            // Serial.println(payload);
        }
        result += payload;
        http.end();
    }
    return latestVersion;
}