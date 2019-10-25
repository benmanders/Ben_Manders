//LEDS//--------------------------------------------------
#include "FastLED.h"
#define NUM_LEDS 15
#define DATA_PIN 10
CRGB leds[NUM_LEDS];
int sensorPin = A0;
int sensorValue = 0;
uint8_t gHue = 0;
//--------------------------------------------------------
#include <SPI.h>
#include <SD.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
//--------------------------------------------------------
const int gpsUpdateRate = 5000; // Log every 5 seconds
unsigned long lastLog = 0; // Global var to keep of last time we logged
const float piToDegrees = 3.1415963 / 180.0f;
//--------------------------------------------------------
// TinyGPS Definitions
TinyGPSPlus tinyGPS; // tinyGPSPlus object to be used throughout
const int GPS_BAUD = 9600; // GPS module's default baud rate
//--------------------------------------------------------
// GPS Serial Port Definitions //
const int ARDUINO_GPS_RX = 9; // GPS TX, Arduino RX pin
const int ARDUINO_GPS_TX = 8; // GPS RX, Arduino TX pin
SoftwareSerial gpsPort(ARDUINO_GPS_TX, ARDUINO_GPS_RX); // Create a SoftwareSerial
//--------------------------------------------------------
const float targetLat = 55.9456;
const float targetLng = -3.1995;
//--------------------------------------------------------
void setup()
{
  //LEDS//--------------------------------------------------
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
  //--------------------------------------------------------
  //float answer =  distanceBetweenPoints(55, 3, targetLat, targetLng);
  Serial.begin(9600);
  gpsPort.begin(GPS_BAUD);
  //Serial.println(answer);
}
//--------------------------------------------------------
void loop()
{
  while (gpsPort.available())
  {
    tinyGPS.encode(gpsPort.read());
  }
  updateGPS();

  //LEDS//--------------------------------------------------
  displayLED();
}
//--------------------------------------------------------
