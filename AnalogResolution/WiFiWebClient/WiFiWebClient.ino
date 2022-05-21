
/*
  Web client

 This sketch connects to a website (http://www.google.com)
 using a WiFi shield.

 This example is written for a network using WPA encryption. For
 WEP or WPA, change the Wifi.begin() call accordingly.

 This example is written for a network using WPA encryption. For
 WEP or WPA, change the Wifi.begin() call accordingly.

 Circuit:
 * WiFi shield attached

 created 13 July 2010
 by dlf (Metodo2 srl)
 modified 31 May 2012
 by Tom Igoe
 */


#ifndef __CC3200R1M1RGC__
// Do not include SPI for CC3200 LaunchPad
#include <SPI.h>
#endif
#include <WiFi.h>
#include <WiFiClient.h>
#include "Ubidots.h"

char ubidotsToken[] = "***********";
char ssid[] = "***********";       // your network name also called SSID
char password[] = "***********";  // your network password

Ubidots ubidots(ubidotsToken, UBI_TCP);

void setup(){
  Serial.begin(115200);
  while(!ubidots.wifiConnect(ssid, password)){
    Serial.println("Waiting to connect!!");
    ubidots.setDebug(true);
  }

  Serial.println("Connected!");
  
}

void loop(){

  float val1 = random(0,9)*10;
  //float val2 = random(0.9)*100;

  ubidots.add("ECG_Sensor_Data", val1);

  bool bufferSent = false;
  bufferSent = ubidots.send();

  if(bufferSent){
    Serial.print("Values successfully sent!");
  }

  delay(5000);
  
}

// --------------- THIS WORKS --------------------

//int status = WL_IDLE_STATUS;     // the Wifi radio's status
//
//IPAddress ip;                    // the IP address of your shield
//
//// if you don't want to use DNS (and reduce your sketch size)
//// use the numeric IP instead of the name for the server:
//// IPAddress server(50,62,217,1);  // numeric IP for destination server
//char server[] = "industrial.api.ubidots.com";    // name address for Google (using DNS)
//
//// Initialize the Ethernet client library
//// with the IP address and port of the server
//// that you want to connect to (port 80 is default for HTTP):
//WiFiClient client;
//
//void setup() {
//  
////Initialize serial and wait for port to open:
//  Serial.begin(115200);
//  Serial.print("Port open...\n");
//  delay(1000);
//
//  Serial.println("Attempting to connect to Network named: ");   // attempt to connect to Wifi network:
//  Serial.print(ssid);  // print the network name (SSID);
//  delay(1000);
//  
//  WiFi.begin(ssid, password);    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
//  Serial.println("WifiBegin() called.. ");
//  delay(1000);
//  
//  Serial.print("Current Status = ");
//  Serial.print(WiFi.status());
//
//  //while (!WiFi.localIP().isSet())
//  //while ( WiFi.status() != 3){
//  while (WiFi.status() != WL_CONNECTED){
//    Serial.print("\nWiFi Status: ");
//    Serial.print(WiFi.status());
//    delay(300);
//   }
//
//   Serial.print("\nWiFi Connected!")
//
//   Serial.println("\nStarting connection to server...");
//   // if you get a connection, report back via serial:
//   if (client.connect(server, 9812)) {
//    Serial.println("Connected to server");
//    Serial.println("Making HTTP request to Ubidots cloud");
//    // Make a HTTP request:
////    client.println("GET https://industrial.api.ubidots.com/api/v2.0/variables/?fields=label,type");
////    client.println("Host: Ubidots Cloud");
////    client.println("Connection: close");
////    client.println();
//  }
//}
// ------------ TO HERE ------------------


//void loop() {
//  // if there are incoming bytes available
//  // from the server, read them and print them:
//  while (client.available()) {
//    char c = client.read();
//    Serial.write(c);
//  }
//
//  // if the server's disconnected, stop the client:
//  if (!client.connected()) {
//    Serial.println();
//    Serial.println("disconnecting from server.");
//    client.stop();
//
//    // do nothing forevermore:
//    while (true);
//  }
//}
//
//
//void printWifiStatus() {
//  // print the SSID of the network you're attached to:
//  Serial.print("SSID: ");
//  Serial.println(WiFi.SSID());
//
//  // print your WiFi shield's IP address:
//  IPAddress ip = WiFi.localIP();
//  Serial.print("IP Address: ");
//  Serial.println(ip);
//
//  // print the received signal strength:
//  long rssi = WiFi.RSSI();
//  Serial.print("signal strength (RSSI):");
//  Serial.print(rssi);
//  Serial.println(" dBm");
//}
