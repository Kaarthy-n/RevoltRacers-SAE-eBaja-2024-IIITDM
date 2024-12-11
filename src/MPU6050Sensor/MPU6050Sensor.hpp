#ifndef MPU6050SENSOR_HPP
#define MPU6050SENSOR_HPP

#include <MPU6050.h>

class MPU6050Sensor {
public:
    MPU6050Sensor();
    void begin();
    void getMotion(int16_t &ax, int16_t &ay, int16_t &az, int16_t &gx, int16_t &gy, int16_t &gz);

private:
    MPU6050 mpu;
};

#endif
