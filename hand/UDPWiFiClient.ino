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

  WiFi.begin(WiFi_SSID, WiFi_Pass);

  //Connecting...
  Serial.print("Connect: ");
  Serial.println(WiFi_SSID);
  Serial.print("Connecting..");

  while(WiFi.status() != WL_CONNECTED) {
    delay(300);
    Serial.print(".");
  }

  //Connected to WiFi_SSID
  Serial.println();
  Serial.print("Connected!The IP: ");
  Serial.println(WiFi.localIP());

  //UDP listining
  UDP.begin(UDP_PORT);
  Serial.print("Sending on UDP Port: ");
  Serial.println(UDP_PORT);
  
    //example data 
  packet[0] = '1';
  packet[1] = '2';
  packet[2] = '3';
}

void loop() {
  
  delay(1000);
  Serial.println("Sending")
  //Send packet
  sent(packet);
}
  
  //class for sending the data
void sent(char data[]) {
  UDP.beginPacket(UDP.remoteIP(),UDP.remotePort());
  UDP.write(data);
  UDP.endPacket();
}
