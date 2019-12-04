void setPixelToCompassDirection(float heading, uint32_t colour)
{
  int pixelIndex = 0;

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
  pixels.setPixelColor(2 * pixelIndex, colour);
  pixels.setPixelColor((2 * pixelIndex) + 1, colour);  
}

void setNorthPixel(float ourHeading)
{
  setPixelToCompassDirection(getHeadingDiff(0.0f, ourHeading), pixels.Color(150, 150, 0));
}
