#include <Wire.h>
int pin = 20; //d1 = 20, d2 = 19
long loop_timer = 0;
float x, y, z;
int acc_x, acc_y, acc_z;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Wire.begin(); 
  Serial.println("setting up...");
  Wire.beginTransmission(0x68);                            
  Wire.write(0x6B);                                        
  Wire.write(0x00);                                        
  Wire.endTransmission();                                  
  //Configure the accelerometer (+/-8g)
  Wire.beginTransmission(0x68);                            
  Wire.write(0x1C);                                        
  Wire.write(0x10);                                        
  Wire.endTransmission();                                  
  //Configure the gyro (500dps full scale)
  Wire.beginTransmission(0x68);                            
  Wire.write(0x1B);                                        
  Wire.write(0x08);                                        
  Wire.endTransmission();                                 
  Serial.println("mpu configured");
}

void loop() {
  read_mpu_6050_data();
  float beta = 0.9;
  x = x * beta + (acc_x / 255.0f * (1 - beta));
  y = y * beta + (acc_y / 255.0f * (1 - beta));
  z = z * beta + (acc_z / 255.0f * (1 - beta));
  Serial.println();
  while(micros() - loop_timer < 4000);                    
  loop_timer = micros();     
}

int read_data(){
  int positiv = Wire.read() << 8;
  int negativ = Wire.read();
  return positiv - negativ;
}
  

void read_mpu_6050_data(){                                
  Wire.beginTransmission(0x68);                           
  Wire.write(0x3B);                                       
  Wire.endTransmission();                                 
  Wire.requestFrom(0x68,14);                              
  while(Wire.available() < 14);                           
  acc_x = read_data();
  acc_y = read_data();
  acc_z = read_data();
  int temperature = Wire.read()<<8|Wire.read();                          
  int gyro_x = Wire.read()<<8|Wire.read();                               
  int gyro_y = Wire.read()<<8|Wire.read();                                
  int gyro_z = Wire.read()<<8|Wire.read();                                 
}
