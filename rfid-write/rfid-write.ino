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
    memcpy(buffer, "0000", 16);
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

    status = (MFRC522::StatusCode)mfrc522.MIFARE_Ultralight_Write(6, buffer, 4);
    if (status != MFRC522::STATUS_OK)
    {
        Serial.print(F("MIFARE_Read() failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
        return;
    }

    Serial.println(F("MIFARE_Ultralight_Write() OK "));

    Serial.println("3");

    mfrc522.PICC_HaltA();
    delay(500);
}