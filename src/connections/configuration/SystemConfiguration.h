//
// Created by Stein.Jonker on 26/06/2024.
//

#ifndef CANCOUNTER_SYSTEMCONFIGURATION_H
#define CANCOUNTER_SYSTEMCONFIGURATION_H
struct SystemConfiguration {
    String ssid;
    String password;
    // Average weight of grams of a can, 13.7g
    float averageCanWeightGrams;
};

#endif //CANCOUNTER_SYSTEMCONFIGURATION_H
