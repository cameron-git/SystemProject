#include <ArduinoJson.h>
#include "Firebase_Arduino_WiFiNINA.h"

#define DATABASE_URL "systemprojectgroup1-default-rtdb.europe-west1.firebasedatabase.app" //<databaseName>.firebaseio.com or <databaseName>.<region>.firebasedatabase.app
#define DATABASE_SECRET "l3ONLDlbr7x1zRP7Y8a0UOA8UNH1PL6VDCilozTU"
#define WIFI_SSID "VM7136878"
#define WIFI_PASSWORD "dryv5qrKqsc7"
// #define WIFI_SSID "S10"
// #define WIFI_PASSWORD "camerons10"
#define ARDUINO_ID "Arduino 1"
#define LDR_CUTOFF 300

FirebaseData fbdo;
int count;
int8_t ldrPins[] = {A0, A1, A2, A3, A6, A7};
StaticJsonDocument<512> doc;
JsonObject shelf1 = doc.createNestedObject();
JsonObject shelf2 = doc.createNestedObject();
char jsonData[512];

void setup()
{
  Serial.begin(9600);

  Firebase.begin(DATABASE_URL, DATABASE_SECRET, WIFI_SSID, WIFI_PASSWORD);
  Firebase.reconnectWiFi(true);

  shelf1["shelfId"] = "LDR 1";
  shelf1["contents"] = 0;
  shelf1["capacity"] = 6;
  shelf1["arduinoId"] = ARDUINO_ID;

  shelf2["shelfId"] = "Shelf 2";
  shelf2["contents"] = 0;
  shelf2["capacity"] = 3;
  shelf2["arduinoId"] = ARDUINO_ID;

  serializeJson(doc, jsonData);

  if (Firebase.setJSON(fbdo, "/", jsonData))
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
  if (updateLDR())
  {
    serializeJson(doc, jsonData);
    if (Firebase.setJSON(fbdo, "/", jsonData))
    {
      if (fbdo.dataType() == "json")
        Serial.println(fbdo.jsonData());
    }
    else
    {
      Serial.println(fbdo.errorReason());
    }
  }
  delay(200);
}

bool updateLDR()
{
  count = 0;
  for (int i = 0; i < 6; i++)
  {
    if (analogRead(ldrPins[i]) > LDR_CUTOFF)
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
