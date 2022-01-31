

int sensorPin = A0;

void setup() {
  // declare the ledPin as an OUTPUT:
  Serial.begin(9600);
}

void loop() {
  // read the value from the sensor:
  Serial.println(analogRead(sensorPin));
  delay(200);
}