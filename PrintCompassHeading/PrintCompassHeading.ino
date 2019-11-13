/*
   Print current compass heading
*/
#include <Wire.h>
#include <SPI.h>
#include <SparkFunLSM9DS1.h>
//------------------------------------------------------------------------------
LSM9DS1 imu;
//------------------------------------------------------------------------------
#define LSM9DS1_M   0x1E // Would be 0x1C if SDO_M is LOW
#define LSM9DS1_AG  0x6B // Would be 0x6A if SDO_AG is LOW
#define PRINT_CALCULATED
#define PRINT_SPEED 10 // 250 ms between prints
static unsigned long lastPrint = 0; // Keep track of print time
//------------------------------------------------------------------------------
// Earth's magnetic field varies by location. Add or subtract
// a declination to get a more accurate heading. Calculate
// your's here:
// http://www.ngdc.noaa.gov/geomag-web/#declination
#define DECLINATION -1.75 // Declination (degrees) in Edinburgh, UK
//------------------------------------------------------------------------------
float offsetX = 0.034;
float offsetY = 0.225;
float offsetZ = 0.155;
//------------------------------------------------------------------------------
void setup()
{
  Serial.begin(115200);

  imu.settings.device.commInterface = IMU_MODE_I2C;
  imu.settings.device.mAddress = LSM9DS1_M;
  imu.settings.device.agAddress = LSM9DS1_AG;

  if (!imu.begin())
  {
    Serial.println("Failed to communicate with LSM9DS1.");
    Serial.println("Double-check wiring.");
    Serial.println("Default settings in this sketch will " \
                   "work for an out of the box LSM9DS1 " \
                   "Breakout, but may need to be modified " \
                   "if the board jumpers are.");
    while (1)
      ;
  }

  imu.calibrateMag(true);
}
//------------------------------------------------------------------------------
void loop()
{
  if ( imu.gyroAvailable() )
  {
    imu.readGyro();
  }
  if ( imu.accelAvailable() )
  {
    imu.readAccel();
  }
  if ( imu.magAvailable() )
  {
    imu.readMag();
  }

  if ((lastPrint + PRINT_SPEED) < millis())
  {
    printAttitude(imu.ax, imu.ay, imu.az,
                  imu.mx, imu.my, imu.mz);
    lastPrint = millis();
  }
}
//------------------------------------------------------------------------------
void printAttitude(float ax, float ay, float az, float mx, float my, float mz)
{
  float roll = atan2(ay, az);
  float pitch = atan2(-ax, sqrt(ay * ay + az * az));
  float heading;
  
  if (my == 0)
    heading = (mx < 0) ? PI : 0;
  else
    heading = atan2(mx, my);

  heading -= DECLINATION * PI / 180;

  if (heading > PI)
    heading -= (2 * PI);
  else if (heading < -PI)
    heading += (2 * PI);

  // Convert everything from radians to degrees:
  heading *= 180.0 / PI;
  pitch *= 180.0 / PI;
  roll  *= 180.0 / PI;


  printCompassDirection(heading + 180.0);
}
//------------------------------------------------------------------------------
void printTiltCompensatedHeading(float ax, float ay, float az, float mx, float my, float mz)
{
  float roll = atan2(ay, az);
  float pitch = atan2(-ax, sqrt(ay * ay + az * az));
  float cosRoll = cos(roll);
  float sinRoll = sin(roll);
  float cosPitch = cos(pitch);
  float sinPitch = sin(pitch);

  float Xh = mx * cosPitch + mz * sinPitch;
  float Yh = mx * sinRoll * sinPitch + my * cosRoll - mz * sinRoll * cosPitch;

  float heading;

  if (my == 0)
    heading = (mx < 0) ? PI : 0;
  else
    heading = atan2(Yh, Xh);

  heading -= DECLINATION * PI / 180;

  if (heading > PI)
    heading -= (2 * PI);
  else if (heading < -PI)
    heading += (2 * PI);

  // Convert everything from radians to degrees:
  heading *= 180.0 / PI;

  printCompassDirection(heading + 180.0);
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
//------------------------------------------------------------------------------
