/* Arduino SKETCH UltraSonic rangefinder
 Runs HC-04 Ultrasonic Module
 Uses: Ultrasonic Library (Copy to Arduino Library folder)
 http://iteadstudio.com/store/images/produce/Sensor/HCSR04/Ultrasonic.rar
 original by terry@yourduino.com 
 Version by Anne Barela based on ideas of Dick Hill*/

/*-----( Import needed libraries )-----*/
#include "Ultrasonic.h"
/*-----( Declare Constants and Pin Numbers )-----*/
#define  TRIG_PIN  2  // ultrasonic trigger pin is on arduino input D2 (purple)
#define  ECHO_PIN  3  // ultrasonic echo pin is on arduino input D3 (gray)
#define  NEAR_POT  0  // the pot on the left controls the red to yellow distance
#define  FAR_POT   1  // the pot on the right controls the yellow to green distance
#define  NEAR_REFERENCE 7  // 7 centimeters was determined to be the short reference value empiracally
#define  FAR_REFERENCE 24  // 24 centimeters was determined to be the far reference empiracally
/*-----( Declare objects )-----*/
Ultrasonic OurModule(TRIG_PIN, ECHO_PIN); // sets up ultrasonic reader code in library
/*-----( Declare Variables )-----*/
unsigned int distanceValue=0;  // distance read in centimeters (2.54 cm = 1 inch)

int ledState = LOW; // start yellow LED unlit
long previousMillis = 0; // hold variable for timer count
long interval = 750; // initial blink interval

void setup()   /****** SETUP: RUNS ONCE ******/
{
  Serial.begin(9600); // Uncomment to debug via USB Port
  Serial.println("UltraSonic Distance Measurement");
  
  pinMode(5, OUTPUT); // green
  pinMode(6, OUTPUT); // yellow
  pinMode(7, OUTPUT); // red
}

void loop()  
{
  unsigned long currentMillis; // keep track of time to blink yellow according to distance
  unsigned int shortpot; // read value of short to intermediate potentiometer (left)
  unsigned int longpot;  // read value of intermediate to long potentiometer (right)
  unsigned int shortvalue;
  unsigned int longvalue;
  
  shortpot = analogRead(NEAR_POT); // get current values
  longpot  = analogRead(FAR_POT);
  shortvalue = map(shortpot, 0, 1023, 2, NEAR_REFERENCE*2); // map pot 0-1023 to distance 2 to NEAR_REFERENCE times two cm
  longvalue =  map(longpot, 0, 1023, shortvalue+2, FAR_REFERENCE*2); // map right pot to be left pot+2 to 2 times FAR_REFERENCE
  Serial.print("Red to Yellow boundary (left pot) set at ");
  Serial.print(shortvalue);
  Serial.println(" centimeters.");
  Serial.print("Yellow to Green boundary (right pot) set at ");
  Serial.print(longvalue);
  Serial.println(" centimeters.");
   
  distanceValue=OurModule.Ranging(CM); // read module in centimeters
  //Serial.print(OurModule.Ranging(CM));
  //Serial.println("cm");
  
  currentMillis = millis();  // get timer value for yellow blink
  
  if(distanceValue <= shortvalue)   // if distance is close (setable by left pot), turn on red light
    { digitalWrite(5,LOW); // green off
      digitalWrite(6,LOW); // yellow off
      digitalWrite(7,HIGH); // red on
    }
  else if(distanceValue > shortvalue && distanceValue < longvalue) // if in intermediate distance turn on yellow blink
    { interval=map(distanceValue,shortvalue,longvalue,100,750); // The closer the distance, the shorter the interval (faster it blinks)
      // note 100 milliseconds low and 750 milliseconds high above are empiracle and may be changed later if desired
      if (currentMillis-previousMillis > interval) // If enough time has elapsed between blink change
        {
          previousMillis = currentMillis;
          if(ledState==LOW)  // toggle LED state
             ledState=HIGH;
          else
             ledState=LOW;
        }    
      digitalWrite(5,LOW); // green off
      digitalWrite(6,ledState); // yellow set state
      digitalWrite(7,LOW); // red off
    }
    else // distance > far value
      { digitalWrite(5,HIGH); // if distance far then turn on green light
        digitalWrite(6,LOW); // yellow off
        digitalWrite(7,LOW); // red off
      }
      
  delay(100); // wait a tenth of a second before checking again (this can be changed if too fast, min=100 for ultrasonic)

} //--(end main loop )---




