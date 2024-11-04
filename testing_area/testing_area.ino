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


// //* Servos
// // #include <Servo.h>

// Servo Xservo;
// Servo Yservo;

// void setupServo()
// {
//   EN_SERVO = true;
//   pinMode(25, OUTPUT);
//   Xservo.attach(23);
//   Yservo.attach(2);
//   Yservo.write(0);
//   Xservo.write(0);
// }

// int XservoPos = 0, YservoPos = 0;

// void targetServo(int Xtarget, int Ytarget)
// {
//   while (abs(Xtarget - XservoPos) + abs(Ytarget - YservoPos) > 2)
//   {
//     int dx = abs(Xtarget - XservoPos) / (Xtarget - XservoPos);
//     int dy = abs(Ytarget - YservoPos) / (Ytarget - YservoPos);
//     Xservo.write(XservoPos + dx);
//     XservoPos = XservoPos + dx;
//     Yservo.write(YservoPos + dy);
//     YservoPos = YservoPos + dy;
//     delay(20);
//   }
// }

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
    printOnDisplay(String(WiFi.localIP()));
  }
}

int contX;
int contY;
int camX;
int camY;
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
  String f = a.substring(commaIndex4 + 1);              // Extract substring after the fourth comma

  contX = b.toInt();
  contY = c.toInt();
  camX = d.toInt();
  camY = e.toInt();
  option = f.toInt();
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
        int speed = contY, shift = contX;
        if (speed != 0 || shift != 0)
        {
          drive(speed, 0, shift);
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
          // scan();
        }
      }
    }
  }
  Serial.println("Client disconnected");
}

// void setupScanner()
// {
//   EN_SCANNER = true;
//   while (0)
//   {
//     delay(1500);
//     for (int test = 0; test <= 180; test += 1)
//     {
//       delay(10);
//       Xservo.write(test * K);
//       Serial.println(test);
//     }
//     delay(500);
//     Xservo.write(0);
//     delay(500);
//   }
// }

// int16_t mesure()
// {
//   if (!EN_SCANNER)
//   {
//     Serial.println("pls turn on scanner!!");
//     ESP.restart();
//   }
//   delay(10);
//   tflI2C.getData(tfDist, tfAddr);
//   Serial.println(tfDist);
//   return tfDist;
// }

// int XServo_position = 0;
// int dx = 1;
// int dy = 1;
// double t = 1.2;

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
//     uploadScan(YServo_position, xdots);
//     String toAdd = "";

//     delay(20);
//   }
  
// }

// void uploadScan(int pos, int theScan[180])
// {
//   HTTPClient http;
//   String Line = "";
//     Line += theScan[i] + ",";
//   }
//   Line += theScan[179];

//     String url = BaseURL + "US/" + pos + "/" + Line;
//     http.begin(url);
//     int httpResponseCode = http.GET();
//     http.end();
// }

//* Main
void setup()
{
  Serial.begin(115200);
  setupMotors();
  // setupServo();
  // setupDisplay();
  // setupRGB();
  // setupGyro();
  // setupIR();
  setupWiFi();
  // setupScanner();

  Serial.println("Ready! Starting...");
  delay(500);
}

void loop()
{
  Serial.println("starting rmtctrl...");
  remoteCtrl();
  Serial.println("rmtctrl ended");
  delay(5000);
  // driveIR(90, 5000, 0.3, 0.08);
}