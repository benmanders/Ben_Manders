void updateThisLatLng()
{
  if ((lastLog + gpsUpdateRate) < millis())
  {
    if (tinyGPS.location.isUpdated())
    {
      thisLng = tinyGPS.location.lng();
      thisLat = tinyGPS.location.lat();
    }
    else // If GPS data isn't valid
    {

    }
  }

}

float getTargetHeading(float lat1, float lng1, float lat2, float lng2)
{
  return getHeadingDegrees(lat1, lng1, lat2, lng2) + 180.0f;
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
