#include <ESP8266WiFi.h>
#include <WiFiUDP.h>

//Wifi data
#define WiFi_SSID "Your SSiD"
#define WiFi_Pass "Your Pass"
#define UDP_PORT 44444
IPAdress ip(192, 168, 178, 86);

//UDP
WiFiUDP UDP;
char packet[255];
char recieve[255];

//FlexSensor
const int flexPin = A0; //The AnalogPin that reads the different values 
const int selectedPin[3] = {D1, D2, D3}; //The pins for the InputState (maybe not the right pins)
int flexValue[5];    //Container for all the flexValues

void setup() {
  Serial.begin(115200);
  
  //Setup the pins for the flex sensor
  pinMode(flexPin, INPUT);  //Setup the analog pin
   for(int i = 0;i<3;i++) {
      pinMode(selectedPin[i], OUTPUT);
      digitialWrite(selectedPin[i], HIGH)
   }
  
  //Connect
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
  
  Serial.println("F1\F2\F3\F4\F5\x\y\z");   //Create a table for the values
  Serial.println(---\t---\t---\t---\t---\t---\t---\t---\t);
}

void loop() {
  
  for(int pin = 0;pin < 5;pin++) { //all the analog sensors
      for(int i = 0; i < 3;i++) {   //the digital pins to switch between the different sensor via the mulitplexer pcb 74HC4051
         digitialWrite(selectedPin[i], (pin >> i & 1) ? HIGH : LOW);    //Write the Values 1 or 0 --> Verunden von bzw 1(001) & S1(001) = 1 || 1(001) & S2(010) = 0
      }
      flexValue[pin] = analogRead(flexPin)   //read the Value from the selected pin
      flexSensor = map(flexSensor,0,1023,0,100); //calculate the procentage
      Serial.print(flexValue[pin] + "\t")    //print the value
  }
  Serial.print("1" + "\t"); //Testdata for x,y,z
  Serial.print("2" + "\t");
  Serial.print("3" + "\t");
  
  Serial.print("\t" + "Sent")
  sent("Hello from the ESP"); //Send packet
  
  Serial.println("");
  Delay(500);
}
  
//class for sending the data
void sent(char data[]) {
  UDP.beginPacket(ip,44444);
  UDP.write(data);
  UDP.endPacket();
}
