const int flexPin = A1; //The AnalogPin that reads the differences in the ohm values 

void setup() {

Serial.begin(9600);

pinMode(flexPin, INPUT);  //Setup the pin as input
Serial.println("Starting"); 
}

void loop() {

int flexStatus = 0; //always reset the value to 0 after one round
Serial.print("Value: ");
for(int i = 0; i < 10;i++) {  //A for circle to get good value because if you only take one value the likely to jump a little bit. This flattes the data a little bit
   flexStatus = flexStatus + analogRead(flexPin); //read the analog values from the seonsor the values are between 1023 and 1015 but I don't know how much this values differentiate between each sensor
}
flexStatus = flexStatus / 10; //divide the the 10 values to get the avagroge 
Serial.println(flexStatus);
delay(1000);   //1 secound brake for the esp
}
