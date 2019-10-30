void displayLED(int bearingDeg)
{
  int ledPos = map(bearingDeg, 0, 360, 0, 15);
  fill_solid(leds, NUM_LEDS, CRGB :: Black);
  leds[ledPos] = CHSV(130, 255, 192);
  FastLED.show();
  leds[ledPos] = CHSV(130, 255, 192);
  FastLED.show();
}
