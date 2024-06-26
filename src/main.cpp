//
// Created by Stein.Jonker on 13/06/2024.
//

#include <Arduino.h>


#include "HX711.h"
#include "connections/configuration/ConfigurationManager.h"
#include "hardware/scale/Scale.h"


ConfigurationManager configurationManager;
Scale scale;

void setup() {
    Serial.begin(57600);
    configurationManager.setup();
    scale.setup();
}

void loop() {
    configurationManager.run();
    // Uncomment this line to configure the device first time. It calls delay() which makes it impossible to connect to the AP
    scale.run();


    Serial.println("Amount of cans currently in the bin: " + String(round(scale.getAmountOfCans())));
}