//* Firebase
#include <Firebase_ESP_Client.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

#define API_KEY "AIzaSyC00hAZFurAIfiE4WiawPSIIo-d8iSC5WU"
#define DATABASE_URL "https://swiftsneak-1379-default-rtdb.europe-west1.firebasedatabase.app/"

FirebaseData firebaseData;
FirebaseAuth auth;
FirebaseConfig config;

FirebaseJson json;

bool signUpOk = false;

#include <WiFi.h>
#include <HTTPClient.h>

const char *ssid = "P2p";
const char *password = "";

WiFiServer server(80);

String BaseURL = "";

void setupWiFi()
{
  // EN_WIFI = true;
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("");
  Serial.print("WiFi connected: ");
  Serial.println(WiFi.localIP());
  // if (EN_DISPLAY)
  // {
  //   // printOnDisplay(String(WiFi.localIP()));
  // }
}

void setupFirebase()
{
  config.host = DATABASE_URL;
  config.api_key = API_KEY;
  if (Firebase.signUp(&config, &auth, "", ""))
  {
    Serial.println("Sign up OK");
  }
  config.token_status_callback = tokenStatusCallback;
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void uploadJson(String path, bool merge)
{
  if (merge)
  {
    if (Firebase.RTDB.pushJSON(&firebaseData, path, &json))
    {
      //Serial.println("Push: Upload OK");
      delay(500);
    }
  }

  else
  {
    if (Firebase.RTDB.setJSON(&firebaseData, path, &json))
    {
      Serial.println("Set: Upload OK");
      delay(500);
    }
  }
}

void uploadData(String path, int data)
{
  if (Firebase.RTDB.setInt(&firebaseData, path, data))
  {
    Serial.println("Upload OK");
  }
}

int mesure()
{
  
  // if (!EN_SCANNER)
  // {
  //   Serial.println("pls turn on scanner!!");
  //   ESP.restart();
  // }
  // delay(60);
  // tflI2C.getData(tfDist, tfAddr);
  // // Serial.println(tfDist);
  // return tfDist;
  return 23;
}

int XServo_position = 0;
int dx = 1;
int dy = 1;
double t = 1.2;

int xdots[180];
int ydots[180];


void uploadScan(int pos, int xdots[180])
{
  int parts = 1;
  json.clear();
  uploadJson("/distances/", false);
 
  for (int j = 0; j < parts; j++)
  {
    //Serial.println("Uploading... " + String(j));
    json.clear();
    for (int i = 0; i < 180 / parts; i++)
    { 
      int k = j * (180 / parts) + i;
      json.add(String(k), String(xdots[k]));
    }
    uploadJson("/distances/"+String(pos), false);
  }
  Serial.println("Uploaded");
}

void scan()
{
  // Xservo.write(XServo_position * K);
  for (int YServo_position = 0; YServo_position < 180; YServo_position += dy)
  {
    // if (false)
    // { //(XServo_position < 90){
    //   for (XServo_position = 0; XServo_position < 180; XServo_position += dx)
    //   {
    //     Xservo.write(XServo_position * K);
    //     xdots[XServo_position] = XServo_position;
    //   }
    // }
    // else
    // {
      for (XServo_position = 0; XServo_position < 180; XServo_position += dx)
      {
        // Xservo.write(XServo_position * K);
        xdots[XServo_position] = mesure();
        // json.add(String(XServo_position), mesure());
      }
    // }
    uploadScan(YServo_position, xdots);
    json.clear();
    // Yservo.write(YServo_position);
    // Xservo.write(0);

    delay(20);
  }
}


void setup()
{
  Serial.begin(115200);
  setupWiFi();
  setupFirebase();
}
int lim = 1;

String repeat(String base, int time)
{
  String toret = "";
  for (int i = 0; i < time; i++)
  {
    toret += base;
  }
  return toret;
}

void loop()
{
  scan();
  // json.clear();
  // for (int i=0; i < lim; i++){
  //   json.add(i, repeat("o", i*1000));
  //   Serial.println(i);
  // }
  // ++lim;
  // Serial.println(lim);

  // uploadJson("/distances/", false);
  delay(200);
}
