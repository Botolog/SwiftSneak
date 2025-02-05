bool EN_MOTORS = false, EN_DISPLAY = false, EN_RGB = false, EN_GYRO = false, EN_IR = false, EN_WIFI = false, EN_SCANNER = false, EN_SERVO = false;
//* Motors
#define PinRFplus 18
#define PinRFminus 17
#define PinLFplus 15
#define PinLFminus 16
#define PinRBplus 14
#define PinRBminus 23
#define PinLBplus 27
#define PinLBminus 12

// #define RFplus 7
// #define RFminus 1
// #define LFplus 2
// #define LFminus 3
// #define RBplus 8
// #define RBminus 5
// #define LBplus 6
// #define LBminus 4

const int freq = 3000;
const int resolution = 8;

void setupMotors() {
  EN_MOTORS = true;
  pinMode(PinRFplus, OUTPUT);
  pinMode(PinRFminus, OUTPUT);
  pinMode(PinLFplus, OUTPUT);
  pinMode(PinLFminus, OUTPUT);
  pinMode(PinRBplus, OUTPUT);
  pinMode(PinRBminus, OUTPUT);
  pinMode(PinLBplus, OUTPUT);
  pinMode(PinLBminus, OUTPUT);
}

void SetSpeed(bool right, bool front, double speed = 0) {
  double res = 1024;
  Serial.print(speed); Serial.print(" ==> ");
  speed = max(min(((speed) + 100) / 200 * res, res), 0.0);
  // speed = (((speed) + 100) / 200 );
  Serial.println(speed);

  
  Serial.print(front ? "Front - " : "Back - ");
  Serial.println(right ? "Right" : "Left");

  if (right && front) {
    Serial.println("RF+");
    analogWrite(PinRFplus, speed);
    Serial.println("RF-");
    analogWrite(PinRFminus, res - speed);
  } else if (right && !front) {
    analogWrite(PinRBplus, speed);
    analogWrite(PinRBminus, res - speed);
  } else if (!right && front) {
    analogWrite(PinLFplus, speed);
    analogWrite(PinLFminus, res - speed);
  } else if (!right && !front) {
    analogWrite(PinLBplus, speed);
    analogWrite(PinLBminus, res - speed);
  }
  Serial.println("done");
  Serial.println("========================================");

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
  analogWrite(PinRFplus, 0);
  analogWrite(PinRFminus, 0);
  analogWrite(PinRBplus, 0);
  analogWrite(PinRBminus, 0);
  analogWrite(PinLFplus, 0);
  analogWrite(PinLFminus, 0);
  analogWrite(PinLBplus, 0);
  analogWrite(PinLBminus, 0);
}


//* Servos
// #include <Servo.h>
// #include <ESP32PWM.h>
// #include <ESP32Servo.h>


Servo Xservo;
Servo Yservo;

void setupServo()
{
  EN_SERVO = true;
  // pinMode(25, OUTPUT);
  Xservo.attach(32);
  Yservo.attach(33);
  Yservo.write(0);
  Xservo.write(0);
}

int XservoPos = 0, YservoPos = 0;

void targetServo(int Xtarget, int Ytarget)
{
  while (abs(Xtarget - XservoPos) + abs(Ytarget - YservoPos) > 2)
  {
    int dx = abs(Xtarget - XservoPos) / (Xtarget - XservoPos);
    int dy = abs(Ytarget - YservoPos) / (Ytarget - YservoPos);
    Xservo.write(XservoPos + dx);
    XservoPos = XservoPos + dx;
    Yservo.write(YservoPos + dy);
    YservoPos = YservoPos + dy;
    delay(20);
  }
}

// void scan()
// {
//   String toSend[180];
//   Xservo.write(XServo_position * K);
//   for (int YServo_position = 0; YServo_position < 180; YServo_position += dy)
//   {
//     if (true)
//     { //(XServo_position < 90){
//       for (XServo_position = 0; XServo_position < 180; XServo_position += dx)
//       {
//         Xservo.write(XServo_position * K);
//         xdots[XServo_position] = XServo_position;
//       }
//     }
//     else
//     {
//       for (XServo_position = 180; XServo_position > 0; XServo_position -= dx)
//       {
//         Xservo.write(XServo_position * K);
//         xdots[XServo_position] = mesure();
//       }
//     }
//     Yservo.write(YServo_position);
//     Xservo.write(0);
//     // uploadScan(YServo_position, xdots); 
//     // TODO uploadScan(YServo_position, xdots);
//     String toAdd = "";

//     delay(20);
//   }
  
// }

//* WiFi & HTTP
#include <WiFi.h>
#include <HTTPClient.h>

const char *ssid = "P2p";
const char *password = "";

WiFiServer server(80);

String BaseURL = "";

void setupWiFi()
{
  EN_WIFI = true;
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("");
  Serial.print("WiFi connected: ");
  Serial.println(WiFi.localIP());
  if (EN_DISPLAY)
  {
    // printOnDisplay(String(WiFi.localIP()));
  }
}

int contX;
int contY;
int camX;
int camY;
int rot;
int option;

