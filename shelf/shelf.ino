#include <ArduinoJson.h>
#include "Firebase_Arduino_WiFiNINA.h"

#define DATABASE_URL "systemprojectgroup1-default-rtdb.europe-west1.firebasedatabase.app" //<databaseName>.firebaseio.com or <databaseName>.<region>.firebasedatabase.app
#define DATABASE_SECRET "l3ONLDlbr7x1zRP7Y8a0UOA8UNH1PL6VDCilozTU"
#define WIFI_SSID "VM7136878"
#define WIFI_PASSWORD "dryv5qrKqsc7"
// #define WIFI_SSID "S10"
// #define WIFI_PASSWORD "camerons10"
#define ARDUINO_ID "Arduino 1"

FirebaseData fbdo;

char jsonData[512];

void setup()
{
  // Serial.begin(9600);
  // while (!Serial)
  // {
  // };

  Firebase.begin(DATABASE_URL, DATABASE_SECRET, WIFI_SSID, WIFI_PASSWORD);
  Firebase.reconnectWiFi(true);

  StaticJsonDocument<512> doc;
  JsonObject shelf1 = doc.createNestedObject();
  JsonObject shelf2 = doc.createNestedObject();

  shelf1["shelfId"] = "Shelf 1";
  shelf1["contents"] = 0;
  shelf1["capacity"] = 2;
  shelf1["arduinoId"] = ARDUINO_ID;

  shelf2["shelfId"] = "Shelf 2";
  shelf2["contents"] = 0;
  shelf2["capacity"] = 3;
  shelf2["arduinoId"] = ARDUINO_ID;

  serializeJson(doc, jsonData);

  if (Firebase.setJSON(fbdo, "/", jsonData))
  {
    // if (fbdo.dataType() == "json")
      // Serial.println(fbdo.jsonData());
  }
  else
  {
    // Failed, then print out the error detail
    // Serial.println(fbdo.errorReason());
  }
}

void loop()
{
// Upload data on change
}