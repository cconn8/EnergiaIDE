#include <arm_const_structs.h>
#include <arm_common_tables.h>
#include <arm_math.h>

#include <WiFi.h>
WiFiClient client;


/* ==== CONSTANTS ====== */ 

char ssid[] = "AndroidAP2744";
char password[] = "Hello1234";
char ubidots_server[] = "industrial.api.ubidots.com";

const unsigned int analogInput = 5;  //P4.1 (A12) analogRead()
const unsigned int analogOutput = 19; //P2.5  analogWrite()
const unsigned int lead0 = 32;  //P3.5   GPIO I/O
const unsigned int lead1 = 31;  //P3.7   GPIO I/O
const unsigned int numSamples = 32; //ecg data samples per block

/* ==== VARIABLES ====== */ 
float sensor_data[numSamples];
float mapped_data[numSamples];
String str_data;
String data;

/* =========================================
 *  SETUP() FUNCTION
 * ========================================= */

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);         //opens serial connection to the board
  analogReadResolution(14);    //set analog resolution to 14 bits (default)  
  pinMode(lead0, INPUT);      //set GPIO pins as Input
  pinMode(lead1, INPUT);     //set GPIO pins as Input
  connectToWiFi();          //sets up connection to local wifi
  delay(100);              //add delay for wifi to set up
  connect();              //try to connect to Ubidots server (this works when its in setup function!)
  Serial.print("\nSetup complete!\n");
}

/* =========================================
 *  MAIN LOOP() FUNCTION
 * ========================================= */

void loop() {

  for(int i=0; i< numSamples; i++){
    dataArray[i] = ecgRead();
    generateData();
  }
  Serial.println("POST DATA CALL COMPLETE");
  readResponse();
  delay(1000);

}

// 1.---------------------------------------

/* =========================================
 *  Supporting Setup Functions
 *  */

void connectToWiFi(){
  Serial.println("Attempting to connect to WiFiNetwork: ");
  Serial.print(ssid);
  
  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(100); 
  }
  Serial.println("\n --> Connected! Waiting for IP address!");
  while(WiFi.localIP() == INADDR_NONE){
    Serial.print(".");
    delay(100);
  }
  Serial.print(WiFi.localIP()); 
} /* --- CONNECT TO WIFI END ()------*/ 


void connect(){

  if (!client.connected()){
    Serial.println("\nStarting connection to server...");
    // if you get a connection, report back via serial:
    if (client.connect(ubidots_server, 80)) {   
      Serial.println(" --> Connected to server");   
    }   
  }
}  /* --- END OF FUNCTION ()------*/

/* 
 *  END Setup Functions
 * =========================================
 * ========================================= */
 

 /* =========================================
 *  MAIN Loop Functions
 *  */

void generateData(){  //PROBLEM IS OCCURING HERE, READING IN ECG values and converting. Need to pull into seperate funciton.

  randomSeed(millis());
  for(int i=0; i<numSamples; i++){                           //define the number of samples per data block 
    //sensor_data[i] = analogRead(analogInput);              //Read in data from ECG 
    sensor_data[i] = random(300);                            //DEBUGGING ONLY
    mapped_data[i] = map(sensorValue, 0, 1023, 0, 255);    //map data to range
    Serial.println("Sensor: "); Serial.print(sensor_data[i]); Serial.print("  Mapped: "); Serial.print(mapped_data[i]);
    delay(100);
  }

  Serial.println("\n -- Data Block Complete! --\n");
}

//void filterData();

String generatePayload() {

  for(int i=0; i<numSamples; i++){
    str_data = String(mapped_data[i], 2);
    data = "{\"DATA_TEST\": "
    data += str_data;
    data += "},";
  }

  Serial.println("\nPayload generated! "); Serial.print("Length: "); Serial.print(data.length());
  return data;
}

void postData(String data){

  if(!client.connected()){
    connect();
  }

  // Make a HTTP request:
  client.println("POST /api/v1.6/devices/test HTTP/1.1");
  client.println("Host: industrial.api.ubidots.com");
  client.println("Content-Type: text/json");
  client.println("X-Auth-Token: BBFF-tCUapuWpXibtncdsyFbUlz5ZaZrrnt");
  client.println("Content-Length: ");
  client.println(data.length());
  client.println();
  client.print(data);
  Serial.println("posted to server");
  Serial.println(data);
  
}


void readResponse(){
   // if there are incoming bytes available from ubidots (get requests)
  // read them and print them:
  
  while (client.available()) {  
    char c = client.read();
    Serial.write(c);
  } 

  // if the server's disconnected, stop the client:
  if (!client.connected()) {
    Serial.print("Not connected to Server, stopping client!");
    client.stop();
  }
}
    
    
    
    
    
    
