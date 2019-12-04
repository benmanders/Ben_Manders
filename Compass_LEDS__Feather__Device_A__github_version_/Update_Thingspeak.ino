void updateThingspeak()
{
  static unsigned long uploadTimer = 0;
  if (millis() - uploadTimer > thingSpeakUpdateRate)
  {    
    ThingSpeak.setField(1, lat);    
    ThingSpeak.setField(2, lng);
    int resp = ThingSpeak.writeFields(WriteChannelNumber, myWriteAPIKey);   
  }
}
void readThingspeak()
{
  targetLat = ThingSpeak.readFloatField(ReadChannelNumber, 1, myReadAPIKey);
  targetLng = ThingSpeak.readFloatField(ReadChannelNumber, 2, myReadAPIKey);
}
