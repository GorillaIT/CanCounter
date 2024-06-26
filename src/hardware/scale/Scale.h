//
// Created by Stein.Jonker on 26/06/2024.
//

#ifndef CANCOUNTER_SCALE_H
#define CANCOUNTER_SCALE_H

#include "HX711.h"

class Scale {
private:
    HX711 scale;
    // Average weight of a can in grams
    const float AVERAGE_CAN_WEIGHT = 13;
public:
    void setup();

    void run();

    float getAmountOfCans();
};


#endif //CANCOUNTER_SCALE_H
