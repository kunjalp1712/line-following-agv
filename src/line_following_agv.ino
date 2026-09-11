// ---------------- MOTOR DRIVER 1 (Front Motors) ----------------
// Front Left Motor
#define F_IN1 25
#define F_IN2 33
#define F_ENA 32  // Front Left PWM 

// Front Right Motor
#define F_IN3 27
#define F_IN4 26
#define F_ENB 14  // Front Right PWM 

// ---------------- MOTOR DRIVER 2 (Rear Motors) ----------------
// Rear Left Motor
#define R_IN1 19
#define R_IN2 18
#define R_ENA 22  // Changed from 21 → 22

// Rear Right Motor
#define R_IN3 13  // Changed from 5 → 13
#define R_IN4 17
#define R_ENB 15  // Changed from 16 → 15

// ---------------- IR SENSORS ----------------
#define IR_LEFT  34
#define IR_RIGHT 35

// ---------------- PWM SETTINGS ----------------
#define PWM_FREQ       5000
#define PWM_RESOLUTION 8

// ---------------- SPEED TUNING ----------------
#define BASE_SPEED     150
#define CURVE_SPEED    120

#define BLACK HIGH   // Change to LOW if your sensor is inverted
#define WHITE LOW    // Change to HIGH if your sensor is inverted

void setup() {
  Serial.begin(115200);

  int outputPins[] = {F_IN1, F_IN2, F_IN3, F_IN4,
                      R_IN1, R_IN2, R_IN3, R_IN4};
  for (int i = 0; i < 8; i++) {
    pinMode(outputPins[i], OUTPUT);
  }

  pinMode(IR_LEFT,  INPUT);
  pinMode(IR_RIGHT, INPUT);

  ledcAttach(F_ENA, PWM_FREQ, PWM_RESOLUTION);
  ledcAttach(F_ENB, PWM_FREQ, PWM_RESOLUTION);
  ledcAttach(R_ENA, PWM_FREQ, PWM_RESOLUTION);
  ledcAttach(R_ENB, PWM_FREQ, PWM_RESOLUTION);

  stopMotors();
  Serial.println("AGV Ready!");
}

void loop() {
  int leftSensor  = digitalRead(IR_LEFT);
  int rightSensor = digitalRead(IR_RIGHT);

  Serial.print("IR L:"); Serial.print(leftSensor);
  Serial.print(" | IR R:"); Serial.println(rightSensor);

  if      (leftSensor == HIGH && rightSensor == HIGH) moveForward();
  else if (leftSensor == LOW  && rightSensor == HIGH) smoothCurveRight();
  else if (leftSensor == HIGH && rightSensor == LOW)  smoothCurveLeft();
  else                                                stopMotors();
}

// ─── Helpers ───────────────────────────────────────────────
void setMotorsForward() {
  digitalWrite(F_IN1, HIGH); digitalWrite(F_IN2, LOW);
  digitalWrite(F_IN3, HIGH); digitalWrite(F_IN4, LOW);
  digitalWrite(R_IN1, HIGH); digitalWrite(R_IN2, LOW);
  digitalWrite(R_IN3, HIGH); digitalWrite(R_IN4, LOW);
}

void setSpeeds(int leftSpeed, int rightSpeed) {
  ledcWrite(F_ENA, leftSpeed);
  ledcWrite(R_ENA, leftSpeed);
  ledcWrite(F_ENB, rightSpeed);
  ledcWrite(R_ENB, rightSpeed);
}

// ─── Movement Functions ────────────────────────────────────
void moveForward() {
  setMotorsForward();
  setSpeeds(BASE_SPEED, BASE_SPEED);
  Serial.println(">> Forward");
}

void smoothCurveLeft() {
  setMotorsForward();
  setSpeeds(CURVE_SPEED, BASE_SPEED);
  Serial.println("<< Curve Left");
}

void smoothCurveRight() {
  setMotorsForward();
  setSpeeds(BASE_SPEED, CURVE_SPEED);
  Serial.println(">> Curve Right");
}

void stopMotors() {
  digitalWrite(F_IN1, LOW); digitalWrite(F_IN2, LOW);
  digitalWrite(F_IN3, LOW); digitalWrite(F_IN4, LOW);
  digitalWrite(R_IN1, LOW); digitalWrite(R_IN2, LOW);
  digitalWrite(R_IN3, LOW); digitalWrite(R_IN4, LOW);
  setSpeeds(0, 0);
  Serial.println("|| Stop");
}
