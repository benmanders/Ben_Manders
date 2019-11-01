
void calibrateMagnetometer(unsigned long calibrationTime)
{
    Serial.println("Let's Start Calibrating");
    float minX = 0,maxX = 0,minY = 0,maxY = 0,minZ = 0,maxZ = 0;

    bool keepTesting = true;
    unsigned long calibrateTimer = millis();
    sensors_event_t a, m, g, temp;
    while(keepTesting)
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

        if(millis() - calibrateTimer > calibrationTime)
            keepTesting = false;
    }

    offsetX = (minX + maxX) / 2;
    offsetY = (minY + maxY) / 2;
    offsetZ = (minZ + maxZ) / 2;

    Serial.println("Offsets:");
    Serial.println("X \t\t Y \t\t Z");
    Serial.print(offsetX, 3); Serial.print("\t\t");
    Serial.print(offsetY, 3); Serial.print("\t\t");
    Serial.print(offsetZ, 3); Serial.print("\t\t");
    Serial.println();
    /*
       find a Max and minimum√
       - x
       - y
       - z
        - store cureent Max and Min √
        - test new values and over wrtite if needed√
        - filter or average the data

       - caluculate offset from max and min √
        - (max + min)/2 mean value √

       - m.magnetic.xyz - offset

       - prompt to say we're calibrarting √

       - test if we're finished calibrarting √

        - print out as well √
     */
}
//------------------------------------------------------------------------------
/*
EEPROM

- store calibration values
*/
