//
// Created by Stein.Jonker on 26/06/2024.
//

#ifndef CANCOUNTER_CONFIGURATIONMANAGER_H
#define CANCOUNTER_CONFIGURATIONMANAGER_H

#include <Preferences.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include "SystemConfiguration.h"

class ConfigurationManager {
    private:
        ESP8266WebServer *server = new ESP8266WebServer (80);
        Preferences preferences;

        const char *hotspotPassword = "YesICanCount";
        const char *hotspotName = "CanTheCanCounterCountCans?";
        const char *ssid;
        const char *networkPassword;

        unsigned long lastTimeConnected = 0L;

    void setupWebserver();
        void setupWiFi();
        void loadConfiguration();
        void startConfigurationMode();
        void configureDevice();

    public:
        void setup();
        void run();
        bool isConfigured();
        static SystemConfiguration getConfig();
        static SystemConfiguration systemConfiguration;
};


#endif //CANCOUNTER_CONFIGURATIONMANAGER_H
