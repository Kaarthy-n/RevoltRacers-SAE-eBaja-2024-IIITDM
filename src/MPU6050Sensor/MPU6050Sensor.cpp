#include "MPU6050Sensor.hpp"

MPU6050Sensor::MPU6050Sensor() : mpu() {}

void MPU6050Sensor::begin() {
    mpu.initialize();
    if (!mpu.testConnection()) {
        while (1);
    }
}

void MPU6050Sensor::getMotion(int16_t &ax, int16_t &ay, int16_t &az, int16_t &gx, int16_t &gy, int16_t &gz) {
    mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
}
