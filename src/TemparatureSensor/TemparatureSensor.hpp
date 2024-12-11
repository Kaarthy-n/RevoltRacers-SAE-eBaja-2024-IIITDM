// File: TemperatureSensor/TemperatureSensor.hpp
#ifndef TEMPERATURESENSOR_HPP
#define TEMPERATURESENSOR_HPP

#include <DallasTemperature.h>
#include <OneWire.h>

class TemperatureSensor {
public:
    TemperatureSensor(int motorPin, int mcuPin);
    void begin();
    float getMotorTemperature();
    float getMCUTemperature();

private:
    OneWire oneWireMotor;
    OneWire oneWireMCU;
    DallasTemperature sensorsMotor;
    DallasTemperature sensorsMCU;
};

#endif
