void updateThingspeak()
{
    float lat = tinyGPS.location.lat();
    ThingSpeak.setField(1,lat);
    float lng = tinyGPS.location.lng();
    ThingSpeak.setField(2,lng);
    int resp = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
    Serial.println("sent to thingspeak");
}      
