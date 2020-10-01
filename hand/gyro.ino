#include <Wire.h>
int pin = 20; //d1 = 20, d2 = 19
long loop_timer = 0;
float x, y, z;
int16_t acc_x, acc_y, acc_z;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Wire.begin(); 
  Wire.beginTransmission(0x68);
  Wire.write(0x6B);  
  Wire.write(0);     
  Wire.endTransmission(true);                              
  Serial.println("mpu configured");
}

void loop() {
  
  Wire.beginTransmission(0x68);
  Wire.write(0x3B);  
  Wire.endTransmission(true);                              
  Wire.requestFrom(MPU_addr,14,true);
  
  acc_x = Wire.read()<<8|Wire.read();  
  acc_x = Wire.read()<<8|Wire.read();  
  acc_x = Wire.read()<<8|Wire.read();  
  
  int temp = Wire.read()<<8|Wire.read();  
  int gyro_x = Wire.read()<<8|Wire.read();  
  int gyro_y = Wire.read()<<8|Wire.read();  
  int gyro_z = Wire.read()<<8|Wire.read();  
  
  float beta = 0.9;
  x = x * beta + (acc_x / 255.0f * (1 - beta));
  y = y * beta + (acc_y / 255.0f * (1 - beta));
  z = z * beta + (acc_z / 255.0f * (1 - beta));
  
  Serial.println(x);
  
  /*while(micros() - loop_timer < 4000);                    
  loop_timer = micros();     */
}
