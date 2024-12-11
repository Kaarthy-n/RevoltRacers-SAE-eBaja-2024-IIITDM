#include "SDCard.hpp"
#include <Arduino.h>

SDCard::SDCard(int chipSelectPin) : chipSelectPin(chipSelectPin) {}

void SDCard::begin() {
    if (!SD.begin(chipSelectPin)) {
        while (1);
    }
}

void SDCard::logData(unsigned long time, int16_t ax, int16_t ay, int16_t az, int16_t gx, int16_t gy, int16_t gz, float speed, float motorTemp, float mcuTemp) {
    File dataFile = SD.open("sensor_data.txt", FILE_WRITE);
    if (dataFile) {
        dataFile.print(time); dataFile.print(", ");
        dataFile.print(ax); dataFile.print(", ");
        dataFile.print(ay); dataFile.print(", ");
        dataFile.print(az); dataFile.print(", ");
        dataFile.print(gx); dataFile.print(", ");
        dataFile.print(gy); dataFile.print(", ");
        dataFile.print(gz); dataFile.print(", ");
        dataFile.print(speed); dataFile.print(", ");
        dataFile.print(motorTemp); dataFile.print(", ");
        dataFile.println(mcuTemp);
        dataFile.close();
    }
}
