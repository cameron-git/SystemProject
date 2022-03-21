#include "ArduinoJson.h"
#include "Firebase_Arduino_WiFiNINA.h"
#include "HX711.h"

#define LOADCELL_DOUT_PIN 2
#define LOADCELL_SCK_PIN 3
#define TARE_PIN 4
#define CALIBRATE_PIN 5
#define SCALE_NUM 1

#define DATABASE_URL "systemprojectgroup1-default-rtdb.europe-west1.firebasedatabase.app"
#define DATABASE_SECRET "kNyRYqXGdkTOw2WDaNOmgNiL4yQaJGoImAPHFr9C"
// #define WIFI_SSID "VM7136878"
// #define WIFI_PASSWORD "dryv5qrKqsc7"
#define WIFI_SSID "S10"
#define WIFI_PASSWORD "camerons10"
#define ARDUINO_ID "Arduino 1"
#define LDR_CUTOFF 300

FirebaseData fbdo;
int count;
float scaleReading;
int8_t ldrPins[] = {A0, A1, A2, A3};
StaticJsonDocument<512> doc;
JsonObject shelf1 = doc.createNestedObject();
JsonObject shelf2 = doc.createNestedObject();
char jsonData[512];
int ldrNum = sizeof(ldrPins);
HX711 scale;
float calibration_factor = 7050;

void setup()
{
  Serial.begin(9600);

  delay(1000);
  Firebase.begin(DATABASE_URL, DATABASE_SECRET, WIFI_SSID, WIFI_PASSWORD);
  Firebase.reconnectWiFi(true);

  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.set_scale(400);
  scale.tare();
  pinMode(TARE_PIN, INPUT);
  pinMode(CALIBRATE_PIN, INPUT);

  shelf1["shelfId"] = "Shelf 1";
  shelf1["barcode"] = "0001";
  shelf1["contents"] = 0;
  shelf1["capacity"] = ldrNum;
  shelf1["type"] = "ldr";
  shelf1["arduinoId"] = ARDUINO_ID;

  shelf2["shelfId"] = "Shelf 2";
  shelf2["barcode"] = "0002";
  shelf2["contents"] = 0;
  shelf2["capacity"] = 0;
  shelf2["type"] = "scale";
  shelf2["arduinoId"] = ARDUINO_ID;

  serializeJson(doc, jsonData);

  if (Firebase.setJSON(fbdo, "/locs/", jsonData))
  {
    if (fbdo.dataType() == "json")
      Serial.println(fbdo.jsonData());
  }
  else
  {
    Serial.println(fbdo.errorReason());
  }
}

void loop()
{
  if (digitalRead(TARE_PIN) == HIGH)
  {
    scale.tare();
    Serial.println("tare");
    while (digitalRead(TARE_PIN) == HIGH)
      ;
    return;
  }
  if (digitalRead(CALIBRATE_PIN) == HIGH)
  {
    calibrateScale();
    while (digitalRead(CALIBRATE_PIN) == HIGH)
      ;
    return;
  }

  if (updateLDR() || updateScale())
  {
    serializeJson(doc, jsonData);
    if (Firebase.setJSON(fbdo, "/locs/", jsonData))
    {
      if (fbdo.dataType() == "json")
        Serial.println(fbdo.jsonData());
    }
    else
    {
      Serial.println(fbdo.errorReason());
    }
  }
}

bool updateLDR()
{
  count = 0;
  for (int i = 0; i < ldrNum; i++)
  {
    if (analogRead(ldrPins[i]) < LDR_CUTOFF)
    {
      count++;
    }
  }
  if (count != shelf1["contents"])
  {
    shelf1["contents"] = count;
    return true;
  }
  return false;
}

bool updateScale()
{
  scaleReading = scale.get_units(10);
  if (scaleReading != shelf2["contents"])
  {
    shelf2["contents"] = scaleReading;
    return true;
  }
  return false;
}

void calibrateScale()
{
  scaleReading = scale.get_value(10) / SCALE_NUM;
  Serial.println(scaleReading);
  scale.set_scale(scaleReading);
}
