// =============================
// INDUSTRIAL MOTOR CONTROL SYSTEM
// ESP32 + Relay + MQ2 + NTC + E-STOP
// =============================

#define RELAY  5        // Motor Relay
#define BUZZER  15

// Buttons
#define START_BTN  18
#define STOP_BTN   19
#define ESTOP_BTN  21

// LEDs
#define LED_MOTOR  2
#define LED_FAULT  4

// Sensors
#define MQ2_PIN     34
#define TEMP_PIN    35     // NTC OUT pin

// Thresholds
int GAS_LIMIT = 2200;
int TEMP_LIMIT = 1500;

bool motorState = false;
bool faultState = false;

void setup() {
  Serial.begin(115200);

  pinMode(RELAY, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  pinMode(LED_MOTOR, OUTPUT);
  pinMode(LED_FAULT, OUTPUT);

  pinMode(START_BTN, INPUT_PULLUP);
  pinMode(STOP_BTN, INPUT_PULLUP);
  pinMode(ESTOP_BTN, INPUT_PULLUP);

  digitalWrite(RELAY, LOW);
  digitalWrite(LED_MOTOR, LOW);
  digitalWrite(LED_FAULT, LOW);
}

void loop() {

  int gasValue = analogRead(MQ2_PIN);
  int tempValue = analogRead(TEMP_PIN);

  Serial.print("Gas: ");
  Serial.print(gasValue);
  Serial.print(" | Temp: ");
  Serial.println(tempValue);

  // EMERGENCY STOP
  if (digitalRead(ESTOP_BTN) == LOW) {
    motorOFF();
    faultON();
  }

  // GAS Fault
  if (gasValue > GAS_LIMIT) {
    motorOFF();
    faultON();
  }

  // Temperature Fault
  if (tempValue > TEMP_LIMIT) {
    motorOFF();
    faultON();
  }

  // START Button
  if (!faultState && digitalRead(START_BTN) == LOW) {
    motorON();
  }

  // STOP Button
  if (digitalRead(STOP_BTN) == LOW) {
    motorOFF();
  }

  delay(100);
}

// ========================
// FUNCTIONS
// ========================

void motorON() {
  motorState = true;
  digitalWrite(RELAY, HIGH);
  digitalWrite(LED_MOTOR, HIGH);
}

void motorOFF() {
  motorState = false;
  digitalWrite(RELAY, LOW);
  digitalWrite(LED_MOTOR, LOW);
}

void faultON() {
  faultState = true;
  digitalWrite(LED_FAULT, HIGH);

  // buzzer alarm
  for (int i = 0; i < 3; i++) {
    digitalWrite(BUZZER, HIGH);
    delay(150);
    digitalWrite(BUZZER, LOW);
    delay(150);
  }
}
