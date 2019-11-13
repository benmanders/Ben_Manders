
void calibrateMagnetometer(unsigned long calibrationTime)
{
  Serial.println("Let's Start Calibrating");
  float minX = 0, maxX = 0, minY = 0, maxY = 0, minZ = 0, maxZ = 0;

  bool keepTesting = true;
  unsigned long calibrateTimer = millis();
  sensors_event_t a, m, g, temp;
  while (keepTesting)
  {
    lsm.read();
    lsm.getEvent(&a, &m, &g, &temp);

    float inX = m.magnetic.x;
    float inY = m.magnetic.y;
    float inZ = m.magnetic.z;

    if (inX < minX )
    {
      calibrateTimer = millis();
      minX = inX;
    }
    if (inX > maxX )
    {
      calibrateTimer = millis();
      maxX = inX;
    }
    if (inY < minY )
    {
      calibrateTimer = millis();
      minY = inY;
    }
    if (inY > maxY )
    {
      calibrateTimer = millis();
      maxY = inY;
    }
    if (inZ < minZ )
    {
      calibrateTimer = millis();
      minZ = inZ;
    }
    if (inZ > maxZ )
    {
      calibrateTimer = millis();
      maxZ = inZ;
    }

    if (millis() - calibrateTimer > calibrationTime)
      keepTesting = false;
  }

  offsetX = (minX + maxX) / 2;
  offsetY = (minY + maxY) / 2;
  offsetZ = (minZ + maxZ) / 2;

  scaleX = (maxX - minX) / 2;
  scaleY = (maxY - minY) / 2;
  scaleZ = (maxZ - minZ) / 2;

  Serial.println("Offsets:");
  Serial.println("X \t\t Y \t\t Z");
  Serial.print(offsetX, 3); Serial.print("\t\t");
  Serial.print(offsetY, 3); Serial.print("\t\t");
  Serial.print(offsetZ, 3); Serial.print("\t\t");
  Serial.println();

  Serial.println("Scales:");
  Serial.println("X \t\t Y \t\t Z");
  Serial.print(scaleX, 3); Serial.print("\t\t");
  Serial.print(scaleY, 3); Serial.print("\t\t");
  Serial.print(scaleZ, 3); Serial.print("\t\t");
  Serial.println();

  Serial.println("Normalising:");
  Serial.println("X \t\t Y \t\t Z");
  Serial.print((maxX - offsetX)/scaleX, 3); Serial.print("\t\t");
  Serial.print((maxY - offsetY)/scaleY, 3); Serial.print("\t\t");
  Serial.print((maxZ - offsetZ)/scaleZ, 3); Serial.print("\t\t");
  Serial.println();
}

void normaliseGauss(sensors_event_t &mag)
{
  mag.magnetic.x = (mag.magnetic.x - offsetX)/scaleX;
  mag.magnetic.y = (mag.magnetic.y - offsetY)/scaleY;
  mag.magnetic.z = (mag.magnetic.z - offsetZ)/scaleZ;
}

//------------------------------------------------------------------------------
/*
  EEPROM

  - store calibration values
*/
