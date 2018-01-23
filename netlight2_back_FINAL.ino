#include <FastLED.h>

#define S1_ECHO    12
#define S1_TRIGGER 11

#define TOTAL_DISTANCE  100

#define BRIGHTNESS  64
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB

#define NUM_STRIPS 6
#define NUM_LEDS_PER_STRIP 60
CRGB leds[NUM_STRIPS][NUM_LEDS_PER_STRIP];

int ledArray[NUM_LEDS_PER_STRIP];
int personPosition;

int durationSensor1;
int distanceSensor1;

int startingStrip = 1;

int movement = 0;


TBlendType    currentBlending;


void setup() {
  delay( 3000 );
  Serial.begin(115200);
  FastLED.addLeds<LED_TYPE, 1, COLOR_ORDER>(leds[0], NUM_LEDS_PER_STRIP).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<LED_TYPE, 6, COLOR_ORDER>(leds[1], NUM_LEDS_PER_STRIP).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<LED_TYPE, 2, COLOR_ORDER>(leds[2], NUM_LEDS_PER_STRIP).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<LED_TYPE, 3, COLOR_ORDER>(leds[3], NUM_LEDS_PER_STRIP).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<LED_TYPE, 4, COLOR_ORDER>(leds[4], NUM_LEDS_PER_STRIP).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<LED_TYPE, 5, COLOR_ORDER>(leds[5], NUM_LEDS_PER_STRIP).setCorrection( TypicalLEDStrip );

  FastLED.setBrightness(  BRIGHTNESS );

  currentBlending = LINEARBLEND;

  pinMode(S1_TRIGGER, OUTPUT); 
  pinMode(S1_ECHO, INPUT);

  setupBaseStrips();


}

void loop() {
  
  DeterminePersonPosition();
  startingStrip = random(1, 5);
  RandomizePath();   
}



void RandomizePath()
{
  int movementDirection;
  int previousLed;
  int currentLed;
  int horizontalRandom = random(2, 6);
  int randomCount = 0;
  int switcheroo;
  unsigned long currentMillis = millis();


    for(int ledNo = 0; ledNo < NUM_LEDS_PER_STRIP; ledNo++) {
     switcheroo = random(1, 10);
     randomCount++;
     if(ledNo == 0){
        ledArray[0] = startingStrip;
        if(startingStrip > 1)
        {
          movementDirection = 1;
        }
        else if(startingStrip < 2)
        {
          movementDirection = -1;
        }
        
     }

     if(ledNo > 0)
     {
      previousLed = ledArray[ledNo - 1];

     if(previousLed < 2)
     {
      movementDirection = 1;
     } else if(previousLed > 4)
     {
      movementDirection = -1;
     } else if (switcheroo == 1)
     {
      if (movementDirection = 1)
      {
        movementDirection = -1;
      }
      else
      {
        movementDirection = 1;
      }
     }

     if(randomCount == horizontalRandom)
     {
      randomCount = 0;
      horizontalRandom = random(2, 6);
      if(movementDirection == 1){
      currentLed = previousLed + 1;
     } else if(movementDirection == -1)
     {
      currentLed = previousLed - 1;
     }         
     } else 
     {
      currentLed = previousLed;
     }

     ledArray[ledNo] = currentLed;    

     }    
      
      
    }
    

      
      if(movement == 1)
      {
        Serial.println("Movement detected");
        
        fillLeds();
      }

      
    

    
}

    


void DeterminePersonPosition()
{
  int increment = TOTAL_DISTANCE / NUM_STRIPS;  
  
  unsigned long currentMillis = millis();

  //if (currentMillis - previousMillis >= secondInterval) {
        digitalWrite(S1_TRIGGER, LOW);
        delayMicroseconds(2);

        digitalWrite(S1_TRIGGER, HIGH);
        delayMicroseconds(10);
        digitalWrite(S1_TRIGGER, LOW);

        durationSensor1 = pulseIn(S1_ECHO, HIGH);
        distanceSensor1 = durationSensor1*0.034/2;


//        if(distanceSensor1 <= increment){
//          startingStrip = 1;
//        } else if(distanceSensor1 <= (increment * 2)) {
//          startingStrip = 2;
//        } else if(distanceSensor1 <= (increment * 3)) {
//          startingStrip = 3;
//        } else if(distanceSensor1 <= (increment * 4)) {
//          startingStrip = 4;          
//        } else if(distanceSensor1 <= (increment * 5)) {
//          startingStrip = 5;         
//        } else {
//          startingStrip = 2;
//        }

        if(distanceSensor1 < TOTAL_DISTANCE)
        {
          
          movement = 1;
        }
        else
        {
          movement = 0;
        }

        //previousMillis = currentMillis;
  //}
}

void setupBaseStrips()
{
  Serial.println("setting up strips");
    for(int i = 0; i < NUM_STRIPS; i++) {
    fill_solid( leds[i], NUM_LEDS_PER_STRIP, CHSV( 64, 100, 100));
  }

  FastLED.show();
}

void cleanStrips()
{
  Serial.println("cleaning strips");
//  for(int i = 0; i < NUM_STRIPS; i++) {
//      //fill_solid( leds[i], NUM_LEDS_PER_STRIP, CRGB(0,0,0));
//       leds[i].nscale8( 192);
//  }

  


  for(int x = 0; x < NUM_LEDS_PER_STRIP; x++) {
  
    for(int i = 0; i < NUM_STRIPS; i++) {
      
        leds[i][x] = CHSV( 64, 100, 100);
        FastLED.show();
        //leds[x][i] = 0x41FF0D;    
     
    }
  }
}
void fillLeds()
{
  Serial.println("filling leds");
  int lastLed;
  int secondToLastLed;
  int thirdToLastLed;
  
  for(int x = 0; x <= NUM_LEDS_PER_STRIP; x++) {
    // This inner loop will go over each led in the current strip, one at a time
    for(int i = 0; i < NUM_STRIPS; i++) {
            
      if(i == ledArray[x])
      {
        leds[i][x] = CHSV( 95, 255, 255);
 
          leds[thirdToLastLed][x-3] = CHSV( 110, 180, 180);
 

          leds[secondToLastLed][x-2] = CHSV( 105, 200, 200);

          leds[lastLed][x-1] = CHSV( 100, 220, 220);
   
        
        FastLED.show();

        thirdToLastLed = secondToLastLed;
        secondToLastLed = lastLed;
        lastLed = i;

        Serial.println(x);
        

        if( x == NUM_LEDS_PER_STRIP -1 )
        {
          Serial.println("before delay");
          delay(1000);
          cleanStrips();
          Serial.println("after delay");

        }
      } 
      delay(10);
    }
  }
  
}

