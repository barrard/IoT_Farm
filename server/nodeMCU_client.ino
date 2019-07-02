/*
This is the code for the rotary encoder to be attached to the axle of the plant/water balance
I believe its an optical encoder. 
Red is 5vdc
Black is ground 
Green is pin D0, arduino ide correlate= 16, (A)
White D1, arduino ide correlate= 5, (B)
/*
2 = MCU pin 4
4 = MCU pin 2 
5 = MCU pin 1
14 = MCU pin 5
16 = MCU pin 0
13 = 7
15 = 8
 */

#include <ESP8266WiFi.h>
#include <Arduino.h>
#include <ESP8266HTTPClient.h>

#ifndef STASSID
#define STASSID "KupaaNew"
#define STAPSK  "GrownOnMaui"
#endif

const char* ssid     = STASSID;
const char* password = STAPSK;

String host = "google.com/";
int port = 80;
 
  volatile unsigned int  temp, counter = 50; //This variable will increase or decrease depending on the rotation of encoder
  int startThreshold = 80;
  int stopThreshold = 40;
  int action  = 0;
/*const int startPin = 4; // this is the button that indicates the starting position
const int stopPin = 0;  //this is the button that indicates the stop position*/

void setup() {
  Serial.begin (9600);

//  wifi code

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
      Serial.println("");
  Serial.println("WiFi connected"); 
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  
//wifi code
  pinMode(16, INPUT_PULLUP); // internal pullup input pin 16 nodeMCU pin :
  
  pinMode(5, INPUT_PULLUP); // internal pullup input pin 5

  pinMode(2, INPUT);

  pinMode(4, INPUT);
//Setting up interrupt
  //A rising pulse from encodenren activated ai0().              AttachInterrupt 0 is DigitalPin nr 2 on moust Arduino.
  attachInterrupt(16, ai0, RISING); 
  //B rising pulse from encodenren activated ai1().               AttachInterrupt 1 is DigitalPin nr 3 on moust Arduino.
  attachInterrupt(5, ai1, RISING);
//  attachInterrupt(2, ai2, RISING);
//  attachInterrupt(4, ai4, RISING);
  
  }

  void loop() 
  {

//wifi code
  WiFiClient client;
  
Serial.print("connecting to ");
  Serial.print(host);
  Serial.print(':');
  Serial.println(port);
  client.begin("http://192.168.0.4:80/");
  http.GET();

  String payload = client.getString();
  Serial.println(payload);
  
  

  // Use WiFiClient class to create TCP connections
//  if (!client.connect(host, port)) {
//    Serial.println("connection failed");
//    delay(5000);
//    return;
//  }

  // This will send a string to the server
  Serial.println("sending data to server");
  if (client.connected()) {
    client.println("hello from ESP8266");
  }

    // This will send the request to the server
//  client.print(String("GET ") + "/" + " HTTP/1.1\r\n" +
//               "Host: " + 
//               "/"+ "\r\n" +
//               "Connection: close\r\n\r\n");
//  unsigned long timeout = millis();
//  while (client.available() == 0) {
//    if (millis() - timeout > 5000) {
//      Serial.println(">>> Client Timeout !");
//      client.stop();
//      return;
//    }
//  }



  // Read all the lines of the reply from server and print them to Serial
  Serial.println("receiving from remote server");
  // not testing 'client.connected()' since we do not need to send data here
  while (client.available()) {
    char ch = static_cast<char>(client.read());
    Serial.print(ch);
  }

  // Close the connection
  Serial.println();
  Serial.println("closing connection");
  client.stop();



//wifi code



    
// send value of counter
  if( counter != temp )
  {
  Serial.println (counter);
  temp = counter;
  }
  //if this button is pressed, we will assign the counter start threshold to 'counter'
   if(digitalRead(2)==1)
   {
    startThreshold = counter;
    Serial.print("start threshold assigned:"); 
    Serial.println(startThreshold);
   }
  
//    if this button is pressed, we will assign the counter start threshold to 'counter'
  if(digitalRead(4)==1)
   {
    stopThreshold = counter;
    Serial.print("stop threshold assigned:");  
    Serial.println(stopThreshold);
    delay(1000);
   }
   delay(1000);
   Serial.print("counter:");
   Serial.println(counter);
   Serial.print("startThreshold:");
   Serial.println(startThreshold);
   Serial.print("stopThreshold:");
   Serial.println(stopThreshold)   ;
   Serial.print("action:");
   Serial.println(action);
   delay(1000);

  if(counter > startThreshold )
    {
    action = 1;
    Serial.println("relay on");    
    delay(1000);
    }
    
    
  if(counter < stopThreshold)
      {
        action = 0;
        Serial.println("relay off") ;
       }
    }
     
  void ai0() {
  // ai0 is activated if DigitalPin nr 5 is going from LOW to HIGH
  // Check pin 5 to determine the direction
  if(digitalRead(5)==HIGH) {
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
/*
  void ai2() {
//start Threshold button
  if(digitalRead(2)==1) {
    startThreshold = counter;
    Serial.println("start threshold assigned at"); 
    Serial.print(startThreshold);
  }
  }

    void ai4() {
//start Threshold button
  if(digitalRead(4)==1) {
    stopThreshold = counter;
    Serial.println("stop threshold assigned at"); 
    Serial.print(stopThreshold);
  }
  }
*/