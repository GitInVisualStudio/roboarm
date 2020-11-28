const int flexPin = A0; //The AnalogPin that reads the different values 
const int selectedPin[3] = {D6, D5, D0}; //The pins for the InputState (maybe not the right pins)
int flexValue[5];    //Container for all the flexValues

void setup() {
   Serial.begin(115200);
   pinMode(flexPin, INPUT);  //Setup the pin as input
   for(int i = 0;i<3;i++) {
      pinMode(selectedPin[i], OUTPUT);
      digitialWrite(selectedPin[i], HIGH)
   }
   
   Serial.println("Starting"); 
   Serial.println("F1\F2\F3\F4\F5");   //Create a table for the values
   Serial.println(---\t---\t---\t---\t---\t);
}

void loop() {

   for(int pin = 0;pin < 5;pin++) { //all the analog sensors
      for(int i = 0; i < 3;i++) {   //the digital pins to switch between the different sensor via the mulitplexer pcb 74HC4051
         digitialWrite(selectedPin[i], (pin >> i & 1) ? HIGH : LOW);    //Write the Values 1 or 0 --> Verunden von bzw 1(001) & S1(001) = 1 || 1(001) & S2(010) = 0
      }
      flexValue[pin] = analogRead(flexPin)   //read the Value from the selected pin
      //flexSensor = map(flexSensor,0,1023,0,100); //calculate the procentage
      Serial.print(flexValue[pin] + "\t")    //print the value
   }
   Serial.println();
   delay(100); //1 secound brake for the esp
}
