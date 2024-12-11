#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Adafruit_MPU6050.h>
#include <SD.h>

#define CURRENT_OUTPUT A0
#define START_STOP_BTN 2
#define ACCELERATOR_PIN 3
#define BRAKE_PIN 4
#define NEUTRAL_PIN 5
#define TSAL_PIN 13
#define HALL_PIN 6
#define TEMP_MOTOR_PIN 7
#define TEMP_MCU_PIN 8

#define CHIP_SELECT 10

LiquidCrystal_I2C lcd(0x3F, 16, 2);
OneWire oneWireMotor(TEMP_MOTOR_PIN);
DallasTemperature sensorsMotor(&oneWireMotor);

OneWire oneWireMCU(TEMP_MCU_PIN);
DallasTemperature sensorsMCU(&oneWireMCU);

Adafruit_MPU6050 mpu;
File logfile;

bool isVehicleRunning = false;
unsigned long lastPulseTime = 0;
float motorSpeed = 0.0;
float currentReading = 0.0;
float motorTemperature = 0.0;
float mcuTemperature = 0.0;
bool currentWarning = false;

void setup() {
  pinMode(START_STOP_BTN, INPUT);
  pinMode(ACCELERATOR_PIN, INPUT);
  pinMode(BRAKE_PIN, INPUT);
  pinMode(NEUTRAL_PIN, INPUT);
  pinMode(TSAL_PIN, OUTPUT);
  pinMode(HALL_PIN, INPUT);

  digitalWrite(TSAL_PIN, LOW);

  lcd.begin(16, 2);
  lcd.print("eBAJA Vehicle");

  sensorsMotor.begin();
  sensorsMCU.begin();

  Wire.begin();

  if (!mpu.begin();) {
    lcd.clear();
    lcd.print("MPU6050 Error!");
    while (1);
  }

  if (!SD.begin(CHIP_SELECT)) {
    lcd.clear();
    lcd.print("SD Error!");
    while (1);
  }

  logfile = SD.open("sensor_data.txt", FILE_WRITE);
  if (logfile) {
    logfile.println("Time, Accel X, Accel Y, Accel Z, Gyro X, Gyro Y, Gyro Z, Speed, Motor Temp, MCU Temp");
    logfile.close();
  } else {
    lcd.clear();
    lcd.print("Error opening file");
    while (1);
  }
}

void loop() {
  bool isNeutral = digitalRead(NEUTRAL_PIN);
  bool isAcceleratorPressed = digitalRead(ACCELERATOR_PIN);
  bool isBrakePressed = digitalRead(BRAKE_PIN);
  bool isStartStopPressed = digitalRead(START_STOP_BTN);

  if (!isVehicleRunning) {
    if (isNeutral) {
      if (isAcceleratorPressed) {
        if (isBrakePressed && isStartStopPressed) {
          isVehicleRunning = true;
          digitalWrite(TSAL_PIN, HIGH);
        }
      }
    }
  } else {
    digitalWrite(TSAL_PIN, HIGH);
  }

  motorSpeed = calculateMotorSpeed();
  currentReading = analogRead(CURRENT_OUTPUT);
  motorTemperature = getTemperature(TEMP_MOTOR_PIN);
  mcuTemperature = getTemperature(TEMP_MCU_PIN);

  if (currentReading > 500) {
    currentWarning = true;
  } else {
    currentWarning = false;
  }

  int16_t ax, ay, az, gx, gy, gz;
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  logfile = SD.open("sensor_data.txt", FILE_WRITE);
  if (logfile) {
    unsigned long currentTime = millis();
    logfile.print(currentTime); logfile.print(", ");
    logfile.print(ax); logfile.print(", ");
    logfile.print(ay); logfile.print(", ");
    logfile.print(az); logfile.print(", ");
    logfile.print(gx); logfile.print(", ");
    logfile.print(gy); logfile.print(", ");
    logfile.print(gz); logfile.print(", ");
    logfile.print(motorSpeed); logfile.print(", ");
    logfile.print(motorTemperature); logfile.print(", ");
    logfile.println(mcuTemperature);
    logfile.close();
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
