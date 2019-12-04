#include <Adafruit_NeoPixel.h>
#include <Adafruit_LSM9DS1.h>
#include <Adafruit_Sensor.h>
#include <Arduino.h>   // required before wiring_private.h
#include <SD.h>
#include <SPI.h>
#include <ThingSpeak.h>
#include <TinyGPS++.h>
#include <WiFi101.h>
#include <wiring_private.h> // pinPeripheral() function
#include <Wire.h>
//------------------------------------------------------------------------------
Uart Serial2 (&sercom1, 11, 10, SERCOM_RX_PAD_0, UART_TX_PAD_2);
void SERCOM1_Handler()
{
  Serial2.IrqHandler();
}
//------------------------------------------------------------------------------
char ssid[] = "...";     //  your network SSID (name)
char pass[] = "...";   // your network password
WiFiClient  client;
//------------------------------------------------------------------------------
const long thingSpeakUpdateRate = 15000; // Log every 5 seconds
unsigned long WriteChannelNumber = 917504;
unsigned long ReadChannelNumber = 922828;
const char * myWriteAPIKey = "M28IML2BN9W8LV43";
const char * myReadAPIKey = "LFCH9YTT7FBP61DW";
//------------------------------------------------------------------------------
const int PIN = 12;
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
//--------------------------------------------------------
float targetLat = 55.9456;
float targetLng = -3.1995;
float thisLat = 55.9456;
float thisLng = -3.1995;
//------------------------------------------------------------------------------
void setup()
{
  pinPeripheral(10, PIO_SERCOM); // Assign pins 10 & 11 SERCOM functionality
  pinPeripheral(11, PIO_SERCOM);
  Serial.begin(GPS_BAUD);
  Serial2.begin(GPS_BAUD);
  
  ThingSpeak.begin(client);
  setupSensor();
  pixels.begin(); // This initializes the NeoPixel library.
}
//------------------------------------------------------------------------------
void loop()
{
  //--------------------------------------------------------------------------
  // GPS
  while (Serial2.available())
  {
    tinyGPS.encode(Serial2.read());
  }
  updateThisLatLng();
  updateTargetLatLng();
  //--------------------------------------------------------------------------
  // 9DOF
  lsm.read();
  sensors_event_t a, m, g, temp;
  lsm.getEvent(&a, &m, &g, &temp);
  float ourHeading =  getHeading(m.magnetic.x - offsetX, m.magnetic.y - offsetY);
  float targetHeading = getTargetHeading(thisLat, thisLng, targetLat, targetLng);
  //--------------------------------------------------------------------------
  // NeoPixels
  pixels.clear();
  setPixelToCompassDirection(getHeadingDiff(ourHeading, targetHeading), pixels.Color(0, 120, 50));
  pixels.show();
  //--------------------------------------------------------------------------
  // ThingSpeak
  updateThingspeak();
  //--------------------------------------------------------------------------
  delay(1000);
}
//------------------------------------------------------------------------------
