void displayLED(int bearingDeg)
{
  int LedPos = map(bearingDeg, 0, 360, 0, 15);
  leds[ledPos] = CHSV(130, 255, 192);
  FastLED.show();
  fill_solid(leds, NUM_LEDS, CRGB :: Black);
  FastLED.show();
}
