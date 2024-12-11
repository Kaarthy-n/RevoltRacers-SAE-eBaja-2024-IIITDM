#include "Display.hpp"

Display::Display(uint8_t address, uint8_t cols, uint8_t rows) : lcd(address, cols, rows) {}

void Display::begin() {
    lcd.begin();
    lcd.print("Initializing...");
}

void Display::showData(float speed, float motorTemp, float mcuTemp, bool currentWarning) {
    lcd.clear();
    if (currentWarning) {
        lcd.setCursor(0, 0);
        lcd.print("WARNING: Current!");
    } else {
        lcd.setCursor(0, 0);
        lcd.print("Speed: ");
        lcd.print(speed);
        lcd.print(" RPM");
    }
    lcd.setCursor(0, 1);
    lcd.print("Motor: ");
    lcd.print(motorTemp);
    lcd.print("C MCU: ");
    lcd.print(mcuTemp);
    lcd.print("C");
}
