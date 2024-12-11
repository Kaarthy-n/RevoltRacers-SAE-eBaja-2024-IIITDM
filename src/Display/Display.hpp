#ifndef DISPLAY_HPP
#define DISPLAY_HPP

#include <LiquidCrystal_I2C.h>

class Display {
public:
    Display(uint8_t address, uint8_t cols, uint8_t rows);
    void begin();
    void showData(float speed, float motorTemp, float mcuTemp, bool currentWarning);

private:
    LiquidCrystal_I2C lcd;
};

#endif
