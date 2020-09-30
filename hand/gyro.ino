#include <Wire.h>
int pin = 5;
long loop_timer = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Wire.begin(); 
  Wire.beginTransmission(pin);
  Wire.write(0x1C);                                                    //Send the requested starting register
  Wire.write(0x10);                                                    //Set the requested starting register
  Wire.endTransmission();   
}

void loop() {
  // put your main code here, to run repeatedly:
  Wire.beginTransmission(pin);                                        //Start communicating with the MPU-6050
  Wire.write(0x3B);                                                    //Send the requested starting register
  Wire.endTransmission();                                              //End the transmission
  Wire.requestFrom(pin, 14);                                           //Request 14 bytes from the MPU-6050
  while(Wire.available() < 14);                                        //Wait until all the bytes are received
  int acc_x = Wire.read()<<8|Wire.read();                                  //Add the low and high byte to the acc_x variable
  int acc_y = Wire.read()<<8|Wire.read();                                  //Add the low and high byte to the acc_y variable
  int acc_z = Wire.read()<<8|Wire.read(); 

  Serial.print("X: ");
  Serial.print(acc_x);
  Serial.print("Y: ");
  Serial.print(acc_y);
  Serial.print("Z: ");
  Serial.print(acc_z);
  Serial.println();

  while(micros() - loop_timer < 4000);                                 //Wait until the loop_timer reaches 4000us (250Hz) before starting the next loop
  loop_timer = micros();     
}
