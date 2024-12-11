#include "TemperatureSensor/TemperatureSensor.hpp"
#include "MotorSpeed/MotorSpeed.hpp"
#include "Display/Display.hpp"
#include "SDCard/SDCard.hpp"
#include "MPU6050Sensor/MPU6050Sensor.hpp"
#include "CurrentSensor/CurrentSensor.hpp"

TemperatureSensor tempSensor(7, 8);
MotorSpeed motorSpeed(6);
Display display(0x3F, 16, 2);
SDCard sdCard(10);
MPU6050Sensor mpu6050;
CurrentSensor currentSensor(A0);

void setup() {
    // Initialize components
    tempSensor.begin();
    motorSpeed.begin();
    display.begin();
    sdCard.begin();
    mpu6050.begin();

    // Display initialization complete message
    display.showData(0, 0, 0, false);
    delay(2000);
}

void loop() {
    // Fetch sensor data
    float motorTemp = tempSensor.getMotorTemperature();
    float mcuTemp = tempSensor.getMCUTemperature();
    float speed = motorSpeed.getSpeed();
    float current = currentSensor.readCurrent();
    bool overcurrent = currentSensor.isOvercurrent();

    int16_t ax, ay, az, gx, gy, gz;
    mpu6050.getMotion(ax, ay, az, gx, gy, gz);

    // Display data on LCD
    display.showData(speed, motorTemp, mcuTemp, overcurrent);

    // Log data to SD card
    unsigned long currentTime = millis();
    sdCard.logData(currentTime, ax, ay, az, gx, gy, gz, speed, motorTemp, mcuTemp);

    // Add delay for sensor sampling rate
    delay(100);
}
