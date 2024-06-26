//
// Created by Stein.Jonker on 13/06/2024.
//
/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/arduino-load-cell-hx711/

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*/


#include <Arduino.h>


#include "HX711.h"
#include "connections/configuration/ConfigurationManager.h"

#define LOADCELL_DOUT_PIN  4    // D2, GPIO4
#define LOADCELL_SCK_PIN  2     // D4, GPIO2


HX711 scale;
ConfigurationManager configurationManager;

void setup() {
    Serial.begin(57600);
//    configurationManager.setup();

    scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
}

void loop() {
 //    configurationManager.run();

  if (scale.is_ready()) {
      scale.set_scale();
      Serial.println("Tare... remove any weights from the scale.");
      delay(5000);
      scale.tare();
      Serial.println("Tare done...");
      Serial.print("Place a known weight on the scale...");
      delay(5000);
      long reading = scale.get_units(10);
      Serial.print("Result: ");
      Serial.println(reading);
  }
  else {
      Serial.println("HX711 not found.");
  }

  delay(1000);
}

//calibration factor will be the (reading)/(known weight)