void sep(String a)
{
  a.remove(0, 1);
  a.remove(a.length() - 1);

  // Split the string by ","
  int commaIndex1 = a.indexOf(',');
  String b = a.substring(0, commaIndex1); // Extract substring before the first comma

  // Find the second comma after the first one
  int commaIndex2 = a.indexOf(',', commaIndex1 + 1);
  String c = a.substring(commaIndex1 + 1, commaIndex2); // Extract substring between first and second comma

  // Find the third comma after the second one
  int commaIndex3 = a.indexOf(',', commaIndex2 + 1);
  String d = a.substring(commaIndex2 + 1, commaIndex3); // Extract substring between second and third comma

  // Find the fourth comma after the third one
  int commaIndex4 = a.indexOf(',', commaIndex3 + 1);
  String e = a.substring(commaIndex3 + 1, commaIndex4); // Extract substring between third and fourth comma

  // Find the fourth comma after the third one
  int commaIndex5 = a.indexOf(',', commaIndex4 + 1);
  String f = a.substring(commaIndex4 + 1, commaIndex5); // Extract substring between third and fourth comma
  String g = a.substring(commaIndex5 + 1);              // Extract substring after the fourth comma

  contX = b.toInt();
  contY = c.toInt();
  camX = d.toInt();
  camY = e.toInt();
  rot  = f.toInt();
  option = g.toInt();
}

void remoteCtrl()
{
  server.begin();
  String command = "N";
  WiFiClient client = server.available();
  Serial.println("rmtctrl started");
  if (client)
  {
    Serial.println("New client connected");
    while (client.connected() || command == "/")
    {
      if (client.available())
      {
        command = client.readStringUntil(';');
        if (command.indexOf("/") != -1)
        {
          Serial.println("Got IP: " + command);
          BaseURL = command;
        }
        Serial.println("Received command: " + command);
        sep(command);
        int speed = contY, shift = contX, Rot = rot;
        if (speed != 0 || shift != 0 || Rot != 0)
        {
          drive(speed, shift, Rot);
        }
        else
        {
          stop();
        }
        if (EN_SERVO)
        {
          // targetServo(camX, camY);
        }
        if (option == 1)
        {
          digitalWrite(2, HIGH);
        }
        else if (option == 0)
        {
          digitalWrite(2, LOW);
        }
        else if (option > 1)
        {
          // scan(;);
        }
      }
    }
  }
  Serial.println("Client disconnected");
}

//* Scanner (Servo+TFL)
#include <TFLI2C.h>

TFLI2C tflI2C;
int16_t tfDist;               // distance in centimeters
int16_t tfAddr = 0x10; // Use this default I2C address

int xdots[180];
int ydots[180];

float K = 0.199 + 0.801;

void setupScanner()
{
  EN_SCANNER = true;
  if (true)
  {
    delay(1500);
    for (int test = 0; test <= 180; test += 1)
    {
      delay(10);
      Xservo.write(test * K);
      Serial.println(test);
    }
    delay(500);
    Xservo.write(0);
    delay(500);
  }
}

int16_t mesure()
{
  if (!EN_SCANNER)
  {
    Serial.println("pls turn on scanner!!");
    ESP.restart();
  }
  delay(10);
  tflI2C.getData(tfDist, tfAddr);
  // Serial.println(tfDist);
  return tfDist;
}

int XServo_position = 0;
int dx = 1;
int dy = 1;
double t = 1.2;
// double K = 1.0;

void scan()
{
  String toSend[180];
  Xservo.write(XServo_position * K);
  for (int YServo_position = 0; YServo_position < 180; YServo_position += dy)
  {
    if (false)
    { //(XServo_position < 90){
      for (XServo_position = 0; XServo_position < 180; XServo_position += dx)
      {
        Xservo.write(XServo_position * K);
        xdots[XServo_position] = XServo_position;
      }
    }
    else
    {
      for (XServo_position = 180; XServo_position > 0; XServo_position -= dx)
      {
        Xservo.write(XServo_position * K);
        xdots[XServo_position] = mesure();
      }
    }
    Yservo.write(YServo_position);
    Xservo.write(0);
    uploadScan(YServo_position, xdots);
    String toAdd = "";

    delay(20);
  }
  
}

void uploadScan(int pos, int theScan[180])
{
  for (int i = 0; i < 180; i++)
  {
    Serial.print(i);
    Serial.print(" : ");
    Serial.println(theScan[i]);
  }
  // HTTPClient http;
  // String Line = "";
  //   Line += theScan[i] + ",";
  // }
  // Line += theScan[179];

  //   String url = BaseURL + "US/" + pos + "/" + Line;
  //   http.begin(url);
  //   int httpResponseCode = http.GET();
  //   http.end();
}

//* Main
void setup()
{
  Serial.begin(115200);
  setupMotors();
  stop();
  setupServo();
  // setupDisplay();
  // setupRGB();
  // setupGyro();
  // setupIR();
  setupWiFi();
  setupScanner();

  Serial.println("Ready! Starting...");
  delay(500);
  scan();
}

void runRemotCtrl()
{
  Serial.println("starting rmtctrl...");
  remoteCtrl();
  Serial.println("rmtctrl ended");
  delay(5000);
}

void loop()
{
  runRemotCtrl();
  // scan();
  // delay(10000);
  // mesure();
  // drive(0, 100, 0);
  // delay(1000);
  // drive(100, 0, 0);
  // delay(1000);
  // drive(-100, 0, 0);
  // delay(1000);
  // drive(0, -100, 0);
  // delay(1000);
  
}