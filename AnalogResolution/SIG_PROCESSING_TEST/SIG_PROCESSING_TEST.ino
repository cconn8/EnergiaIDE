#include <stdio.h>
//#include <aJSON.h>
#include <arm_const_structs.h>
#include <arm_common_tables.h>
#include <arm_math.h>
#include <WiFi.h>
WiFiClient client;

//#define PI 3.14159
#define ADC_RESOLUTION 16
#define ANALOG_FREQ 32768      //32.768kHz default frequency on A11 (P4.2)

#define SNR_THRESHOLD_F32   75.0f
#define BLOCK_SIZE          32
#define NUM_TAPS_ARRAY_SIZE 32
#define NUM_TAPS            29
#define TEST_LENGTH_SAMPLES 320

/* -------------------------------------------------------------------
 * Declare State buffer of size (numTaps + blockSize - 1)
 * ------------------------------------------------------------------- */
static float32_t firStateF32[2 * BLOCK_SIZE + NUM_TAPS - 1];
//static float32_t firStateF32[BLOCK_SIZE + NUM_TAPS - 1];

/* ----------------------------------------------------------------------
** FIR Coefficients buffer generated using fir1() MATLAB function.
** fir1(28, 6/24)
** ------------------------------------------------------------------- */
const float32_t firCoeffs32[NUM_TAPS_ARRAY_SIZE] = {
  -0.0018225230f, -0.0015879294f, +0.0000000000f, +0.0036977508f, +0.0080754303f, +0.0085302217f, -0.0000000000f, -0.0173976984f,
  -0.0341458607f, -0.0333591565f, +0.0000000000f, +0.0676308395f, +0.1522061835f, +0.2229246956f, +0.2504960933f, +0.2229246956f,
  +0.1522061835f, +0.0676308395f, +0.0000000000f, -0.0333591565f, -0.0341458607f, -0.0173976984f, -0.0000000000f, +0.0085302217f,
  +0.0080754303f, +0.0036977508f, +0.0000000000f, -0.0015879294f, -0.0018225230f, 0.0f,0.0f,0.0f
};

/* ==== CONSTANTS ====== */ 

char ssid[] = "***********";
char password[] = "***********";
char ubidots_server[] = "industrial.api.ubidots.com";

const unsigned int analogInput = 6;  //P4.3 (A10) analogRead() default freq 32.768kHz
const unsigned int analogOutput = 31; //P3.7  analogWrite()
const unsigned int lead0 = 5;  //P4.1   GPIO I/O
const unsigned int lead1 = 8;  //P4.6   GPIO I/O
const unsigned int numSamples = 320; //ecg data samples per block
const float arm_biquad_cascade_df1_f32();

float32_t *inputF32 , *outputF32;

/* ==== VARIABLES ====== */ 
uint32_t blockSize = BLOCK_SIZE;
uint32_t numBlocks = TEST_LENGTH_SAMPLES/BLOCK_SIZE;
//float32_t  snr;

float32_t sensor_data[TEST_LENGTH_SAMPLES];
float32_t mapped_data[TEST_LENGTH_SAMPLES];
float32_t filtered_data[TEST_LENGTH_SAMPLES];
String str_data;

