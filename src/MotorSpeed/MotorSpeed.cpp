#include "MotorSpeed.hpp"
#include <Arduino.h>

MotorSpeed::MotorSpeed(int hallPin) : hallPin(hallPin), pulseCount(0), lastTime(0), speed(0.0) {}

void MotorSpeed::begin() {
    pinMode(hallPin, INPUT);
    attachInterrupt(digitalPinToInterrupt(hallPin), [this]() { this->handlePulse(); }, RISING);
}

void MotorSpeed::handlePulse() {
    pulseCount++;
}

float MotorSpeed::getSpeed() {
    unsigned long currentTime = millis();
    if (currentTime - lastTime >= 1000) {
        speed = (pulseCount * 60.0) / ((currentTime - lastTime) / 1000.0);
        pulseCount = 0;
        lastTime = currentTime;
    }
    return speed;
}
