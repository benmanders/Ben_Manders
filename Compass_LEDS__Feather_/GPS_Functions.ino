void updateGPS()
{
  if ((lastLog + gpsUpdateRate) < millis())
  {
    if (tinyGPS.location.isUpdated())
    {
      float lng = tinyGPS.location.lng();
      ThingSpeak.setField(1,lng);
      float lat = tinyGPS.location.lat();
      //      float heading = atan2(lng - targetLng, lat - targetLat) * piToDegrees;
      float heading = getHeadingDegrees(lat, lng, targetLat, targetLng);
      float distance = distanceBetweenPoints(lat, lng, targetLat, targetLng); // this is no global

      Serial.print(lat, 4);
      Serial.print(" , ");
      Serial.print(lng, 4);
      Serial.println(" , ");
      Serial.print(distance, 4);
      Serial.println(" , ");
      Serial.println(heading, 4);
      Serial.println(" ... ");
    }
  }
  else // If GPS data isn't valid
  {

  }
}
float getTargetHeading()
{
  static float lastHeading = 0.0f;
  if (tinyGPS.location.isUpdated())
  {
    float lng = tinyGPS.location.lng();
    float lat = tinyGPS.location.lat();
    //      float heading = atan2(lng - targetLng, lat - targetLat) * piToDegrees;
    lastHeading = getHeadingDegrees(lat, lng, targetLat, targetLng);
//    distance = distanceBetweenPoints(lat, lng, targetLat, targetLng); // this is no global
  }
  return lastHeading + 180.0f;
}
float distanceBetweenPoints(float currLat, float currLng, float tarLat, float tarLng)
{
  const static float radiusOfEarth = 6371.0f;
  float dLat = deg2rad(currLat - tarLat);
  float dLng = deg2rad(currLng - tarLng);
  float a = (sin(dLat / 2) * sin(dLat / 2))
  + ((sin(dLng / 2) * sin(dLng / 2))
  * (cos(deg2rad(currLat)) * cos(deg2rad(tarLat))));
  float c = 2 * atan2(sqrt(a), sqrt(1 - a));
  float distanceKm = c * radiusOfEarth;
  return distanceKm;
}

////////////////////////////////////

float getHeadingDegrees(float lat1, float lng1, float lat2, float lng2)
{
  float dLng = (lng2 - lng1);
  float y = sin(dLng) * cos(lat2);
  float x = (cos(lat1) * sin(lat2)) - (sin(lat1) * cos(lat2) * cos(dLng));
  float bearingRad = atan2(y, x);

  int bearingDeg = rad2deg(bearingRad);

  bearingDeg = (bearingDeg  + 360) % 360;
  bearingDeg = 360 - bearingDeg;
  return bearingDeg;
}

////////////////////////////////////

float deg2rad(float deg)
{
  return deg * piToDegrees;
}

float rad2deg(float rad)
{
  return rad / piToDegrees;
}

////////////////////////////////////
