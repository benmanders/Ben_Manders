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

float getHeading(float mx, float my)
{
  float D = atan2(mx , my) * radToDegCoef;
  return D + 180.0f;
}
//------------------------------------------------------------------------------
float getHeadingDiff(float ourHeading, float otherHeading)
{
  float diff = ourHeading - otherHeading;
  return (diff < 0) ? 360 - abs(diff) : diff;
}
