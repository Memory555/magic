#include <NTPClient.h>
// change next line to use with another board/shield
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

const char *ssid = "玛卡巴卡";
const char *password = "123123123";
// const char *WIFI_SSID = "玛卡巴卡"; 
// const char *WIFI_PWD = "123123123"; 

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

void setup(){
  Serial.begin(115200);

  WiFi.begin(ssid, password);

  while ( WiFi.status() != WL_CONNECTED ) {
    delay (500);
    Serial.print (".");
  }
  timeClient.setTimeOffset(60*60*8);	//东八区时间设置
  timeClient.begin();
}

void loop() {
  timeClient.update();
  Serial.println(timeClient.getFormattedTime());
  delay(1000);
}
