#include "TemperatureSensor.hpp"

TemperatureSensor::TemperatureSensor(int motorPin, int mcuPin)
    : oneWireMotor(motorPin), oneWireMCU(mcuPin),
      sensorsMotor(&oneWireMotor), sensorsMCU(&oneWireMCU) {}

void TemperatureSensor::begin() {
    sensorsMotor.begin();
    sensorsMCU.begin();
}

float TemperatureSensor::getMotorTemperature() {
    sensorsMotor.requestTemperatures();
    return sensorsMotor.getTempCByIndex(0);
}

float TemperatureSensor::getMCUTemperature() {
    sensorsMCU.requestTemperatures();
    return sensorsMCU.getTempCByIndex(0);
}
