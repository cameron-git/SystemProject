#include <ArduinoJson.h>
#include "Firebase_Arduino_WiFiNINA.h"

FirebaseData fbdo;

int sensorPin = A0;
int prev;
int cur;

void setup()
{
  Serial.begin(9600);
  while (!Serial) ;
  
  Serial.println("start");

  Firebase.begin("systemprojectgroup1-default-rtdb.europe-west1.firebasedatabase.app", "kecPXGK9y9C6ecdNcVYLouU4lGzbTZ7AM09bx1Om", "S10", "camerons10");
  
  Firebase.reconnectWiFi(true);

  if (Firebase.setInt(fbdo, "/int", 1))
  {
    if (fbdo.dataType() == "int")
      Serial.println(fbdo.intData());
  }
  else
  {
    // Failed, then print out the error detail
    Serial.println(fbdo.errorReason());
  }

  // StaticJsonDocument<512> doc;
  // doc["arduinoId"] = "arduino 1";
  // JsonArray shelves = doc.createNestedArray("shelves");
  // JsonObject shelf1 = shelves.createNestedObject();
  // JsonObject shelf2 = shelves.createNestedObject();

  // shelf1["shelfId"] = "P60S9yHK54mJKG0nm3Jj";
  // shelf1["contents"] = 0;
  // shelf1["capacity"] = 2;

  // shelf2["shelfId"] = "shelf 2";
  // shelf2["contents"] = 0;
  // shelf2["capacity"] = 3;

  // serializeJson(doc, Serial);
  // Serial.println();
}

void loop()
{
  
  /*
  // read the value from the sensor:
  cur = analogRead(sensorPin);

  if (cur != prev)
  {
    Serial.println(cur);
    prev = cur;
  }
  delay(200);
  */
}