void setupWifi()
{
  WiFi.setPins(8, 7, 4, 2); // Setup the WiFi on the Feather boards
  /* Start the WiFi connection */
  Serial.println("Starting...");
  Serial.println("Connecting to WiFi");
  int conn = WiFi.begin(ssid, pass);
  if ( conn == WL_CONNECTED )        {
    Serial.println("OK!");
  }
  else if ( conn == WL_IDLE_STATUS ) {
    Serial.println("Idle");
  }
  else                              {
    Serial.println("Unknown response");
  }
}

bool isWiFiConnected()
{
  return WiFi.status() != WL_CONNECTED;
}
