bool EN_MOTORS = false, EN_DISPLAY = false, EN_RGB = false, EN_GYRO = false, EN_IR = false, EN_WIFI = false, EN_SCANNER = false, EN_SERVO = false;
//* Motors
#define PinRFplus 18
#define PinRFminus 17
#define PinLFplus 15
#define PinLFminus 16
#define PinRBplus 14
#define PinRBminus 23
#define PinLBplus 13
#define PinLBminus 12

#define RFplus 2
#define RFminus 3
#define LFplus 4
#define LFminus 5
#define RBplus 6
#define RBminus 7
#define LBplus 8
#define LBminus 9

const int freq = 3000;
const int resolution = 8;

void setupMotors() {
  EN_MOTORS = true;
  ledcSetup(RFplus, freq, resolution);
  ledcSetup(RFminus, freq, resolution);
  ledcSetup(LFplus, freq, resolution);
  ledcSetup(LFminus, freq, resolution);
  ledcSetup(RBplus, freq, resolution);
  ledcSetup(RBminus, freq, resolution);
  ledcSetup(LBplus, freq, resolution);
  ledcSetup(LBminus, freq, resolution);
  ledcAttachPin(PinRFplus, RFplus);
  ledcAttachPin(PinRFminus, RFminus);
  ledcAttachPin(PinLFplus, LFplus);
  ledcAttachPin(PinLFminus, LFminus);
  ledcAttachPin(PinRBplus, RBplus);
  ledcAttachPin(PinRBminus, RBminus);
  ledcAttachPin(PinLBplus, LBplus);
  ledcAttachPin(PinLBminus, LBminus);
}

void SetSpeed(bool right, bool front, double speed = 0) {
  Serial.print(speed); Serial.print(" ==> ");
  speed = max(min(((speed) + 100) / 200 * 255, 255.0), 0.0);
  Serial.println(speed);

  if (right && front) {
    ledcWrite(RFplus, speed);
    ledcWrite(RFminus, 255 - speed);
  } else if (right && !front) {
    ledcWrite(RBplus, speed);
    ledcWrite(RBminus, 255 - speed);
  } else if (!right && front) {
    ledcWrite(LFplus, speed);
    ledcWrite(LFminus, 255 - speed);
  } else if (!right && !front) {
    ledcWrite(LBplus, speed);
    ledcWrite(LBminus, 255 - speed);
  }
}

double B(double x) {
  return min(100.0, max(-100.0, x));
}

void drive(double forward, double right, double turn) {
  forward /= 100; right /= 100; turn /= 100;
  SetSpeed(false, true,  100*B(forward + right + turn));
  SetSpeed(true, true,   100*B(forward - right - turn));
  SetSpeed(false, false, 100*B(forward - right + turn));
  SetSpeed(true, false,  100*B(forward + right - turn));
}

void stop() {
  ledcWrite(RFplus, 0);
  ledcWrite(RFminus, 0);
  ledcWrite(RBplus, 0);
  ledcWrite(RBminus, 0);
  ledcWrite(LFplus, 0);
  ledcWrite(LFminus, 0);
  ledcWrite(LBplus, 0);
  ledcWrite(LBminus, 0);
}


//* Main
void setup() {
  setupMotors();
  // stop();
  Serial.begin(115200);
  Serial.println("Ready! Starting...");
  delay(500);
}

void loop() {
  Serial.println("startDrive");
  Serial.println(abs(-1));
  Serial.println(B(10.0));
  delay(2000);
  Serial.println("===============");
  drive(50.0, 0, 0);
  delay(1000);
  Serial.println("===============");
  drive(0, 50.0, 0);
  delay(1000);
  Serial.println("===============");
  drive(0, 0, 50.0);
  delay(1000);
  Serial.println("===============");
  drive(0, 0, -50.0);
  delay(1000);
  Serial.println("===============");
  drive(0, -50.0, 0);
  delay(1000);
  Serial.println("===============");
  drive(-50, 0, 0);
  delay(1000);
  Serial.println("===============");
  stop();
}