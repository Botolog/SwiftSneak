//* Scanner (Servo+TFL)
#include <TFLI2C.h>
#include "Wire.h"    

TFLI2C tflI2C;
int16_t tfDist;               // distance in centimeters
int16_t tfAddr = 0x10; // Use this default I2C address


int16_t mesure()
{
  delay(10);
  tflI2C.getData(tfDist, tfAddr);
  Serial.println(tfDist);
  return tfDist;
}

void setup(){
  Wire.begin();
  Serial.begin(115200);
  mesure();

}

void loop(){
  mesure();
  delay(1000);
}