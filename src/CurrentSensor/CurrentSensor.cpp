#include "CurrentSensor.hpp"
#include <Arduino.h>

CurrentSensor::CurrentSensor(int currentPin) : currentPin(currentPin), currentValue(0.0) {}

float CurrentSensor::readCurrent() {
    currentValue = analogRead(currentPin) * (5.0 / 1023.0); // Example conversion
    return currentValue;
}

bool CurrentSensor::isOvercurrent() {
    return currentValue > 500; // Example threshold
}
