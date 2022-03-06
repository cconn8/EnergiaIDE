const int analogInput = A13;  //P4.2
const int lead0 = 39;  //P2.6
const int lead1 = 38;  //P2.4

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

  Serial.println("ADC 10-bit (default)");
  for(int i=0; i<50; i++){
  // read input on A0 at default resolution (10 bits) 
  // send it out serial connection back to Energia IDE
  analogReadResolution(10);
  //Serial.println("ADC 10-bit (default) : ");
  Serial.println(analogRead(analogInput));
  }

  delay(1000);

  Serial.println("ADC 12-bit : ");
  for(int i=0; i<50; i++){
  //change resolution to 12 bits
  analogReadResolution(12);
  //Serial.println("ADC 12-bit : ");
  Serial.println(analogRead(analogInput));

  }

  delay(1000);

  Serial.println("ADC 14-bit");  
  for(int i=0; i<50; i++){
  //change resolution to 14 bits
  analogReadResolution(14);
  //Serial.println("ADC 14-bit : ");
  Serial.println(analogRead(analogInput));
  }

  delay(1000);
  }

  
}
