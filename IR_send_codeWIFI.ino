
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>


#include <ArduinoOTA.h>

#include <ESP8266HTTPClient.h>

#include <WiFiUdp.h>
#include <NTPClient.h>
//#include <DNSServer.h>



#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>

const uint16_t kIrLed = 4;  // ESP8266 GPIO pin to use. Recommended: 4 (D2).
IRsend irsend(kIrLed);  // Set the GPIO to be used to sending the message.

int incomingByte = 0;

WiFiClient client;
ESP8266WebServer server(80);

//DNSServer dnsServer;
//const byte DNS_PORT = 53;



void setup() {
  Serial.begin(9600);
  irsend.begin();
  Serial.println("ESP booting.");





wifi_on();

server.begin();

server.onNotFound([]() {
    if (!handleAjax(server.uri()))
            handleNotFound();
    });

arduino_OTA_func();
}
void loop() {



if (Serial.available() > 0) {
    // read the incoming byte:
    incomingByte = Serial.read();

    if(incomingByte==49){send(0xD88);} // 1 - time     0xD88 
    if(incomingByte==50){send(0xDCC);} // 2 - silent night  0xDCC
    if(incomingByte==51){send(0xDA0);} // 3 - turbo boost   0xDA0
    if(incomingByte==52){send(0xD82);} // 4 - +/-       0xD82
    if(incomingByte==53){send(0xD84);} // 5 - decrescendo   0xD84
    if(incomingByte==54){send(0xD81);} // 6 - on/off      0xD81
  }


server.handleClient();
ArduinoOTA.handle();
}

void send(int code)
{
  Serial.println("send "+String(code));
  for(int i=0;i<3;i++)
    irsend.sendSymphony(code, 12, 0);
}





void wifi_on()
{
int wait_for_wifi_count = 0;

  WiFi.begin("___WIFI_NAME___", "___WIFI_PASSWORD___");
    while (WiFi.status() != WL_CONNECTED) {


      wait_for_wifi_count++;
      
      if(wait_for_wifi_count>40) // +- try for 20secs
        {
        Serial.println("Can't connect to WiFi...");
        return;
        }
    delay(500);
    Serial.print(".");
  }

 

Serial.println("Wifi IP address:");
Serial.println(WiFi.localIP());

}


bool handleAjax(String path) {

path.remove(0, 1);



if (path == "send_time")
    {
    send(0xD88);
    server.send(200, "text/plain", "ok");
    }
else if (path == "send_silent_night")
    {
    send(0xDCC);
    server.send(200, "text/plain", "ok");
    }
else if (path == "send_turbo_boost")
    {
    send(0xDA0);
    server.send(200, "text/plain", "ok");
    }
else if (path == "send_more_less")
    {
    send(0xD82);
    server.send(200, "text/plain", "ok");
    }
else if (path == "send_decrescendo")
    {
    send(0xD84);
    server.send(200, "text/plain", "ok");
    }
else if (path == "send_onoff")
    {
    send(0xD81);
    server.send(200, "text/plain", "ok");
    }
else if (path == "ping")
    {
    server.send(200, "text/plain", "ok");
    }
else
  return false;
  
return true;
}



void handleNotFound() {
  if (isCaptivePortal()) { // If caprive portal redirect instead of displaying the error page.
    return;
  }
  String message = F("File Not Found\n\n");
  message += F("URI: ");
  message += server.uri();
  message += F("\nMethod: ");
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += F("\nArguments: ");
  message += server.args();
  message += F("\n");

  for (uint8_t i = 0; i < server.args(); i++) {
    message += String(F(" ")) + server.argName(i) + F(": ") + server.arg(i) + F("\n");
  }
  server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  server.sendHeader("Pragma", "no-cache");
  server.sendHeader("Expires", "-1");
  server.send(404, "text/plain", message);
}

boolean isCaptivePortal() {
  if (!isIp(server.hostHeader()) && server.hostHeader() != "Rowenta") {
    Serial.println("Request redirected to captive portal for "+server.uri());
    server.sendHeader("Location", String("http://") + toStringIp(server.client().localIP()), true);
    server.send(302, "text/plain", "");   // Empty content inhibits Content-length header so we have to close the socket ourselves.
    server.client().stop(); // Stop is needed because we sent no content length
    return true;
  }
  return false;
}

void arduino_OTA_func()
{
//ArduinoOTA.setPassword((const char *)"123");
ArduinoOTA.setHostname("Rowenta");

  ArduinoOTA.onStart([]() {
    Serial.println("OTA Start");
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
}

void reboot()
{
ESP.restart();
}


String get_list_ssid()
{
String lst_wifi="";

int n = WiFi.scanNetworks();
 
if(n >= 0)
{
  for (int i = 0; i < n; i++)
  {
    lst_wifi += WiFi.SSID(i)+"\t"+WiFi.RSSI(i)+"\t"+WiFi.encryptionType(i)+"\r";
  }
  WiFi.scanDelete();
}

return lst_wifi;
}



boolean isIp(String str) {
  for (size_t i = 0; i < str.length(); i++) {
    int c = str.charAt(i);
    if (c != '.' && (c < '0' || c > '9')) {
      return false;
    }
  }
  return true;
}

String toStringIp(IPAddress ip) {
  String res = "";
  for (int i = 0; i < 3; i++) {
    res += String((ip >> (8 * i)) & 0xFF) + ".";
  }
  res += String(((ip >> 8 * 3)) & 0xFF);
  return res;
}
