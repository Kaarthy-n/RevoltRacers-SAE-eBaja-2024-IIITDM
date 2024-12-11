#ifndef MOTORSPEED_HPP
#define MOTORSPEED_HPP

class MotorSpeed {
public:
    MotorSpeed(int hallPin);
    void begin();
    float getSpeed();
    void handlePulse();

private:
    int hallPin;
    volatile unsigned long pulseCount;
    unsigned long lastTime;
    float speed;
};

#endif
