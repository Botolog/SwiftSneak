bool EN_MOTORS = false, EN_DISPLAY = false, EN_RGB = false, EN_GYRO = false, EN_IR = false, EN_WIFI = false, EN_SCANNER = false, EN_SERVO = false;
//* Motors
#define PinRFplus 12
#define PinRFminus 13
#define PinLFplus 32
#define PinLFminus 33
#define PinRBplus 14
#define PinRBminus 5
#define PinLBplus 34
#define PinLBminus 35

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

void setupMotors()
{
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

void SetSpeed(bool right, bool front, float speed = 0)
{
  speed = max(min((speed + 100) / 200 * 255, 255.0F), 0.0F);

  if (right && front)
  {
    ledcWrite(RFplus, speed);
    ledcWrite(RFminus, 255 - speed);
  }
  else if (right && !front)
  {
    ledcWrite(RBplus, speed);
    ledcWrite(RBminus, 255 - speed);
  }
  else if (!right && front)
  {
    ledcWrite(LFplus, speed);
    ledcWrite(LFminus, 255 - speed);
  }
  else if (!right && !front)
  {
    ledcWrite(LBplus, speed);
    ledcWrite(LBminus, 255 - speed);
  }
}

double B(double x){
  min(100, max(-100, x));
}

void drive(double forward, double right, double turn)
{
  SetSpeed(false, true, B((forward+right+turn)/(abs(forward)+abs(right)+abs(turn))));
  SetSpeed(true, true, B((forward-right-turn)/(abs(forward)+abs(right)+abs(turn))));
  SetSpeed(false, false, B((forward-right+turn)/(abs(forward)+abs(right)+abs(turn))));
  SetSpeed(true, false, B((forward+right-turn)/(abs(forward)+abs(right)+abs(turn))));

}

void stop()
{
  ledcWrite(RFplus, 0);
  ledcWrite(RFminus, 0);
  ledcWrite(RBplus, 0);
  ledcWrite(RBminus, 0);
  ledcWrite(LFplus, 0);
  ledcWrite(LFminus, 0);
  ledcWrite(LBplus, 0);
  ledcWrite(LBminus, 0);
}