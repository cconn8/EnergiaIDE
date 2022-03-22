#define LED RED_LED

void setup() {
  // put your setup code here, to run once:
  Serial.print(9600);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
}

void loop() {
  // put your main code here, to run repeatedly: 
  while(1){
    digitalWrite(LED, HIGH);
    delay(10);
    digitalWrite(LED, LOW);
    delay(1000);
  }
}
