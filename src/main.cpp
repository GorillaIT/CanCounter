//
// Created by Stein.Jonker on 13/06/2024.
//

#include <Arduino.h>


#include "HX711.h"
#include "connections/configuration/ConfigurationManager.h"
#include "hardware/scale/Scale.h"
#include "hardware/display/Display.h"

#define DISPLAY_CLK_PIN 12 //D6 GPIO12
#define DISPLAY_DIO_PIN 13 //D7 GPIO13

ConfigurationManager configurationManager;
Scale scale;
Display display(DISPLAY_CLK_PIN, DISPLAY_DIO_PIN);

void setup() {
    Serial.begin(57600);
    display.displayValue(8888);
    configurationManager.setup();
    scale.setup();
}

void loop() {
    configurationManager.run();
    // Uncomment this line to configure the device first time. It calls delay() which makes it impossible to connect to the AP
    scale.run();

    int cans = round(scale.getAmountOfCans());
    display.displayValue(cans);

    Serial.println("Amount of cans currently in the bin: " + String(cans));
}