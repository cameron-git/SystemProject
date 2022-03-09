/*
pin wiring:

2 motor ain1
3 motor ain2
5 motor bin1
6 motor bin2
9 rfid reset
10 rfid sda
11 rfid mosi
12 rfid miso
13 rfid sck
14 ir1
15 ir2
16 ir3
17 servo pwm
18 lidar sda
19 lidar scl
21 buzzer
 */

// Libraries
#include "SPI.h"
#include "MFRC522.h"
#include "Firebase_Arduino_WiFiNINA.h"

// Firebase and WIFI defs
#define DATABASE_URL "systemprojectgroup1-default-rtdb.europe-west1.firebasedatabase.app"
#define DATABASE_SECRET "kNyRYqXGdkTOw2WDaNOmgNiL4yQaJGoImAPHFr9C"
// #define WIFI_SSID "VM7136878"
// #define WIFI_PASSWORD "dryv5qrKqsc7"
#define WIFI_SSID "S10"
#define WIFI_PASSWORD "camerons10"

// Line following defs
#define MOT_A1_PIN 2
#define MOT_A2_PIN 3
#define MOT_B1_PIN 5
#define MOT_B2_PIN 6

// RFID defs
#define RST_PIN 9
#define SS_PIN 10

// Line following vars
boolean reverseL = 0;
boolean reverseR = 0;
int leftServoSpeed = 0;
int rightServoSpeed = 0;
const int irPins[3] = {A0, A1, A2};
int irSensorDigital[3] = {0, 0, 0};
int threshold = 500;
const int maxSpeed = 180;
int irSensors = 0b000;
int error = 0;
int errorLast = 0;

// RFID vars
MFRC522 mfrc522(SS_PIN, RST_PIN);
MFRC522::StatusCode status;
byte buffer[18];
byte len = sizeof(buffer);

// Shelf and Firebase vars
char targetShelf[8];
char lastShelf[8];
bool onTarget;
FirebaseData fbdo;

void setup(void)
{
  Serial.begin(9600);
  // delay(1000);

  // Firebase setup
  Firebase.begin(DATABASE_URL, DATABASE_SECRET, WIFI_SSID, WIFI_PASSWORD);
  Firebase.reconnectWiFi(true);

  // Motor setup
  pinMode(MOT_A1_PIN, OUTPUT);
  pinMode(MOT_A2_PIN, OUTPUT);
  pinMode(MOT_B1_PIN, OUTPUT);
  pinMode(MOT_B2_PIN, OUTPUT);
  digitalWrite(MOT_A1_PIN, LOW);
  digitalWrite(MOT_A2_PIN, LOW);
  digitalWrite(MOT_B1_PIN, LOW);
  digitalWrite(MOT_B2_PIN, LOW);

  // RFID setup
  SPI.begin();
  mfrc522.PCD_Init();
}
void loop()
{
  // delay(2000);
  if (Firebase.getString(fbdo, "/barcode"))
  {
    if (fbdo.dataType() == "string")
    {
      fbdo.stringData().toCharArray(targetShelf, 5);
      Serial.print("targetShelf: ");
      Serial.println(targetShelf);
      Serial.print("lastShelf: ");
      Serial.println(lastShelf);
      if (strcmp(targetShelf, "stop") == 0)
        return;
      if (strcmp(targetShelf, lastShelf) != 0)
      {
        Serial.println("lastShelf set");
        strcpy(lastShelf, targetShelf);
        onTarget = false;
        while (!onTarget)
        {
          Serial.println("---");
          onTarget = scanRFID();
          // Add LIDAR and buzzer code here
          scan_IR();
          UpdateDirection();
          spin_and_wait(leftServoSpeed, rightServoSpeed, 10);
        }
        spin_and_wait(0, 0, 0);
        Serial.println("On rfid");
      }
    }
  }
  else
  {
    // Failed, then print out the error detail
    Serial.println(fbdo.errorReason());
  }
}

bool scanRFID()
{
  if (!mfrc522.PICC_IsNewCardPresent())
  {
    return false;
  }
  if (!mfrc522.PICC_ReadCardSerial())
  {
    return false;
  }
  status = mfrc522.MIFARE_Read(6, buffer, &len);

  if (status != MFRC522::STATUS_OK)
  {
    Serial.print(F("MIFARE_Read() failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return false;
  }
  Serial.print("Buffer: ");
  for (uint8_t i = 0; i < 4; i++)
  {
    Serial.write(buffer[i]);
  }
  Serial.println();
  mfrc522.PICC_HaltA();
  if (memcmp(buffer, targetShelf, 4) == 0)
  {
    return true;
  }
  return false;
};

void set_motor_pwm(int pwm, int IN1_PIN, int IN2_PIN)
{
  if (pwm < 0)
  { // reverse speeds
    analogWrite(IN1_PIN, -pwm);
    digitalWrite(IN2_PIN, LOW);
  }
  else
  { // stop or forward
    digitalWrite(IN1_PIN, LOW);
    analogWrite(IN2_PIN, pwm);
  }
}

void set_motor_currents(int pwm_A, int pwm_B)
{

  set_motor_pwm(pwm_A, MOT_A1_PIN, MOT_A2_PIN);
  set_motor_pwm(pwm_B, MOT_B1_PIN, MOT_B2_PIN);
}

void spin_and_wait(int pwm_A, int pwm_B, int duration)
{
  set_motor_currents(pwm_A, pwm_B);
  delay(duration);
}

void scan_IR()
{
  irSensors = 0b000;
  for (int i = 0; i < 3; i++)
  {
    int sensorValue = analogRead(irPins[i]);
    // Serial.print(sensorValue);
    // Serial.print(" ");
    if (sensorValue >= threshold)
    {
      irSensorDigital[i] = 1;
    }
    else
    {
      irSensorDigital[i] = 0;
    }
    int b = 2 - i;
    irSensors = irSensors + (irSensorDigital[i] << b);
  }
  // Serial.println();
}

void UpdateDirection()
{
  errorLast = error;
  switch (irSensors)
  {
  case 0b000: // no sensor detects the line
    if (errorLast < 0)
    {
      error = -180;
    }
    else if (errorLast > 0)
    {
      error = 180;
    }
    break;

  case 0b100: // left sensor on the line
    error = -120;
    break;

  case 0b110:
    error = -40;
    break;

  case 0b010:
    error = 0;
    break;

  case 0b011:
    error = 40;
    break;

  case 0b001: // right sensor on the line
    error = 120;
    break;

  case 0b111:
    error = 1;
    break;

  default:
    error = errorLast;
  }

  if (error >= 0)
  {
    leftServoSpeed = maxSpeed;
    rightServoSpeed = maxSpeed - error;
  }

  else if (error < 0)
  {
    leftServoSpeed = maxSpeed + error;
    rightServoSpeed = maxSpeed;
  }
}