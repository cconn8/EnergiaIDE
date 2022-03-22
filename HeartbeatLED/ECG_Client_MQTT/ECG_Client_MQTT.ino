/*******************************************************************************
 * Copyright (c) 2014 IBM Corp.
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * and Eclipse Distribution License v1.0 which accompany this distribution. 
 *
 * The Eclipse Public License is available at 
 *   http://www.eclipse.org/legal/epl-v10.html
 * and the Eclipse Distribution License is available at 
 *   http://www.eclipse.org/org/documents/edl-v10.php.
 *
 * Contributors:
 *    Ian Craggs - initial contribution
 *******************************************************************************/
#include <SPI.h>
#include <WiFi.h>
#include <WifiIPStack.h>
#include <Countdown.h>
#include <MQTTClient.h>

/****************************************************
 *  Declare Constants
 ***************************************************/

#define MQTTCLIENT_QOS 0
#define TOKEN "BBFF-u0EJzOxnOiQxUJyAIkNKsljgYPLj5s"
#define MQTTCLIENT_NAME "MSP342-CC3100_GATEWAY"  //this is the name of "me (device/client)" connecting to the broker (can be anything)

#define VARIABLE_LABEL "ECG_Sensor_Data"
#define DEVICE_LABEL "ecg_monitoring_device"
#define TOPIC "ECG_Data"


char mqttBroker[] = "industrial.api.ubidots.com";
char payload[1000];
float vals[] = {42.4, 53.1, 12.7, 34.1};
char str_vals[(sizeof(vals)/sizeof(vals[0]))];
char dataForSend[50];  //space to store the data to send
char string_val[100];

char* deviceLabel = DEVICE_LABEL;
char* topic = TOPIC;

 
//************** DECALRE VARIABLES *******************

char ssid[] = "OnePlus3";    // network name also called SSID
char password[] = "Hello1234";   // your network password
char printbuf[100];  //receiving buffer / feedback buffer for debugging
int arrivedcount = 0;

//************** AUXILLARY FUNCTIONS ******************

void messageArrived(MQTT::MessageData& md){
  MQTT::Message &message = md.message;
  
  sprintf(printbuf, "Message %d arrived: qos %d, retained %d, dup %d, packetid %d\n", ++arrivedcount, message.qos, message.retained, message.dup, message.id);
  Serial.print(printbuf);
  sprintf(printbuf, "Payload %s\n", (char*)message.payload);
  Serial.print(printbuf);
}

WifiIPStack ipstack;
MQTT::Client<WifiIPStack, Countdown> client = MQTT::Client<WifiIPStack, Countdown>(ipstack);

// ******** MQTT CONNECT FUNCTION *****************
void connect(){

  int rc;
  int port = 8883;
  
  sprintf(printbuf, "Connecting to %s:%d\n", mqttBroker, port);
  Serial.print(printbuf);
  
  rc = ipstack.connect(mqttBroker, port);  //rc denotes the return value. shows success/error val/
  
  if (rc != 1){
    sprintf(printbuf, "rc from TCP connect is %d\n", rc);
    Serial.print(printbuf);
  }
 
  Serial.println("MQTT connecting");
  MQTTPacket_connectData data = MQTTPacket_connectData_initializer;       
  data.MQTTVersion = 3;
  data.clientID.cstring = (char*)"ECG_Sensor_Data";

  Serial.println("Break after connecting - all initialized"); // debugging break point
  
  rc = client.connect(data);
  
  sprintf(printbuf, "rc from MQTT connect is %d\n", rc);

  Serial.println(printbuf);   //debugging break point to check RC
  
  while (rc != 0){ 
    Serial.print(".");
    delay(250);
  } 
  
  Serial.println("MQTT connected");

  Serial.println("Subscribing to MQTT Broker with QOS0"); 
  rc = client.subscribe(topic, MQTT::QOS0, messageArrived);

  sprintf(printbuf, "rc from MQTT subscribe is %d\n", rc);
  while (rc != 0){
    Serial.print(".");
    delay(250);
  } 
  
  Serial.println("MQTT subscribed");
}

void setup(){

  Serial.begin(115200); // attempt to connect to Wifi network:
  Serial.print("Attempting to connect to Network named: ");
  Serial.println(ssid); // print the network name (SSID);

  WiFi.begin(ssid, password); // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
  while ( WiFi.status() != WL_CONNECTED) {
    Serial.print(".");  // print dots while we wait to connect
    delay(300);
  }
  
  Serial.println("\nYou're connected to the network");
  Serial.println("Waiting for an ip address");
  
  while (WiFi.localIP() == INADDR_NONE) {
    Serial.println(".");  // print dots while we wait for an ip addresss
    delay(300);
  }

  // We are connected and have an IP address.
  Serial.println("\nIP Address obtained");
  Serial.println(WiFi.localIP());  
  
  Serial.println("MQTT example - now trying to connect() to broker!");
  connect();
  
}

void loop()
{
  if (!client.isConnected())
    connect();
  
  MQTT::Message message;
  
  arrivedcount = 0;
  
  //for(int i=0; i<(sizeof(vals)/sizeof(vals[i])); i++){

   // ******** turn floats to strings  *********
  //dtostrf(vals[i], 4, 2, &str_vals[i]);
  //dtostrf(vals[0], 4, 2, &str_vals[0]);
 
  // Send and receive QoS 1 message
  //sprintf(buf, "Basic Send!  QoS 0 message");
  sprintf(topic, "%s%s", "/v1.6/devices/", deviceLabel);
  sprintf(payload, "%s", "");
  sprintf(payload, "%s {\"value\": %s}}", payload, "42.5");

  //}
  
  Serial.println("Publishing to Ubidots cloud device");
  message.qos = MQTT::QOS0;
  message.payload = payload;
  message.payloadlen = strlen(payload)+1;
  
  int rc = client.publish(topic, message);

  while (arrivedcount == 1)
    client.yield(1000);
  
  delay(2000);
}
