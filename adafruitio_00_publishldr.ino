// Adafruit IO Publish Example
//
// Adafruit invests time and resources providing this open source code.
// Please support Adafruit and open source hardware by purchasing
// products from Adafruit!
//
// Written by Todd Treece for Adafruit Industries
// Copyright (c) 2016 Adafruit Industries
// Licensed under the MIT license.
//
// All text above must be included in any redistribution.

/************************** Configuration ***********************************/

// edit the config.h tab and enter your Adafruit IO credentials
// and any additional configuration needed for WiFi, cellular,
// or ethernet clients.
#include "config.h"

/************************ Example Starts Here *******************************/

// this int will hold the current count for our sketch
int count = 0;
#define LED_PIN1 D1
#define LED_PIN2 D4
// set up the 'digital' feed
AdafruitIO_Feed *led1 = io.feed("led1");
AdafruitIO_Feed *led3 = io.feed("led3");
// set up the 'counter' feed
AdafruitIO_Feed *ldr1 = io.feed("ldr1");

void setup() {

  pinMode(LED_PIN1, OUTPUT);
  #if defined(ARDUINO_ARCH_ESP32)
    // ESP32 pinMode()
    ledcAttachPin(LED_PIN2, 1);
    ledcSetup(1, 1200, 8);
  #else
    pinMode(LED_PIN2, OUTPUT);
  #endif
  // start the serial connection
  Serial.begin(115200);

  // wait for serial monitor to open
  while(! Serial);

  Serial.print("Connecting to Adafruit IO");

  // connect to io.adafruit.com
  io.connect();


  led1->onMessage(handleMessage);
  led3->onMessage(handleMessage1);

  // wait for a connection
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  // we are connected
  Serial.println();
  Serial.println(io.statusText());
  led3->get();
}

void loop() {

  // io.run(); is required for all sketches.
  // it should always be present at the top of your loop
  // function. it keeps the client connected to
  // io.adafruit.com, and processes any incoming data.
  io.run();

  // save count to the 'counter' feed on Adafruit IO
  Serial.print("sending -> ");
  count=analogRead(A0);
  Serial.println(count);
  ldr1->save(count);

  // increment the count by 1
 

  // Adafruit IO is rate limited for publishing, so a delay is required in
  // between feed->save events. In this example, we will wait three seconds
  // (1000 milliseconds == 1 second) during each loop.
  delay(3000);

}
void handleMessage(AdafruitIO_Data *data) {

  Serial.print("received <- ");

  if(data->toPinLevel() == HIGH)
    Serial.println("HIGH");
  else
    Serial.println("LOW");


  digitalWrite(LED_PIN1, data->toPinLevel());
}
void handleMessage1(AdafruitIO_Data *data) {

  // convert the data to integer
  int reading = data->toInt();

  Serial.print("received <- ");
  Serial.println(reading);

  
  // write the current 'reading' to the led
  #if defined(ARDUINO_ARCH_ESP32)
    ledcWrite(1, reading); // ESP32 analogWrite()
  #else
    analogWrite(LED_PIN2, reading);
  #endif

}
