#include <Adafruit_NeoPixel.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_LSM9DS1.h>
#include <Adafruit_Sensor.h>
#include <SPI.h>
#include <SD.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
//------------------------------------------------------------------------------
const int PIN = 10;
const int NUMPIXELS = 8;
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
//------------------------------------------------------------------------------
Adafruit_LSM9DS1 lsm = Adafruit_LSM9DS1();
#define LSM9DS1_SCK A5
// #define LSM9DS1_MISO 12
#define LSM9DS1_MOSI A4
// #define LSM9DS1_XGCS 6
// #define LSM9DS1_MCS 5
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
  Serial.begin(115200);
  while (!Serial);
  setupSensor();
  gpsPort.begin(GPS_BAUD);
  pixels.begin(); // This initializes the NeoPixel library.
}
//------------------------------------------------------------------------------
void loop()
{
  lsm.read();
  sensors_event_t a, m, g, temp;
  lsm.getEvent(&a, &m, &g, &temp);

  while (gpsPort.available())
  {
    tinyGPS.encode(gpsPort.read());
  }
  float ourHeading =  180.0f;
  // float ourHeading =  getHeading(m.magnetic.x-offsetX, m.magnetic.y-offsetY);
  float targetHeading = 0.0f; // get heading from GPS
  // float targetHeading = getTargetHeading(); // get heading from GPS

  pixels.clear();
  setNorthPixel(ourHeading);
  setPixelToCompassDirection(ourHeading, pixels.Color(0,0,50));
  setPixelToCompassDirection(getHeadingDiff(ourHeading, targetHeading), pixels.Color(0,150,0));
  Serial.println(getHeading(m.magnetic.x-offsetX, m.magnetic.y-offsetY));

  pixels.show();
  delay(100);
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

  // int roundHeading = (int)heading;
  // roundHeading = map(roundHeading, 0, 337,0,7) % 7;

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
  pixels.setPixelColor(pixelIndex, colour);

}


float getHeadingDiff(float ourHeading,float otherHeading)
{
  float diff = ourHeading - otherHeading;
  float headDiff = 360 - abs(diff);

  float newHeading =  (diff > 0) ? ourHeading + headDiff : ourHeading - headDiff;
  newHeading = (newHeading > 360) ? (360 - newHeading) : newHeading;
  newHeading = (newHeading < 0 )? (360 + newHeading) : newHeading;

  return newHeading;
}


void setNorthPixel(float ourHeading)
{
  setPixelToCompassDirection(getHeadingDiff(0.0f,ourHeading), pixels.Color(150,150,0));
}
