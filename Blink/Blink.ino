/*
  Blink
  The basic Energia example.
  Turns on an LED on for one second, then off for one second, repeatedly.
  Change the LED define to blink other LEDs.
  
  Hardware Required:
  * LaunchPad with an LED
  
  This example code is in the public domain.
*/

// most launchpads have a red LED
#define LED RED_LED
#define LED2 GREEN_LED

//see pins_energia.h for more LED definitions
//#define LED GREEN_LED

int brightness = 0;
int fadeAmount = 5;
  
// the setup routine runs once when you press reset:
void setup() {                
  // initialize the digital pin as an output.
  pinMode(LED1, OUTPUT);    
  pinMode(LED2, OUTPUT); 
}

// the loop routine runs over and over again forever:
void loop() {
    //digitalWrite(LED1, HIGH);   // turn the LED on (HIGH is the voltage level)
    //delay(1000);               // wait for a second
    //digitalWrite(LED1, LOW);    // turn the LED off by making the voltage LOW
    //digitalWrite(LED2, HIGH);
    //delay(1000);               // wait for a second
    //digitalWrite(LED2, LOW);

    //set the brightness of
    //analogWrite(LED2, brightness);

    //change the brightness for next time through the loop
    //brightness = brightness + fadeAmount;

    //if (brightness == 0 || brightness == 255){
    //  fadeAmount = -fadeAmount;
    //}

    //wait for 30 milliseconds to see the dimming effect
    //delay(30);
    
}
