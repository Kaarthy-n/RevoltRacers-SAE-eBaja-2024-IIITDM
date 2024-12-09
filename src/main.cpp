const int current_op = A0;
const int start_stop_btn = 2;
const int accelerator_pin = 3;
const int brake_pin = 4;
const int neutral_pin = 5;
const int tsal_pin = 13;

bool isVehicleRunning = false;

void setup() {
  pinMode(start_stop_btn, INPUT);
  pinMode(accelerator_pin, INPUT);
  pinMode(brake_pin, INPUT);
  pinMode(neutral_pin, INPUT);
  pinMode(tsal_pin, OUTPUT);
  digitalWrite(tsal_pin, LOW);
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
}
