#include <Wire.h>

long loop_timer = 0;
int16_t acc_x, acc_y, acc_z;
int gyro_x, gyro_y, gyro_z;
int cal_x, cal_y, cal_z;
long gyro_x_cal, gyro_y_cal, gyro_z_cal;
float angle_pitch, angle_roll;
float total_length;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
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
  delay(1000);
  int len = 2000;
  for (int cal_int = 0; cal_int < len ; cal_int ++){                  
    read_mpu();                                              
    gyro_x_cal += gyro_x;                                    
    gyro_y_cal += gyro_y;                                    
    gyro_z_cal += gyro_z;  
    cal_x += acc_x;                                  
    cal_y += acc_y;                                  
    cal_z += acc_z;                                  
    delay(3);                                                
  }
  gyro_x_cal /= len;                                        
  gyro_y_cal /= len;                                        
  gyro_z_cal /= len;                                        

  cal_x /= len;
  cal_y /= len;
  cal_z /= len;
  
  total_length = sqrt((cal_x*cal_x)+(cal_y*cal_y)+(cal_z*cal_z));
}

int16_t read_data(){
  return Wire.read()<<8|Wire.read();
}

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

void loop() {
  read_mpu();

  gyro_x -= gyro_x_cal;                                                //Subtract the offset calibration value from the raw gyro_x value
  gyro_y -= gyro_y_cal;                                                //Subtract the offset calibration value from the raw gyro_y value
  gyro_z -= gyro_z_cal;                                                //Subtract the offset calibration value from the raw gyro_z value

  angle_pitch += gyro_x * 0.0000611;                                   //Calculate the traveled pitch angle and add this to the angle_pitch variable
  angle_roll += gyro_y * 0.0000611;                                    //Calculate the traveled roll angle and add this to the angle_roll variable
  Serial.printf("pitch: %f \t\t roll: %f\n", angle_pitch, angle_roll);
  while(micros() - loop_timer < 4000);                    
  loop_timer = micros();     
}
