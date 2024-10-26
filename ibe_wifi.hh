#ifndef IBE_WIFI
#define IBE_WIFI

#include <WiFi.h>

WiFiUDP ntpUDP;
NTPClient _timeClient(ntpUDP);


bool wifiCredAvailable(){
  getWifiUser();
  return strlen(_wifiUser.uname) > 0;
}

bool wifiInit()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(_wifiUser.uname, _wifiUser.passwd);

  long start_wifi_millis = millis();
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    delay(2000);
    Serial.print(".");
    if (millis() - start_wifi_millis > 20000) {
      WiFi.disconnect(true);
      Serial.println("-F-");
      return false;
    }
  }
  byte mac[6];
  WiFi.macAddress(mac);
  _deviceId =  String(mac[0],HEX) +String(mac[1],HEX) +String(mac[2],HEX) +String(mac[3],HEX) + String(mac[4],HEX) + String(mac[5],HEX);
  Serial.println(_deviceId);
  
  Serial.println(WiFi.localIP());
  _localIp = WiFi.localIP();
  _timeClient.begin();
  _timeClient.update();
  Serial.println(_tz);
  _timeClient.setTimeOffset(timeOffset(_tz));
  return true;
}

// https://github.com/arduino-libraries/NTPClient/issues/36
void wifiGetTimeStampString() {
   time_t rawtime = _timeClient.getEpochTime();
   struct tm * ti;
   ti = localtime (&rawtime);
 
   char *ts = _ts;
  
   uint16_t year = ti->tm_year + 1900;
   uint8_t month = ti->tm_mon + 1;
   uint8_t day = ti->tm_mday;
   uint8_t hours = ti->tm_hour;
   uint8_t minutes = ti->tm_min;
   uint8_t seconds = ti->tm_sec;

   uint16_t milli = millis()%1000;

  ts += sprintf(ts, "%04u%02u%02u%02u%02u%02u.%03u", year, month, day, hours, minutes, seconds, milli);
  *ts++ = 0;
}



#endif
