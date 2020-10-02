#include <Wire.h>
int pin = 20; //d1 = 20, d2 = 19
long loop_timer = 0;
float x, y, z;
int16_t acc_x, acc_y, acc_z, gyro_x, gyro_y, gyro_z;
float angle_pitch, angle_roll;

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

  float acc_total_vector = sqrt((acc_x*acc_x)+(acc_y*acc_y)+(acc_z*acc_z));  //Calculate the total accelerometer vector
  //57.296 = 1 / (3.142 / 180) The Arduino asin function is in radians
  float angle_pitch_acc = asin((float)acc_y/acc_total_vector)* 57.296;       //Calculate the pitch angle
  float angle_roll_acc = asin((float)acc_x/acc_total_vector)* -57.296;       //Calculate the roll angle

  /*x = x * beta + (acc_x / 255.0f * (1 - beta));
  y = y * beta + (acc_y / 255.0f * (1 - beta));
  z = z * beta + (acc_z / 255.0f * (1 - beta));*/

  Serial.print(angle_pitch_acc);
  Serial.print("\t\t");
  Serial.print(angle_roll_acc);
  Serial.print("\t\t");
  Serial.print(acc_x);
  Serial.println();
  
  while(micros() - loop_timer < 4000);                    
  loop_timer = micros();     
}
