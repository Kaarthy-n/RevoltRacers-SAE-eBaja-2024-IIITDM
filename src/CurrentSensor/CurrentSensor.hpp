#ifndef CURRENTSENSOR_HPP
#define CURRENTSENSOR_HPP

class CurrentSensor {
public:
    CurrentSensor(int currentPin);
    float readCurrent();
    bool isOvercurrent();

private:
    int currentPin;
    float currentValue;
};

#endif
