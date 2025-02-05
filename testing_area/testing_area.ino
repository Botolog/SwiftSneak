//* Scanner (Servo+TFL)
#include <TFLI2C.h>

TFLI2C tflI2C;
int16_t tfDist;               // distance in centimeters
int16_t tfAddr = ; // Use this default I2C address




int16_t mesure()
{
  
  
  delay(10);
  tflI2C.getData(tfDist, tfAddr);
  // Serial.println(tfDist);
  return tfDist;
}


void setup(){
  Serial.begin(115200);
}

void loop(){
  int x = mesure();
  Serial.println(x);
  Serial.println(mesure());
}
