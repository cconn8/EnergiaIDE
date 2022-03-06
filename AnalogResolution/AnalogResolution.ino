const int analogInput = 24;  //P4.2 analogRead()
const int analogOutput = 34; //P2.3 analogWrite()
const int lead0 = 39;  //P2.6   GPIO I/O
const int lead1 = 38;  //P2.4   GPIO I/O

//declare values
int sensorValue = 0;
int outputValue = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); //opens serial connection to the board
  pinMode(lead0, INPUT);
  pinMode(lead1, INPUT);
  
}

void loop() {

  if(lead0 == digitalRead(1) || lead1 == digitalRead(1)){
    Serial.println("No reading - place pads!");
  }
  
  else{

  //delay(1000);

  //Serial.println("ADC 14-bit");  
  //change resolution to 14 bits
  analogReadResolution(12);

  sensorValue = analogRead(analogInput);
  Serial.println(sensorValue);

  outputValue = map(sensorValue, 0, 1023, 0, 255);
  analogWrite(analogOutput, outputValue);

//  print results to serial monitor
//  Serial.print("Sensor = ");
//  Serial.print(sensorValue);
//  Serial.print("\t Output = ");
//  Serial.println(outputValue);

  //add short delay for the analog-to-digital converter to settle
  delay(1);

  }

  
}
