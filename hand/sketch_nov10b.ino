
#include <ESP8266WiFi.h>
#include <WiFiUDP.h>
#include <Wire.h>

//Wifi data
#define WiFi_SSID "hand"
#define WiFi_PASS "einfachbesser"
#define PORT 1337

//UDP
WiFiServer server(1337);
WiFiClient client;

//FlexSensor
const int flexPin = A0; //The AnalogPin that reads the different values 
const int selectedPin[3] = {D6, D5, D0}; //The pins for the InputState (maybe not the right pins)
int flexValue[5];    //Container for all the flexValues

//Gyros
long loop_timer = 0;
int16_t acc_x, acc_y, acc_z;
int gyro_x, gyro_y, gyro_z;

void setup() {
  Serial.begin(115200);
  
  //Setup the pins for the flex sensor
  pinMode(flexPin, INPUT);  //Setup the analog pin
   for(int i = 0;i<3;i++) {
      pinMode(D1, OUTPUT);
      digitalWrite(selectedPin[i], HIGH);
   }
  
  //Connect
  WiFi.mode(WIFI_AP);
  if(!WiFi.softAP(WiFi_SSID, WiFi_PASS)){
    Serial.println("Netzwerk konnte nicht erstellt werden.");
    return;
  }
  server.begin();
  Serial.printf("Lokale ip ist: %s \n",WiFi.softAPIP());
  Serial.println("Warte auf einen neuen Client: ..");
  client = server.available();
  //starting the connection 
  Wire.begin(); 
  Wire.beginTransmission(0x68);
  Wire.write(0x6B);  
  Wire.write(0);     
  Wire.endTransmission(true);                              
  Wire.beginTransmission(0x68);                                        
  Wire.write(0x1C);                                                    
  Wire.write(0x10);                                                    
  Wire.endTransmission(true);
  Wire.beginTransmission(0x68);                                        
  Wire.write(0x1B);                                                    
  Wire.write(0x08);                                                    
  Wire.endTransmission(true);                                              
  Serial.println("mpu configured");
  Serial.println("start reading");
  
  Serial.println("F1\tF2\tF3\tF4\tF5\tx1\ty1\tz1\tTe1");   //Create a table for the values
  Serial.println("---\t---\t---\t---\t---\t---\t---\t---\t---\t");
}

void loop() {
  read_mpu(); 
  /*for(int pin = 0;pin < 5;pin++) { //all the analog sensors
      for(int i = 0; i < 3;i++) {   //the digital pins to switch between the different sensor via the mulitplexer pcb 74HC4051
         digitalWrite(selectedPin[i], (pin >> i & 1) ? HIGH : LOW);    //Write the Values 1 or 0 --> Verunden von bzw 1(001) & S1(001) = 1 || 1(001) & S2(010) = 0
      }
      flexValue[pin] = analogRead(flexPin);   //read the Value from the selected pin
      flexValue[pin] = map(flexValue[pin],0,1023,0,100); //calculate the procentage
      Serial.print(flexValue[pin] + "\t");    //print the value
  }*/

  sent(buf);
  while(micros() - loop_timer < 4000);                    
  loop_timer = micros();     
}

void send_infomation(){
  char buf[255];
  sprintf(buf, "%i;%i;%i;", acc_x, acc_y, acc_z);
  client.println(buf);
  client.flush();
}

//read the mpu data
void read_mpu(){
  Wire.beginTransmission(0x68);
  Wire.write(0x3B);  
  Wire.endTransmission(true);                              
  Wire.requestFrom(0x68, 14, true);
  while(Wire.available() < 14);     
  acc_x = read_data();  
  acc_y = read_data();  
  acc_z = read_data();  
  
  int temp = read_data();  
  gyro_x = read_data();  
  gyro_y = read_data();  
  gyro_z = read_data();  
}

int16_t read_data(){
  return Wire.read()<<8|Wire.read();
}
