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
float prev;
float x, y, z, pitch, roll;

void setup() {
  Serial.begin(115200);
  pinMode(flexPin, INPUT);  //Setup the pin as input
  for(int i = 0;i<3;i++) {
    pinMode(selectedPin[i], OUTPUT);
    digitalWrite(selectedPin[i], HIGH);
  }

  //Connect
  WiFi.mode(WIFI_AP);
  if(!WiFi.softAP(WiFi_SSID, WiFi_PASS)){
    Serial.println("Netzwerk konnte nicht erstellt werden.");
    return;
  }
  server.begin();
  Serial.printf("Lokale ip ist: ");
  Serial.println(WiFi.softAPIP());
  Serial.println("Warte auf einen neuen Client: ..");
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
}

void loop() {
  if(client == NULL)
    client = server.available();
  if(!client){
    Serial.println("no client connected!");
    delay(1000);
    return;
  }
  read_mpu(); 
  process_data();
  send_infomation();
  while(micros() - loop_timer < 4000);                    
    loop_timer = micros();     
}

void send_infomation(){
  char buf[1024];
  sprintf(buf, "%f;%f;%f;%i;%i;%i;%i;%i", x, y, z, flexValue[0], flexValue[1], flexValue[2], flexValue[3], flexValue[4]);
  client.println(buf);
  client.flush();
  Serial.println("sending data..."); 
}

float round_float(float f, byte base, byte dez){
  return (round(f * pow(10, dez) / base)) / pow(10, dez) * base;
}

void process_data(){

  float vector = sqrt(acc_x * acc_x + acc_y * acc_y + acc_z * acc_z);
  float angle_pitch = asin((float)acc_y/vector) * 180.0 / 3.141592653f;
  float angle_roll = asin((float)acc_x/vector) * 180.0 / -3.141592653f;
  pitch = pitch * 0.9 + angle_pitch * 0.1;
  roll = roll * 0.9 + angle_roll * 0.1;
  float p = round_float(pitch, 1, 0);
  float r = round_float(roll, 1, 0);
  float tolerance = 15;
  if(abs(p) < tolerance)
    p = 0;
  if(abs(r) < tolerance)
    r = 0;
  if(flexValue[0] >= 80){
    z -= p / 10000;
  }else{
    x += p / 10000;
  }
  y += r / 10000;
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
  int gyro_x = read_data();  
  int gyro_y = read_data();  
  int gyro_z = read_data();  

   for(int pin = 0;pin < 5;pin++) { //all the analog sensors
      for(int i = 0; i < 3;i++) {   //the digital pins to switch between the different sensor via the mulitplexer pcb 74HC4051
         digitalWrite(selectedPin[i], (pin >> i & 1) ? HIGH : LOW);    //Write the Values 1 or 0 --> Verunden von bzw 1(001) & S1(001) = 1 || 1(001) & S2(010) = 0
      }
      flexValue[pin] = map(analogRead(flexPin), 0, 1024, 0, 100);   //read the Value from the selected pin
   }
}

int16_t read_data(){
  return Wire.read()<<8|Wire.read();
}
