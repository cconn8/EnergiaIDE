

#include <WiFi.h>
WiFiClient client;


/* ==== CONSTANTS ====== */ 

char ssid[] = "AndroidAP2744";
char password[] = "Hello1234";
char ubidots_server[] = "industrial.api.ubidots.com";

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
  randomSeed(millis());
  String data = "{\"direction\": ";
  data += random(300);
  data += "}";
  return data;
}

/* =========================================
 *  SETUP() FUNCTION
 * ========================================= */

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);         //opens serial connection to the board
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
  readResponse();
  //add short delay for the analog-to-digital converter to settle
  delay(100);
  }

  else {
    Serial.print("Loop not running, client not connected!");
  }
}
