/* ===================================
 *  BASIC SETUP CODE TO GET THINGS UP AND RUNNING
 * =================================== */


const int analogInput = 5;  //P4.1 (A12) analogRead()
const int analogOutput = 19; //P2.5  analogWrite()
const int lead0 = 39;  //P3.5   GPIO I/O
const int lead1 = 38;  //P3.7   GPIO I/O

/* ==== VARIABLES ====== */

float sensorValue = 0;  //Store incoming data from AD8232
float outputValue = 0;  //Store mapped output to send
String str_output;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);         //opens serial connection to the board
  analogReadResolution(14);   //set analog resolution to 14 bits (default)  
  pinMode(lead0, INPUT);      //set GPIO pins as Input
  pinMode(lead1, INPUT);


}

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

  //add short delay for the analog-to-digital converter to settle
  delay(10);

  }
  
}
