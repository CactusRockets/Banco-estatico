// Getting_the_Boards_MAC_Address
//----------------------------------------Load libraries
#include "WiFi.h"
//----------------------------------------

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  WiFi.mode(WIFI_MODE_STA);
  Serial.println(WiFi.macAddress());
}

void loop() {
  
}
