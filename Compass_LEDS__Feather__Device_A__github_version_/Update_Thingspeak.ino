void updateThingspeak()
{
  static unsigned long uploadTimer = 0;
  if (millis() - uploadTimer > thingSpeakUpdateRate)
  {    
    ThingSpeak.setField(1, thisLat);    
    ThingSpeak.setField(2, thisLng);
    int resp = ThingSpeak.writeFields(WriteChannelNumber, myWriteAPIKey);   
  }
}

void updateTargetLatLng()
{
  targetLat = ThingSpeak.readFloatField(ReadChannelNumber, 1, myReadAPIKey);
  targetLng = ThingSpeak.readFloatField(ReadChannelNumber, 2, myReadAPIKey);
}