/* =========================================
 *  SETUP() FUNCTION
 * ========================================= */

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);         //opens serial connection to the board
  analogReadResolution(16);    //set analog resolution to 14 bits (default)  
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

  if(lead0 == 1 || lead1 == 1){
    Serial.println("Leads not connected!");
    delay(100);
  } 
  
  else {
    
    generateData();
    delay(100);
    //filterData();

    arm_fir_instance_f32 S;
    arm_status status;  
  
    /* Initialize input and output buffer pointers */
    inputF32 = &sensor_data[0];
    outputF32 = &filtered_data[0];
    
    /* Call FIR init function to initialize the instance structure. */
    arm_fir_init_f32(&S, NUM_TAPS, (float32_t *)&firCoeffs32[0], &firStateF32[0], blockSize);

    for(int i=0; i < numBlocks; i++)
    {
      arm_fir_f32(&S, inputF32 + (i * blockSize), outputF32 + (i * blockSize), blockSize);
      Serial.println(sensor_data[i]); Serial.print(" "); Serial.print(filtered_data[i]); Serial.print(" "); 
      delay(100);
    }

    Serial.print("Filter complete!");
    
    //postData(generatePayload());
    //delay(100);
    //readResponse();
    //delay(1000); 
    
  }
}

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

  //randomSeed(millis());
  
  for(int i=0; i<numSamples; i++){                           //define the number of samples per data block 
    sensor_data[i] = analogRead(analogInput);              //Read in data from ECG 
    //sensor_data[i] = sin(1000 * (2 * PI) * i/44000);         //DEBUGGING ONLY
    mapped_data[i] = map(sensor_data[i], 0, 1023, 0, 255);   //map data to range
    //Serial.println("\nSensor: "); Serial.print(sensor_data[i]); Serial.print(" , Mapped: "); Serial.print(mapped_data[i]);
    Serial.println(sensor_data[i]); Serial.print(" "); 
    delay(100);
  } 
  
  Serial.println("\n -- Data Block Complete! Filtering ...  --\n");
  //filterData();
  
}

void filterData(){
/*
 * Removing baseline wander by implementing a high pass filter
 * Choosing an ideal high-pass filter as starting point
 * Baseline wander/drift can be seen by shifting (up/down) along the x axis
 * Caused by improper electrodes(electrode-skin impedence), patient movement and breathing(respiration)
 * Frequency content of baseline wander is in the range of 0.5Hz
 * We address this by implementing FIR High Pass zero phase forward/backward filter with cut off frequency of 0.5Hz
 */
//  arm_fir_instance_f32 S;
//  arm_status status;  
//
//  /* Initialize input and output buffer pointers */
//  inputF32 = &sensor_data[0];
//  outputF32 = &filtered_data[0];
//  
//  /* Call FIR init function to initialize the instance structure. */
//  arm_fir_init_f32(&S, NUM_TAPS, (float32_t *)&firCoeffs32[0], &firStateF32[0], blockSize);

  /* ----------------------------------------------------------------------
  ** Call the FIR process function for every blockSize samples
  ** ------------------------------------------------------------------- */
//  for(int i=0; i < numBlocks; i++)
//  {
//    arm_fir_f32(&S, inputF32 + (i * blockSize), outputF32 + (i * blockSize), blockSize);
//    Serial.println(sensor_data[i]); Serial.print(" "); Serial.print(filtered_data[i]); Serial.print(" "); 
//    delay(100);
//  }
//
//  Serial.print("Filter complete!");
  
};

void display(){
    
    for(int i=0; i<numSamples; i++){
      Serial.println(sensor_data[i]); Serial.print(" "); Serial.println(filtered_data[i]); Serial.print(" "); 
      delay(100);
    }
}

//String generatePayload() {
//
////  aJsonObject* root = aJson.createObject();
////  aJsonObject* data;
////  
////  for(int i=0; i<numSamples; i++){
////      str_data = String(mapped_data[i], 2);
////      aJson.addItemToObject(root, "ECG_Data", data);
////      if(data != NULL){
////        aJson.addStringToObject(data, "testVals", str_data);
////      }
////  }

////    String payload = aJson.print(root);
//
////  String data;
////  for(int i=0; i<numSamples; i++){
////    str_data = String(mapped_data[i], 2);
////    
////    data = "{\"DATA_TEST\": ";
////    data += str_data;
////    data += "}";
//    
////    }
////
////  data += "]";
//  //NEED TO CONVERT DATA TO AN ARRAY OF STRINGS RATHER THAN JUST ONE LONG ONE!
//
////  Serial.println("\nPayload generated! "); Serial.print("Length: "); Serial.print(data.length());
////  return payload;
//}

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
    
    
    
    
    
    
