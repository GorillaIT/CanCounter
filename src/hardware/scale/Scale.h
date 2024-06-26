//
// Created by Stein.Jonker on 26/06/2024.
//

#ifndef CANCOUNTER_SCALE_H
#define CANCOUNTER_SCALE_H

#include "HX711.h"

class Scale {
private:
    HX711 scale;
public:
    void setup();

    void run();

    float convertWeightToCans(float weightInGrams) const;

    float getWeight();
};


#endif //CANCOUNTER_SCALE_H
