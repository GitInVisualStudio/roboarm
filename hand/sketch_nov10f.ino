#include <ESP8266WiFi.h>
#include <WiFiUDP.h>

//Wifi data
/*#define WiFi_SSID "JPRS-C13"
#define WiFi_Pass "internet@schule1833"*/
const char* WiFi_SSID = "JPRS-C13";
const char* WiFi_Pass = "internet@schule1833";
const int UDP_PORT = 44444;

//UDP
WiFiServer server(1337);
char packet[255];
//char reply[] = "Packet Recieved!"
char recieve[255];
void (threads*)();

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_AP);
  if(!WiFi.softAP("hand", "dunuttensohn")){
    Serial.println("nicht geschafft");
    return;
  }
  server.begin();
  Serial.println(WiFi.softAPIP());
  Serial.println("erfolgreich");
}

void loop() {
  delay(3000);
  Serial.printf("Anzahl der Verbunden Geräte %i\n", WiFi.softAPgetStationNum());
  WiFiClient client = server.available();
  if(!client){
    return;
  }
  Serial.println("new client connected!");
  client.println("test");
  client.flush();
  client.close();
}
  
  //class for sending the data
/*void sent(char data[]) {
  UDP.beginPacket("192.168.178.86",UDP_PORT);
  UDP.write(data);
  UDP.endPacket();
}*/
