//
// Created by Stein.Jonker on 26/06/2024.
//

#include "Display.h"

Display::Display(int clkPin, int dioPin) {
    display = new TM1637Display(clkPin, dioPin);
    // Set the brightness to maximum
    display->setBrightness(0x0f);
}

void Display::displayCans(int cans) {
    display->showNumberDec(cans, false);
}