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

#define LOADCELL_DOUT_PIN  2    // D4, GPIO2
#define LOADCELL_SCK_PIN  0     // D3, GPIO0


HX711 scale;
ConfigurationManager configurationManager;

// A samsung galaxy S21 Ultra weighs 229g and we got a reading of 149470
// Use .0 at the end of the numbers to force the compiler to use floating point math
float calibration_factor = 149470.0 / 229.0;
// On average a can = 12.8g

void setup() {
    Serial.begin(57600);
//    configurationManager.setup();

    scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);

    Serial.println("Before setting up the scale:");
    Serial.print("read: \t\t");
    Serial.println(scale.read());			// print a raw reading from the ADC

    Serial.print("read average: \t\t");
    Serial.println(scale.read_average(20));  	// print the average of 20 readings from the ADC

    Serial.print("get value: \t\t");
    Serial.println(scale.get_value(5));		// print the average of 5 readings from the ADC minus the tare weight (not set yet)

    Serial.print("get units: \t\t");
    Serial.println(scale.get_units(5), 1);	// print the average of 5 readings from the ADC minus tare weight (not set) divided
    // by the SCALE parameter (not set yet)

    scale.set_scale(calibration_factor);                      // this value is obtained by calibrating the scale with known weights; see the README for details
    scale.tare();				                        // reset the scale to 0

    Serial.println("After setting up the scale:");

    Serial.print("read: \t\t");
    Serial.println(scale.read());                 // print a raw reading from the ADC

    Serial.print("read average: \t\t");
    Serial.println(scale.read_average(20));       // print the average of 20 readings from the ADC

    Serial.print("get value: \t\t");
    Serial.println(scale.get_value(5));		// print the average of 5 readings from the ADC minus the tare weight, set with tare()

    Serial.print("get units: \t\t");
    Serial.println(scale.get_units(5), 1);        // print the average of 5 readings from the ADC minus tare weight, divided
    // by the SCALE parameter set with set_scale

    Serial.println("Readings:");
}

void loop() {
    //    configurationManager.run();
    Serial.print("one reading:\t");
    Serial.print(scale.get_units(), 1);
    Serial.print("\t| average:\t");
    Serial.println(scale.get_units(10), 1);

    scale.power_down();			        // put the ADC in sleep mode
    delay(1000);
    scale.power_up();
}