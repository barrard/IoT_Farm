/*
This is the code for the rotary encoder to be attached to the axle of the plant/water balance
I believe its an optical encoder. 
Red is 5vdc
Black is ground 
Green is pin D0, arduino ide correlate= 16, (A)
White D1, arduino ide correlate= 5, (B)
*/


volatile unsigned int temp, counter = 0; //This variable will increase or decrease depending on the rotation of encoder
    
void setup() {
  Serial.begin (9600);

  pinMode(16, INPUT_PULLUP); // internal pullup input pin 16 
  
  pinMode(5, INPUT_PULLUP); // internal pullup input pin 5
//Setting up interrupt
  //A rising pulse from encodenren activated ai0().              AttachInterrupt 0 is DigitalPin nr 2 on moust Arduino.
  attachInterrupt(16, ai0, RISING);
   
  //B rising pulse from encodenren activated ai1().               AttachInterrupt 1 is DigitalPin nr 3 on moust Arduino.
  attachInterrupt(5, ai1, RISING);
  }
   
  void loop() {
  // Send the value of counter
  if( counter != temp ){
  Serial.println (counter);
  temp = counter;
  }
  }
   
  void ai0() {
  // ai0 is activated if DigitalPin nr 5 is going from LOW to HIGH
  // Check pin 5 to determine the direction
  if(digitalRead(5)==LOW) {
  counter++;
  }else{
  counter--;
  }
  }
   
  void ai1() {
  // ai0 is activated if DigitalPin nr 16 is going from LOW to HIGH
  // Check with pin 16 to determine the direction
  if(digitalRead(16)==LOW) {
  counter--;
  }else{
  counter++;
  }
}
