#include "Arduino.h"
#include <SPI.h>
#include <RF24.h>
int sensorPin = A0;
int Moisture_Analog_value = 0;
int Moisture_Percentage = 0;
struct package
{
  int x = 0;
};
typedef struct package Package;
Package RF_data[30];
// Package RF_dataRx[30];

// This is just the way the RF24 library works:
// Hardware configuration: Set up nRF24L01 radio on SPI bus (pins 10, 11, 12, 13) plus pins 7 & 8
RF24 radio(7, 8);

byte addresses[][6] = {"1Node", "2Node"};


// -----------------------------------------------------------------------------
// SETUP   SETUP   SETUP   SETUP   SETUP   SETUP   SETUP   SETUP   SETUP
// -----------------------------------------------------------------------------
void setup() {
  Serial.begin(9600);
  Serial.println("THIS IS THE TRANSMITTER CODE - YOU NEED THE OTHER ARDIUNO TO SEND BACK A RESPONSE");
  pinMode(3, OUTPUT);
  digitalWrite(3, HIGH);
  // Initiate the radio object
  radio.begin();

  // Set the transmit power to lowest available to prevent power supply related issues
  radio.setPALevel(RF24_PA_MIN);

  // Set the speed of the transmission to the quickest available
  radio.setDataRate(RF24_2MBPS);

  // Use a channel unlikely to be used by Wifi, Microwave ovens etc


  // Open a writing and reading pipe on each radio, with opposite addresses
  radio.openWritingPipe(addresses[1]);
  radio.openReadingPipe(1, addresses[0]);
  radio.openReadingPipe(1, 0xF0F0F0F0b);
  radio.openWritingPipe(0xF0F0F0F0);
  // Random number seeding (we're going to be sending a single random number)

}

// -----------------------------------------------------------------------------
// LOOP     LOOP     LOOP     LOOP     LOOP     LOOP     LOOP     LOOP     LOOP
// -----------------------------------------------------------------------------
void loop()
{
  Moisture_Analog_value = analogRead(sensorPin);

  Moisture_Analog_value = constrain(Moisture_Analog_value, 300, 1005);

  Moisture_Percentage = map(Moisture_Analog_value, 1005, 300, 0, 100);

  Serial.print("Moisture_Percentage=");

  Serial.print(Moisture_Percentage);

  Serial.println("%");


  RF_data[0].x = Moisture_Percentage;

  radio.stopListening();

  if (radio.available() || radio.write( &RF_data, sizeof(unsigned long) ))
  {
    Serial.print("Sent: ");

    Serial.println(RF_data[0].x);
  }

  radio.startListening();

  unsigned long started_waiting_at = millis();

  while ( ! radio.available() )
  {
    if (millis() - started_waiting_at > 200 )
    {
      //  Serial.println("No response received - timeout!");
      delay(500);

      return;
    }
  }
}
