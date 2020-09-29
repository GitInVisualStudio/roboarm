#include <ESP8266WiFi.h>
#include <WiFiUDP.h>

//Wifi data
#define WiFi_SSID "Your SSiD"
#define WiFi_Pass "Your Pass"
#define UDP_PORT 4269

//UDP
WiFiUDP UDP;
char packet[255];
//char reply[] = "Packet Recieved!"
char recieve[255];

void setup() {
  Serial.begin(115200);

  WiFi.begin(WiFiSSID, WiFiPass);

  //Connecting...
  Serial.print("Connect: ");
  Serial.print(WiFiSSID);
  Serial.println("Connecting..")

  while(WiFi.status != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }

  //Connected to WiFi_SSID
  Serial.println();
  Serial.print("Connected!The IP: ");
  Serial.print(WiFi.localIP());

  //UDP listining
  UDP.begin(UDP_PORT);
  Serial.print("Listening on UDP Port: ");
  Serial.print(UDP_PORT);
}

void loop() {
  //example data 
  packet[0] = '1';
  packet[1] = '2';
  packet[2] = '3';
  
  //Send packet
  sent(packet);
}

void sent(char[] data) {
  UDP.beginPacket(UDP.remoteIP(),UDP.remotePort());
  UDP.write(data);
  UDP.endPacket();
}
