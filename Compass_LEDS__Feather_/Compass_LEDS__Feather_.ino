#include <Adafruit_NeoPixel.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_LSM9DS1.h>
#include <Adafruit_Sensor.h>
#include <SPI.h>
#include <SD.h>
#include <TinyGPS++.h>
//#include <SoftwareSerial.h>
//------------------------------------------------------------------------------
#include <Arduino.h>   // required before wiring_private.h
#include "wiring_private.h" // pinPeripheral() function

bool DEBUG_MODE = false;

Uart Serial2 (&sercom1, 11, 10, SERCOM_RX_PAD_0, UART_TX_PAD_2);
void SERCOM1_Handler()
{
  Serial2.IrqHandler();
}
//------------------------------------------------------------------------------
#include "ThingSpeak.h"
#include <SPI.h>
#include <WiFi101.h>

char ssid[] = "HANABI";     //  your network SSID (name)
char pass[] = "lowernorwood";   // your network password
WiFiClient  client;

unsigned long myChannelNumber = 917504; //Put your channel number in here
const char * myWriteAPIKey = "M28IML2BN9W8LV43"; //Put your API key in here
//------------------------------------------------------------------------------
const int PIN = 10;
const int NUMPIXELS = 16;
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
//------------------------------------------------------------------------------
Adafruit_LSM9DS1 lsm = Adafruit_LSM9DS1();
#define LSM9DS1_SCK A5
#define LSM9DS1_MOSI A4
float offsetX = -0.139;
float offsetY = 0.25;
float offsetZ = -0.128;
constexpr float radToDegCoef = (180.0f / 3.1415963f);
//--------------------------------------------------------
const int gpsUpdateRate = 5000; // Log every 5 seconds
unsigned long lastLog = 0; // Global var to keep of last time we logged
const float piToDegrees = 3.1415963 / 180.0f;
//--------------------------------------------------------
// TinyGPS Definitions
TinyGPSPlus tinyGPS; // tinyGPSPlus object to be used throughout
const int GPS_BAUD = 4800; // GPS module's default baud rate
//--------------------------------------------------------
// GPS Serial Port Definitions //
const int ARDUINO_GPS_RX = 9; // GPS TX, Arduino RX pin
const int ARDUINO_GPS_TX = 8; // GPS RX, Arduino TX pin
//SoftwareSerial Serial2(ARDUINO_GPS_TX, ARDUINO_GPS_RX); // Create a SoftwareSerial
//--------------------------------------------------------
const float currentLat = 55.94565;
const float currentLng = -3.19955;
const float targetLat = 55.9456;
const float targetLng = -3.1995;
float lng = -3.19955;
float lat = 55.94565;
float heading ;
float distance;

//--------------------------------------------------------
void setupSensor()
{
  if (!lsm.begin())
  {
    Serial.println("Oops ... unable to initialize the LSM9DS1. Check your wiring!");
    while (1);
  }
  Serial.println("Found LSM9DS1 9DOF");

  lsm.setupAccel(lsm.LSM9DS1_ACCELRANGE_2G); // 1.) Set the accelerometer range
  lsm.setupMag(lsm.LSM9DS1_MAGGAIN_16GAUSS);   // 2.) Set the magnetometer sensitivity
  lsm.setupGyro(lsm.LSM9DS1_GYROSCALE_245DPS);   // 3.) Setup the gyroscope
}
//------------------------------------------------------------------------------
void setup()
{
  Serial.begin(4800);
  Serial2.begin(4800);
  //  while (!Serial);
  setupWifi();
  
  ThingSpeak.begin(client);
  Serial.println("Started");
  setupSensor();
  Serial2.begin(GPS_BAUD);
  pixels.begin(); // This initializes the NeoPixel library.
  // Assign pins 10 & 11 SERCOM functionality
  pinPeripheral(10, PIO_SERCOM);
  pinPeripheral(11, PIO_SERCOM);
}
//------------------------------------------------------------------------------
void loop()
{
  lsm.read();
  sensors_event_t a, m, g, temp;
  lsm.getEvent(&a, &m, &g, &temp);

  while (Serial2.available())
  {
    tinyGPS.encode(Serial2.read());
  }
  
  updateGPS();
  

  float ourHeading =  getHeading(m.magnetic.x - offsetX, m.magnetic.y - offsetY);
  float targetHeading = getTargetHeading(); // get heading from GPS

  Serial.print("Heading ");
  Serial.println(targetHeading);

  pixels.clear();
  //  setNorthPixel(ourHeading);
  //  setPixelToCompassDirection(ourHeading, pixels.Color(0,0,50));
  setPixelToCompassDirection(getHeadingDiff(ourHeading, targetHeading), pixels.Color(0, 120, 50));
  Serial.println(getHeadingDiff(ourHeading, targetHeading));

  pixels.show();
  delay(1000);
  updateThingspeak();

  while (!isWiFiConnected())
  {
    setupWifi();
    delay(10000);
  }
}
//------------------------------------------------------------------------------
float getHeading(float mx, float my)
{
  float D = atan2(mx , my) * radToDegCoef;
  return D + 180.0f;
}
//------------------------------------------------------------------------------
void setPixelToCompassDirection(float heading, uint32_t colour)
{
  int pixelIndex = 0;

  //   int roundHeading = (int)heading;
  //   roundHeading = map(roundHeading, 0, 337,0,15) % 15;
  //   pixelIndex = roundHeading;

  if (heading > 337.25 || heading < 22.5)
  {
    pixelIndex = 0;
  }
  else if (heading > 292.5 && heading < 337.25)
  {
    pixelIndex = 1;
  }
  else if (heading > 247.5 && heading < 292.5)
  {
    pixelIndex = 2;
  }
  else if (heading > 202.5 && heading < 292.5)
  {
    pixelIndex = 3;
  }
  else if (heading > 157.5 && heading < 202.5)
  {
    pixelIndex = 4;
  }
  else if (heading > 112.5 && heading < 157.5)
  {
    pixelIndex = 5;
  }
  else if (heading > 67.5 && heading < 112.5)
  {
    pixelIndex = 6;
  }
  else if (heading > 22.5 && heading < 67.5)
  {
    pixelIndex = 7;
  }
  pixels.setPixelColor(2 * pixelIndex, colour);
  pixels.setPixelColor((2 * pixelIndex) + 1, colour);
  Serial.print("Pixels : ");
  Serial.print(2 * pixelIndex);
  Serial.print(" & ");
  Serial.println((2 * pixelIndex) + 1);
}


float getHeadingDiff(float ourHeading, float otherHeading)
{
  float diff = ourHeading - otherHeading;
  return (diff < 0) ? 360 - abs(diff) : diff;
}


void setNorthPixel(float ourHeading)
{
  setPixelToCompassDirection(getHeadingDiff(0.0f, ourHeading), pixels.Color(150, 150, 0));
}
