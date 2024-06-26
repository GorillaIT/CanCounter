//
// Created by Stein.Jonker on 26/06/2024.
//

#ifndef CANCOUNTER_DISPLAY_H
#define CANCOUNTER_DISPLAY_H

#include <TM1637Display.h>

class Display {
    private:
        TM1637Display* display;
    public:
        Display(int clkPin, int dioPin);
        void displayValue(int cans);
};


#endif //CANCOUNTER_DISPLAY_H
