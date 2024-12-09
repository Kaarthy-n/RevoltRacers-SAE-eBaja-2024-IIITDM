#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <MPU6050.h>
#include <SD.h>

const int current_op = A0;
const int start_stop_btn = 2;
const int accelerator_pin = 3;
const int brake_pin = 4;
const int neutral_pin = 5;
const int tsal_pin = 13;
const int hall_pin = 6;
const int temp_motor_pin = 7;
const int temp_mcu_pin = 8;

const int chipSelect = 10;

LiquidCrystal_I2C lcd(0x3F, 16, 2);
OneWire oneWireMotor(temp_motor_pin);
DallasTemperature sensorsMotor(&oneWireMotor);

OneWire oneWireMCU(temp_mcu_pin);
DallasTemperature sensorsMCU(&oneWireMCU);

MPU6050 mpu;
File dataFile;

bool isVehicleRunning = false;
unsigned long lastPulseTime = 0;
float motorSpeed = 0.0;
float currentReading = 0.0;
float motorTemperature = 0.0;
float mcuTemperature = 0.0;
bool currentWarning = false;

void setup() {
  pinMode(start_stop_btn, INPUT);
  pinMode(accelerator_pin, INPUT);
  pinMode(brake_pin, INPUT);
  pinMode(neutral_pin, INPUT);
  pinMode(tsal_pin, OUTPUT);
  pinMode(hall_pin, INPUT);

  digitalWrite(tsal_pin, LOW);

  lcd.begin(16, 2);
  lcd.print("eBAJA Vehicle");

  sensorsMotor.begin();
  sensorsMCU.begin();

  Wire.begin();
  mpu.initialize();
  if (!mpu.testConnection()) {
    lcd.clear();
    lcd.print("MPU6050 Error!");
    while (1);
  }

  if (!SD.begin(chipSelect)) {
    lcd.clear();
    lcd.print("SD Error!");
    while (1);
  }

  dataFile = SD.open("sensor_data.txt", FILE_WRITE);
  if (dataFile) {
    dataFile.println("Time, Accel X, Accel Y, Accel Z, Gyro X, Gyro Y, Gyro Z, Speed, Motor Temp, MCU Temp");
    dataFile.close();
  } else {
    lcd.clear();
    lcd.print("Error opening file");
    while (1);
  }
}

void loop() {
  bool isNeutral = digitalRead(neutral_pin);
  bool isAcceleratorPressed = digitalRead(accelerator_pin);
  bool isBrakePressed = digitalRead(brake_pin);
  bool isStartStopPressed = digitalRead(start_stop_btn);

  if (!isVehicleRunning) {
    if (isNeutral) {
      if (isAcceleratorPressed) {
        if (isBrakePressed && isStartStopPressed) {
          isVehicleRunning = true;
          digitalWrite(tsal_pin, HIGH);
        }
      }
    }
  } else {
    digitalWrite(tsal_pin, HIGH);
  }

  motorSpeed = calculateMotorSpeed();
  currentReading = analogRead(current_op);
  motorTemperature = getTemperature(temp_motor_pin);
  mcuTemperature = getTemperature(temp_mcu_pin);

  if (currentReading > 500) {
    currentWarning = true;
  } else {
    currentWarning = false;
  }

  int16_t ax, ay, az, gx, gy, gz;
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  dataFile = SD.open("sensor_data.txt", FILE_WRITE);
  if (dataFile) {
    unsigned long currentTime = millis();
    dataFile.print(currentTime); dataFile.print(", ");
    dataFile.print(ax); dataFile.print(", ");
    dataFile.print(ay); dataFile.print(", ");
    dataFile.print(az); dataFile.print(", ");
    dataFile.print(gx); dataFile.print(", ");
    dataFile.print(gy); dataFile.print(", ");
    dataFile.print(gz); dataFile.print(", ");
    dataFile.print(motorSpeed); dataFile.print(", ");
    dataFile.print(motorTemperature); dataFile.print(", ");
    dataFile.println(mcuTemperature);
    dataFile.close();
  }

  displayData();
  delay(100);
}

float calculateMotorSpeed() {
  static unsigned long lastTime = 0;
  static unsigned long pulseCount = 0;

  if (digitalRead(hall_pin) == HIGH) {
    unsigned long currentTime = millis();
    if (currentTime - lastTime > 1000) {
      motorSpeed = (pulseCount * 60.0) / ((currentTime - lastTime) / 1000.0);
      pulseCount = 0;
      lastTime = currentTime;
    }
    pulseCount++;
  }
  return motorSpeed;
}

float getTemperature(int pin) {
  sensorsMotor.requestTemperatures();
  sensorsMCU.requestTemperatures();
  if (pin == temp_motor_pin) {
    return sensorsMotor.getTempCByIndex(0);
  }
  return sensorsMCU.getTempCByIndex(0);
}

void displayData() {
  lcd.clear();
  if (currentWarning) {
    lcd.setCursor(0, 0);
    lcd.print("WARNING: Current!");
  } else {
    lcd.setCursor(0, 0);
    lcd.print("Speed: ");
    lcd.print(motorSpeed);
    lcd.print(" RPM");
  }

  lcd.setCursor(0, 1);
  lcd.print("Motor: ");
  lcd.print(motorTemperature);
  lcd.print("C ");
  lcd.print("MCU: ");
  lcd.print(mcuTemperature);
  lcd.print("C");
}
