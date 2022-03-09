#include "SPI.h"
#include "MFRC522.h"

#define RST_PIN 9
#define SS_PIN 10

MFRC522 mfrc522(SS_PIN, RST_PIN);
MFRC522::StatusCode status;
byte buffer[18];
byte len = sizeof(buffer);

void setup()
{
  Serial.begin(9600);
  while (!Serial)
    ;
  SPI.begin(); // Init SPI bus
  mfrc522.PCD_Init();
  Serial.println("Looking...");
}

void loop()
{
  if (!mfrc522.PICC_IsNewCardPresent())
  {
    return;
  }
  Serial.println("1");
  if (!mfrc522.PICC_ReadCardSerial())
  {
    return;
  }
  Serial.println("2");

  status = (MFRC522::StatusCode)mfrc522.MIFARE_Read(6, buffer, &len);
  if (status != MFRC522::STATUS_OK)
  {
    Serial.print(F("MIFARE_Read() failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }
  Serial.println("3");

  for (uint8_t i = 0; i < 4; i++)
  {
    Serial.write(buffer[i]);
  }
  Serial.println();

  if (memcmp(buffer, "1234", 4) == 0)
  {
    Serial.println("AYYYYY");
  }

  mfrc522.PICC_HaltA();
  delay(500);
}