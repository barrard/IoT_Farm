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

#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <ESP8266HTTPClient.h>

#include <WiFiClientSecureBearSSL.h>

ESP8266WiFiMulti WiFiMulti;


#ifndef STASSID
#define STASSID "KupaaNew"
#define STAPSK  "GrownOnMaui"
#endif

const char* ssid     = STASSID;
const char* password = STAPSK;

  String iot_url = "https://iot.dakine.website";
 
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
  WiFiMulti.addAP(ssid, password);


  
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

  // while (WiFiMulti.status() != WL_CONNECTED) {
  //   delay(500);
  //   Serial.print(".");
  //   Serial.println("");
 
  // }

  // Serial.println("");
  // Serial.println("WiFi connected");
  // Serial.println("IP address: ");
  // Serial.println(WiFiMulti.localIP());


  
  }

  void loop() 
  {
    /* WIFI CODE */
    // bool connected = false;
    // if ((WiFiMulti.run() == WL_CONNECTED)) {connected = true}
  /* WIFI CODE */
    if ((WiFiMulti.run() != WL_CONNECTED)) {
      Serial.println('Not yet connected');
      delay(2000);
      return;
    }

  


  // This will send a string to the server
  // Serial.println("sending data to server");
  // if (client.connected()) {
  //   client.println("hello from ESP8266");
  // }
  //wifi code
  std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
  client->setInsecure();

    HTTPClient http;
  

  
//wifi code



    
// send value of counter
  if( counter != temp ){
    Serial.println (counter);
    temp = counter;
    Serial.print("[HTTP] begin...sending counter \n");
    if (http.begin(*client, iot_url + "/rotary/"+counter+"/0")) {  // HTTP
      http.addHeader("secret", "8266iot");

      Serial.print("[HTTP] GET...\n");
      // start connection and send HTTP header
      int httpCode = http.GET();
      Serial.print(httpCode);
      http.end();
    } else {
      Serial.printf("[HTTP} Unable to connect - relay off\n");
    }
  
  }
  //if this button is pressed, we will assign the counter start threshold to 'counter'
   if(digitalRead(2)==1)
   {
    startThreshold = counter;
    Serial.print("start threshold assigned:"); 
    Serial.println(startThreshold);
    Serial.print("[HTTP] begin.. setting start threshold.\n");
    if (http.begin(*client, iot_url + "/rotary/set_start_treshold/"+startThreshold+"/0")) {  // HTTP
      http.addHeader("secret", "8266iot");

      Serial.print("[HTTP] GET...\n");
      // start connection and send HTTP header
      int httpCode = http.GET();
      Serial.print(httpCode);
      http.end();
    } else {
      Serial.printf("[HTTP} Unable to connect - relay off\n");
    }
   }
  
//    if this button is pressed, we will assign the counter start threshold to 'counter'
  if(digitalRead(4)==1)
   {
    stopThreshold = counter;
    Serial.print("stop threshold assigned:");  
    Serial.println(stopThreshold);
    Serial.print("[HTTP] begin.. setting stopThreshold.\n");
    if (http.begin(*client, iot_url + "/rotary/set_stop_treshold/"+stopThreshold+"/0")) {  // HTTP
      http.addHeader("secret", "8266iot");

      Serial.print("[HTTP] GET...\n");
      // start connection and send HTTP header
      int httpCode = http.GET();
      Serial.print(httpCode);
      http.end();
    } else {
      Serial.printf("[HTTP} Unable to connect - relay off\n");
    }
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

  if(counter > startThreshold ){
    action = 1;
    Serial.println("relay on");  

    Serial.print("[HTTP] begin.. counter > startThreshold.\n");
    if (http.begin(*client, iot_url + "/rotary/action/relay_on/0")) {  // HTTP
      http.addHeader("secret", "8266iot");

      Serial.print("[HTTP] GET...\n");
      // start connection and send HTTP header
      int httpCode = http.GET();
      Serial.print(httpCode);
      http.end();
    } else {
      Serial.printf("[HTTP} Unable to connect - relay off\n");
    }
  }
    
    
  if(counter < stopThreshold){
      action = 0;
      Serial.println("relay off") ;

      Serial.print("[HTTP] begin.. counter < stopThreshold.\n");
      if (http.begin(*client, iot_url + "/rotary/action/relay_off/0")) {  // HTTP
        http.addHeader("secret", "8266iot");

        Serial.print("[HTTP] GET...\n");
        // start connection and send HTTP header
        int httpCode = http.GET();
        Serial.print(httpCode);
        http.end();
      } else {
        Serial.printf("[HTTP} Unable to connect - relay off\n");
      }
    }
      delay(1000)
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


// void send_data(int data, WiFiClientSecure *client){
//   Serial.print("[HTTP] begin...\n");
//   if (http.begin(*client, iot_url + "/temp/"+temperature+"/"+humidity+"/0")) {  // HTTP
//       http.addHeader("secret", "8266iot");

//     Serial.print("[HTTP] GET...\n");
//     // start connection and send HTTP header
//     int httpCode = http.GET();
//           Serial.print(httpCode);


//     http.end();
//   } else {
//     Serial.printf("[HTTP} Unable to connect\n");
//   }
  

// }