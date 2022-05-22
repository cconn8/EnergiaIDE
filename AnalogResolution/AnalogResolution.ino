

#include <WiFi.h>

// Initialize the Ethernet client library
// with the IP address and port of the server
// that you want to connect to (port 80 is default for HTTP):
WiFiClient client;


/* ==== CONSTANTS ====== */ 

char ssid[] = "**********";
char password[] = "********";
char ubidots_server[] = "industrial.api.ubidots.com";

const int analogInput = 5;  //P4.1 (A12) analogRead()
const int analogOutput = 19; //P2.5  analogWrite()
const int lead0 = 39;  //P3.5   GPIO I/O
const int lead1 = 38;  //P3.7   GPIO I/O

/* ==== VARIABLES ====== */

float sensorValue = 0;  //Store incoming data from AD8232
float outputValue = 0;  //Store mapped output to send
String str_output;

/* =========================================
 *  Supporting Setup Functions
 * ========================================= */

void connectToWiFi(){
  Serial.print("Attempting to connect to Network named: ");
  Serial.print(ssid);
  
  //Serial.print("Calling WiFi begin() fucntion!");
  WiFi.begin(ssid, password);
  //Serial.print("Entering while loop");
  while(WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(150); 
  }

  Serial.println("Connected! Waiting for IP address!");

  while(WiFi.localIP() == INADDR_NONE){
    Serial.print(".");
    delay(300);
  }

  //print wifi details
  Serial.println("IP Address: ");
  Serial.print(WiFi.localIP()); 
  
  // print the received signal strength:
//  long rssi = WiFi.RSSI();
//  Serial.println("signal strength (RSSI):");
//  Serial.print(rssi);
//  Serial.println(" dBm");

  /* --- END OF FUNCTION ()------*/
  } 

void connect(){
  if (!client.connected()){
    Serial.println("\nStarting connection to server...");
  }

  if(client.connect(ubidots_server, 80)){
    Serial.println("Connected!");;
  }
  /* --- END OF FUNCTION ()------*/
}

void postData(String data){
  
  connect();
  
  // Make a HTTP request:
  client.println("POST /api/v1.6/devices/test HTTP/1.1");
  client.println("Host: industrial.api.ubidots.com");
  client.println("Content-Type: text/json");
  client.println("X-Auth-Token: BBFF-tCUapuWpXibtncdsyFbUlz5ZaZrrnt");
  client.print("Content-Length: ");
  client.println(data.length());
  client.println();
  client.print(data);
  Serial.println("posted to server");
  Serial.println(data);
  
  /* --- END OF FUNCTION ()------*/
  
}

void readResponse(){
    // if there are incoming bytes available
  // from the server, read them and print them:
  while (client.available()) {
    char c = client.read();
    Serial.write(c);
  }

  // if the server's disconnected, stop the client:
  if (!client.connected()) {
    Serial.println();
    Serial.println("disconnecting from server.");
    client.stop();
  }
  /* --- END OF FUNCTION ()------*/
  
}

/* =========================================
 *  SETUP() FUNCTION
 * ========================================= */

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);         //opens serial connection to the board
  analogReadResolution(14);   //set analog resolution to 14 bits (default)  
  pinMode(lead0, INPUT);      //set GPIO pins as Input
  pinMode(lead1, INPUT);

  connectToWiFi();
  
}

/* =========================================
 *  MAIN LOOP() FUNCTION
 * ========================================= */

void loop() {

  if(lead0 == digitalRead(1) || lead1 == digitalRead(1)){
    Serial.println("Reading not available - please place pads!");
  }
  
  else{

  //Serial.println("ADC 14-bit");  
  sensorValue = analogRead(analogInput);
  Serial.println(sensorValue);

  //Map the analog data to meaningful out for Ubidots
  outputValue = map(sensorValue, 0, 1023, 0, 255);
  analogWrite(analogOutput, outputValue);

  //convert float output to string for transmission  dtostrf(outputValue, 6, 3, str_output);

  //print results to serial monitor
  Serial.print("Sensor = ");
  Serial.print(sensorValue);
  Serial.print("\t Output = ");
  Serial.println(outputValue);

  postData(str_output);
  readResponse();

  //add short delay for the analog-to-digital converter to settle
  delay(10000);

  }
  
}
