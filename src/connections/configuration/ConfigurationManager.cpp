//
// Created by Stein.Jonker on 26/06/2024.
//

#include "ConfigurationManager.h"

char angulon_index_html[]
        PROGMEM = R"=====(
<!doctype html>
<html lang='en' dir='ltr'>
<head>
    <meta http-equiv='Content-Type' content='text/html; charset=utf-8'/>
    <meta name='viewport' content='width=device-width, initial-scale=1.0'/>
    <title>Configure</title>
    <link href="https://cdn.jsdelivr.net/npm/bootstrap@5.0.2/dist/css/bootstrap.min.css" rel="stylesheet"
          integrity="sha384-EVSTQN3/azprG1Anm3QDgpJLIm9Nao0Yz1ztcQTwFspd3yD65VohhpuuCOmLASjC" crossorigin="anonymous">
</head>
<body>
<div class="container">
    <h1>Configure</h1>
    <form action='/configure' method='get'>
        <div class='form-group'>
            <label for='ssid'>SSID</label>
            <input type='text' class='form-control' id='ssid' name='ssid' placeholder='SSID' value='{{ssid}}'>
        </div>
        <div class='form-group'>
            <label for='password'>Password</label>
            <input type='password' class='form-control' id='password' name='password' placeholder='Password' value='{{password}}'>
        </div>
        <div class='form-group'>
            <label for='averageCanWeightGrams'>Average Can Weight (grams)</label>
            <input type='number' step='0.1' class='form-control' id='averageCanWeightGrams' name='averageCanWeightGrams' placeholder='Average Can Weight' value='{{averageCanWeightGrams}}'>
        </div>
        <button type='submit' class='btn btn-primary mt-3'>Submit</button>
    </form>
</div>
</body>
</html>
)=====";

SystemConfiguration ConfigurationManager::systemConfiguration;
#define NETWORK_TIMEOUT 10000

void ConfigurationManager::setup() {
    Serial.println("Loading configuration");

    preferences.begin("configuration", false);

    if (!isConfigured()) {
        ConfigurationManager::startConfigurationMode();
    } else {
        this->loadConfiguration();
        this->setupWiFi();
    }

    // TODO: This always creates a webserver so you can reset or reconfigure the device at any time. Do we want this behaviour?
    this->setupWebserver();
}


void ConfigurationManager::startConfigurationMode() {
    Serial.println("Device has not been configured. Starting access point for configuration...");

    WiFi.softAP(hotspotName, hotspotPassword);
    IPAddress IP = WiFi.softAPIP();

    Serial.println("Access point started");
    Serial.println("Connect to the hotspot with the following credentials:");
    Serial.println(hotspotName);
    Serial.println(hotspotPassword);
    Serial.println("After connecting, navigate to:");
    Serial.println(IP.toString().c_str());
}


void ConfigurationManager::setupWebserver() {
    server->on("/", [this]() {
        SystemConfiguration configuration = ConfigurationManager::getConfig();
        String ssid = configuration.ssid;
        String password = configuration.password;

        String html = angulon_index_html;
        html.replace("{{ssid}}", ssid);
        html.replace("{{password}}", password);
        html.replace("{{averageCanWeightGrams}}", String(configuration.averageCanWeightGrams));
        server->send(200, "text/html", html);
    });
    server->on("/configure", [this]() {
        this->configureDevice();
    });
    server->on("/reconfigure", [this]() {
        Serial.println("Reconfiguring device");
        preferences.putBool("isConfigured", false);
        ESP.restart();
    });
    server->on("/restart", [this]() {
        Serial.println("Rebooting...");
        ESP.restart();
    });

    server->begin();
    Serial.println("Webserver started");
}

/**
 * Handler the submit of the configuration formm
 */
void ConfigurationManager::configureDevice() {
    const String ssid = server->arg("ssid");
    const String password = server->arg("password");
    const float averageCanWeightGrams = server->arg("averageCanWeightGrams").toFloat();

    // Check if the ssid, password and averageCanWeightGrams are not empty
    if (ssid.length() == 0 || password.length() == 0 || averageCanWeightGrams == 0.0) {
        server->send(400, "text/plain", "SSID, password and average can weight are required");
        return;
    }

    // Save the configuration
    preferences.putString("ssid", ssid);
    preferences.putString("password", password);
    preferences.putFloat("averageCanWeightGrams", averageCanWeightGrams);

    // Set the device as configured
    preferences.putBool("isConfigured", true);

    // Return a correct HTTP Code and restart the device
    server->send(200, "text/plain", "Configuration saved. Restarting device...");
    Serial.println("Configuration saved successfully. Restarting device...");
    delay(1000);
    ESP.restart();
}

void ConfigurationManager::setupWiFi() {
    SystemConfiguration configuration = ConfigurationManager::getConfig();
    this->ssid = configuration.ssid.c_str();
    this->networkPassword = configuration.password.c_str();

    WiFi.begin(ssid, networkPassword);
    Serial.println("Connecting to WiFi network: " + configuration.ssid);

    // Try to connect to the Wi-Fi with a delay of 500 ms each time. If it does not connect after NETWORK_TIMEOUT, it will start configure mode
    unsigned long start = millis();
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");

        if (millis() - start > NETWORK_TIMEOUT) {
            Serial.println("Could not connect to WiFi network. Rebooting in configuration mode...");
            preferences.putBool("isConfigured", false);
            ESP.restart();
        }
    }

    Serial.println("Connected to WiFi network");
    Serial.println("IP address: " + WiFi.localIP().toString());
}

void ConfigurationManager::run() {
    server->handleClient();

    // We only need to perform the Wi-Fi connection check if the system is configured
    if (!isConfigured()) return;

    if (WiFi.status() != WL_CONNECTED) {
        unsigned long now = millis();
        if (now - lastTimeConnected >= NETWORK_TIMEOUT) {
            Serial.println("Lost connection to WiFi- Rebooting!");
            ESP.restart();
        }
    } else {
        lastTimeConnected = millis();
    }
}


void ConfigurationManager::loadConfiguration() {
    SystemConfiguration config{};
    config.ssid = preferences.getString("ssid", "");
    config.password = preferences.getString("password", "");
    config.averageCanWeightGrams = preferences.getFloat("averageCanWeightGrams", 0.0);
    systemConfiguration = config;
}

SystemConfiguration ConfigurationManager::getConfig() {
    return systemConfiguration;
}

bool ConfigurationManager::isConfigured() {
    return preferences.getBool("isConfigured", false);
}
