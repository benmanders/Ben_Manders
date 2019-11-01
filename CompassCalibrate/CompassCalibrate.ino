/*
   calibrate Adafruit_LSM9DS1
*/
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_LSM9DS1.h>
#include <Adafruit_Sensor.h>
//------------------------------------------------------------------------------
Adafruit_LSM9DS1 lsm = Adafruit_LSM9DS1();
//------------------------------------------------------------------------------
#define LSM9DS1_SCK A5
#define LSM9DS1_MISO 12
#define LSM9DS1_MOSI A4
#define LSM9DS1_XGCS 6
#define LSM9DS1_MCS 5
//------------------------------------------------------------------------------
const bool calibrationMode = false;
float offsetX = 0.034;
float offsetY = 0.225;
float offsetZ = 0.155;
float printScaling = 1000.;
const float radToDegCoef = (180.0f / 3.1415963f);
//------------------------------------------------------------------------------
void setupSensor()
{
  lsm.setupAccel(lsm.LSM9DS1_ACCELRANGE_2G); // 1.) Set the accelerometer range
  lsm.setupMag(lsm.LSM9DS1_MAGGAIN_16GAUSS);   // 2.) Set the magnetometer sensitivity
  lsm.setupGyro(lsm.LSM9DS1_GYROSCALE_245DPS);   // 3.) Setup the gyroscope
}
//------------------------------------------------------------------------------
void setup()
{
  Serial.begin(115200);

  while (!Serial);

  if (!lsm.begin())
  {
    Serial.println("Oops ... unable to initialize the LSM9DS1. Check your wiring!");
    while (1);
  }
  Serial.println("Found LSM9DS1 9DOF");

  setupSensor();
  if (calibrationMode)
    calibrateMagnetometer(5000);
}

void loop()
{
  lsm.read();
  sensors_event_t a, m, g, temp;
  lsm.getEvent(&a, &m, &g, &temp);
  //----------------------------------------------------------------------------
  // Data Averaging
  float avgX = 0, avgY = 0;
  for (int i = 0; i < 10; i++)
  {
    lsm.read();
    lsm.getEvent(&a, &m, &g, &temp);
    avgX += (m.magnetic.x - offsetX);
    avgY += (m.magnetic.y  - offsetY);
    delay(1);
  }

  avgX /= 10.f;
  avgY /= 10.f;
  //----------------------------------------------------------------------------
  printAlternateHeading(avgX, avgY);
  delay(5);
}
//------------------------------------------------------------------------------
void printAlternateHeading(float mx, float my)
{
  float D = atan2(mx , my) * radToDegCoef;
  Serial.println(D, 2);
}
//------------------------------------------------------------------------------
void printCompassDirection(float heading)
{
  String direction;

  if (heading > 337.25 || heading < 22.5)
  {
    direction = "NORTH";
  }
  else if (heading > 292.5 && heading < 337.25)
  {
    direction = "NORTH-WEST";
  }
  else if (heading > 247.5 && heading < 292.5)
  {
    direction = "WEST";
  }
  else if (heading > 202.5 && heading < 292.5)
  {
    direction = "SOUTH-WEST";
  }
  else if (heading > 157.5 && heading < 202.5)
  {
    direction = "SOUTH";
  }
  else if (heading > 112.5 && heading < 157.5)
  {
    direction = "SOUTH-EAST";
  }
  else if (heading > 67.5 && heading < 112.5)
  {
    direction = "EAST";
  }
  else if (heading > 22.5 && heading < 67.5)
  {
    direction = "NORTH-EAST";
  }

  Serial.print("Heading: "); Serial.println(heading); Serial.println(direction);
}
