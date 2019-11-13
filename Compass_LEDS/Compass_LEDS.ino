#include <Adafruit_NeoPixel.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_LSM9DS1.h>
#include <Adafruit_Sensor.h>
//------------------------------------------------------------------------------
const int PIN =            10;
const int NUMPIXELS =      8;
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

//------------------------------------------------------------------------------
Adafruit_LSM9DS1 lsm = Adafruit_LSM9DS1();
#define LSM9DS1_SCK A5
#define LSM9DS1_MISO 12
#define LSM9DS1_MOSI A4
#define LSM9DS1_XGCS 6
#define LSM9DS1_MCS 5
float offsetX = -0.139;
float offsetY = 0.25;
float offsetZ = -0.128;
constexpr float radToDegCoef = (180.0f / 3.1415963f);
//------------------------------------------------------------------------------
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
  pixels.begin(); // This initializes the NeoPixel library.
}
//------------------------------------------------------------------------------
void loop()
{
  lsm.read();
  sensors_event_t a, m, g, temp;
  lsm.getEvent(&a, &m, &g, &temp);

  float ourHeading getHeading(m.magnetic.x-offsetX, m.magnetic.y-offsetY);
  float otherHeading = 0; // get heading from GPS

  pixels.clear();
  setNorthPixel();
  setPixelToCompassDirection(ourHeading, pixels.Color(0,0,50));
  setPixelToCompassDirection(getHeadingDiff(ourHeading, otherHeading), pixels.Color(0,150,0));
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
  pixels.setPixelColor(pixelIndex, color);

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


void setNorthPixel()
{
  setPixelToCompassDirection(getHeadingDiff(0.0f,ourHeading), pixels.Color(150,150,0));
}
