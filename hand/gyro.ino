#include <Wire.h>
int pin = 20; //d1 = 20, d2 = 19
long loop_timer = 0;
int acc_x, acc_y, acc_z;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Wire.begin(); 
  Serial.println("setting up...");
  Wire.beginTransmission(0x68);                                        //Start communicating with the MPU-6050
  Wire.write(0x6B);                                                    //Send the requested starting register
  Wire.write(0x00);                                                    //Set the requested starting register
  Wire.endTransmission();                                              //End the transmission
  //Configure the accelerometer (+/-8g)
  Wire.beginTransmission(0x68);                                        //Start communicating with the MPU-6050
  Wire.write(0x1C);                                                    //Send the requested starting register
  Wire.write(0x10);                                                    //Set the requested starting register
  Wire.endTransmission();                                              //End the transmission
  //Configure the gyro (500dps full scale)
  Wire.beginTransmission(0x68);                                        //Start communicating with the MPU-6050
  Wire.write(0x1B);                                                    //Send the requested starting register
  Wire.write(0x08);                                                    //Set the requested starting register
  Wire.endTransmission();                                              //End the transmission
  Serial.println("mpu configured");
}

float x, y, z;

void loop() {
  read_mpu_6050_data();
  float beta = 0.9;
  x = x * beta + (acc_x / 255.0f * (1 - beta));
  y = y * beta + (acc_y / 255.0f * (1 - beta));
  z = z * beta + (acc_z / 255.0f * (1 - beta));

  /*Serial.print("X: ");
  Serial.print(x);*/
  /*Serial.print("\t\t");
  Serial.print("Y: ");
  Serial.print(acc_y);
  Serial.print("\t\t");
  Serial.print("Z: ");
  Serial.print(acc_z);*/
  Serial.println();
  while(micros() - loop_timer < 4000);                                 //Wait until the loop_timer reaches 4000us (250Hz) before starting the next loop
  loop_timer = micros();     
}

void read_mpu_6050_data(){                                             //Subroutine for reading the raw gyro and accelerometer data
  Wire.beginTransmission(0x68);                                        //Start communicating with the MPU-6050
  Wire.write(0x3B);                                                    //Send the requested starting register
  Wire.endTransmission();                                              //End the transmission
  Wire.requestFrom(0x68,14);                                           //Request 14 bytes from the MPU-6050
  while(Wire.available() < 14);                                        //Wait until all the bytes are received
  //acc_x = Wire.read()<<8;                                  //Add the low and high byte to the acc_x variable#
  int k = Wire.read();
  int l = Wire.read();
  Serial.print(k/255.0f);
  Serial.print("    ");
  Serial.print(l/255.0f);
  Serial.println();
  
  acc_y = Wire.read()<<8|Wire.read();                                  //Add the low and high byte to the acc_y variable
  acc_z = Wire.read()<<8|Wire.read();                                  //Add the low and high byte to the acc_z variable
  int temperature = Wire.read()<<8|Wire.read();                            //Add the low and high byte to the temperature variable
  int gyro_x = Wire.read()<<8|Wire.read();                                 //Add the low and high byte to the gyro_x variable
  int gyro_y = Wire.read()<<8|Wire.read();                                 //Add the low and high byte to the gyro_y variable
  int gyro_z = Wire.read()<<8|Wire.read();                                 //Add the low and high byte to the gyro_z variable
}
