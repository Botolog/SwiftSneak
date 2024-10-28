void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
}

int i = 0;
void loop() {
  // put your main code here, to run repeatedly:
  i++;
  Serial.println("Hello World!");
  Serial.println(i);
  delay(1000);
  Serial.println("Hello World!");
  Serial.println(i);
  delay(1000);
}
