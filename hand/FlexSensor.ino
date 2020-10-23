const int flexPin = A0; //The AnalogPin that reads the differences in all in the ohm values 
const int selectedPin[3] = {D1, D2, D3}; //The pins for the InputState 
int flexValue[5];    //Container for all the flexValues

void setup() {
   Serial.begin(9600);
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
         digitialWrite(pin, pin & (1 << i)?HIGH:LOW);    //Write the Values 1 or 0 --> Verunden von bzw 1(001) & S1(001) = 1 || 1(001) & S2(010) = 0
      }
      flexValue[pin] = analogRead(flexPin)   //read the Value from the selected pin
      Serial.print(flexValue[pin] + "\t")    //print the value
   }
   Serial.println();
   delay(1000); //1 secound brake for the esp
}
