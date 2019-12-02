void updateThingspeak()
{
    float lat = tinyGPS.location.lat();
    ThingSpeak.setField(1,lat);
    float lng = tinyGPS.location.lng();
    ThingSpeak.setField(2,lng);
    int resp = ThingSpeak.writeFields(WriteChannelNumber, myWriteAPIKey);
    Serial.print("Current Location = ");
    Serial.print(lat);
    Serial.print(" , ");
    Serial.println(lng);   
}      
void readThingspeak()
{
  long targetLat = ThingSpeak.readLongField(ReadChannelNumber, 1, myReadAPIKey);
  long targetLng = ThingSpeak.readLongField(ReadChannelNumber, 2, myReadAPIKey);
  Serial.print("target Location = ");
  Serial.print(targetLat);
  Serial.print(" , ");
  Serial.println(targetLng);
}

