int sensorPin = A0;
int prev;
int cur;

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  // read the value from the sensor:
  cur = analogRead(sensorPin);
  if (cur < 512)
  {
    cur = 0;
  }
  else
  {
    cur = 1;
  }

  if (cur != prev)
  {
    Serial.println(cur);
    prev = cur;
  }
  delay(500);
}