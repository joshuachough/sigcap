/*
  ECG Signal Capture

  Captures digitized ECG signal from the ADS1013 ADC and saves it to a microSD card.
*/

#include <Wire.h>

#define ADS1013_ID 72 // 1001000 in decimal = 72

void setup()
{
  Wire.begin(); // join i2c bus (address optional for master)
  Serial.begin(9600);
  while (!Serial)
    ;

  // 1. Write to Config register:
  //   – First byte : 0b10010000(first 7 - bit I2C address followed by a low R / W bit)
  //   – Second byte : 0b00000001(points to Config register)
  //   – Third byte : 0b10000100(MSB of the Config register to be written)
  //   – Fourth byte : 0b10000011(LSB of the Config register to be written)
  Wire.beginTransmission(ADS1013_ID); // transmit to ADS1013
  Wire.write(0x01);                   // points to Config register
  Wire.write(0x84);                   // MSB of the Config register to be written
  Wire.write(0x03);                   // LSB of the Config register to be written
  Wire.endTransmission();             // stop transmitting
  Serial.println("Wrote to Config register");

  // 2. Write to Address Pointer register :
  //   – First byte : 0b10010000(first 7 - bit I2C address followed by a low R / W bit)
  //   – Second byte : 0b00000000(points to Conversion register)
  Wire.beginTransmission(ADS1013_ID); // transmit to ADS1013
  Wire.write(0x00);                   // points to Conversion register
  Wire.endTransmission();             // stop transmitting
  Serial.println("Wrote to Address Pointer register");
}

void loop()
{
  // 3. Read Conversion register:
  //   – First byte : 0b10010001(first 7 - bit I2C address followed by a high R / W bit)
  //   – Second byte : the ADS101x response with the MSB of the Conversion register
  //   – Third byte : the ADS101x response with the LSB of the Conversion register
  Wire.requestFrom(ADS1013_ID, 2); // request 2 bytes from ADS1013
  Serial.print("Reading Conversion register...");
  while (Wire.available())
  {
    Serial.print("conversion value found! -> ");
    int MSB = Wire.read(); // MSB of the Conversion register
    int LSB = Wire.read(); // LSB of the Conversion register
    int val = (MSB << 8) | LSB;
    Serial.print(val);
  }
  Serial.println("");
}