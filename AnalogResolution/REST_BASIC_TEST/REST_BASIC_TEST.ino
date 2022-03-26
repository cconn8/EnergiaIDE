

#include <WiFi.h>
WiFiClient client;


/* ==== CONSTANTS ====== */ 

char ssid[] = "AndroidAP2744";
char password[] = "Hello1234";
char ubidots_server[] = "industrial.api.ubidots.com";

const int analogInput = 5;  //P4.1 (A12) analogRead()
const int analogOutput = 19; //P2.5  analogWrite()
const int lead0 = 39;  //P3.5   GPIO I/O
const int lead1 = 38;  //P3.7   GPIO I/O

/* ==== VARIABLES ====== */ 
float sensorValue = 0;
float mappedOutput = 0;
String mappedOutput_as_String;

/* =========================================
 *  Supporting Setup Functions
 * ========================================= */

void connectToWiFi(){
  Serial.println("1.Attempting to connect to Network named: ");
  Serial.print(ssid);
  
  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(150); 
  }
  Serial.println(" --> Connected! Waiting for IP address!");
  while(WiFi.localIP() == INADDR_NONE){
    Serial.print(".");
    delay(150);
  }
  Serial.println(WiFi.localIP()); 
/* --- CONNECT TO WIFI END ()------*/
} 


void connect(){
   while(!client.connected()){
      Serial.println("\n2. Attempting connection to Ubidots server");
      client.connect(ubidots_server,80);
      delay(250);
   }  
  Serial.println(" -----> Connected to Ubidots");
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
    Serial.println("Not connected to Server, stopping client!");
    client.stop();
  }
}

String generateData(){
  sensorValue = analogRead(analogInput);  //Read in data from ECG 
  Serial.println(sensorValue);

  mappedOutput = map(sensorValue, 0, 1023, 0, 255); //scale/map raw data to 14 bit resolution analog out put (ie. 0 off, 255 fully on scale)
  //analogWrite(analogOutput, mappedOutput);   
  mappedOutput_as_String = String(mappedOutput, 2);  //convert float outputValue to string with 2 decimal places
  
  String data = "{\"ECG\": ";
  data += mappedOutput_as_String;  //take the mapped ecg data and turn it into string data here to append
  data += "}";
  return data;
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
  delay(100);
  connect();
}

/* =========================================
 *  MAIN LOOP() FUNCTION
 * ========================================= */

void loop() {

delay(100);
if(client.connected()){
 
  postData(generateData());
  delay(100);
  readResponse();
  delay(100);
  }

  else {
    Serial.print("Loop not running, client not connected!");
  }
}
