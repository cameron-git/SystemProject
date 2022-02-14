#include <ArduinoJson.h>

int sensorPin = A0;
int prev;
int cur;

void setup()
{
  Serial.begin(9600);

  StaticJsonDocument<512> doc;
  doc["arduinoId"] = "arduino 1";
  JsonArray shelves = doc.createNestedArray("shelves");
  JsonObject shelf1 = shelves.createNestedObject();
  JsonObject shelf2 = shelves.createNestedObject();

  shelf1["shelfId"] = "P60S9yHK54mJKG0nm3Jj";
  shelf1["contents"] = 0;
  shelf1["capacity"] = 2;

  shelf2["shelfId"] = "shelf 2";
  shelf2["contents"] = 0;
  shelf2["capacity"] = 3;

  serializeJson(doc, Serial);
  Serial.println();
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