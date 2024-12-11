#ifndef SDCARD_HPP
#define SDCARD_HPP

#include <SD.h>

class SDCard {
public:
    SDCard(int chipSelectPin);
    void begin();
    void logData(unsigned long time, int16_t ax, int16_t ay, int16_t az, int16_t gx, int16_t gy, int16_t gz, float speed, float motorTemp, float mcuTemp);

private:
    int chipSelectPin;
};

#endif
