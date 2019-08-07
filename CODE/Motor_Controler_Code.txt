#include "Arduino.h"
#include <SPI.h>
#include <RF24.h>


struct package
{
  int Moisture_percentage = 0;
};


typedef struct package Package;

Package RF_data[30];
// Package dataRx[30];

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
  radio.openReadingPipe(1, 0xF0F0F0F0);
  radio.openWritingPipe(0xF0F0F0F01);
  // Random number seeding (we're going to be sending a single random number)
  pinMode(2, OUTPUT);// connected to S terminal of Relay
  pinMode(3, OUTPUT);
  digitalWrite(3, HIGH);
}

// -----------------------------------------------------------------------------
// LOOP     LOOP     LOOP     LOOP     LOOP     LOOP     LOOP     LOOP     LOOP
// -----------------------------------------------------------------------------
void loop() {
  if (radio.available())

  {
    radio.read( &RF_data, sizeof(unsigned long) );

    Serial.print("received: ");

    Serial.println(RF_data[0].Moisture_percentage);
  }

  radio.startListening();

  unsigned long started_waiting_at = millis();

  while ( ! radio.available() )
  {
    if (millis() - started_waiting_at > 200 )
    {
      delay(500);
      return;
    }
  }

  if (RF_data[0].Moisture_percentage < 20)
  {
    digitalWrite(2, HIGH);
  }
  else
  {
    digitalWrite(2, LOW);
  }
}
