  
// Neopixel Salt Lamp v2.0
// By Chris Barrett - Polaris Interplanetary - Special Projects Division
// Supports two modes; All pixel colour fade, and 8 pixel RGB mixer. All modes are asynchronous, and compartmentalized into functions.

//  January 2017  - began to implement relative intervals to control the rate of colours firing and ensure better spacing and pacing.
//  Dec  2016     - Changed shimmer to use a different array based system to try and finally end flickering.
//  June 2016     - Re-implemented rotary encoder functions for brightness control
//                - New timing function to space out updates
//                - LIGHTNING FUNCTION 
//                - produces a quicker shimmer effect, may replace shimmer with it later.
//  January 2016  - Shimmer has been overhauled. Cleaner, faster and more productive. NO MORE MID FADE FLICKER?
//                - Collision detection has been implemented, not sure if it works, will need to check further.

#include <Adafruit_NeoPixel.h>  //  Neopixel library
//#include <avr/power.h>        //  Not sure why this is here, lol
#include <Encoder.h>            //  Rotary encoder library.


#define PIN            6        //  Which pin on the Arduino is connected to the NeoPixels


#define NUMPIXELS      37      //  Highest pixel location. 
#define LOWPIXELS      0       //  Lowest pixel location This was helpful in prototyping because I could use a long strip of lights and only address the last few and stick them in the salt crystal.



// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

Encoder myEnc(2, 3);                                //  Encoder function variables

//--------------------------------------------------//  TIMER STUFF


/* These are from Blink without Delay 
Shows another way to execute delays. */
unsigned long previousMillis = 0;
unsigned long lastbolt = 0;
unsigned long interval[30];
int maxb;


int butpin = 5;                                     //  integer to store button pin
int butval;                                         //  Button digital read variable
int butsel = 1;                                     //  Button mode selection variable.
int butbounce;                                      //  button debounce
int rcount;                                         //  Counter for red
int gcount;                                         //  Counter for green
int bcount;                                         //  Counter for blue
int rval;                                           //  int stores red brightness
int gval;                                           //  int stores green brightness
int bval;                                           //  int stores blue brightness
int rgo;                                            //  red go int, 0 for off, 1 for upwards, 2 for downwards
int ggo;                                            //  green go int, 0 for off, 1 for upwards, 2 for downwards
int bgo;                                            //  blue go int, 0 for off, 1 for upwards, 2 for downwards
int magicnumber;                                    //  magic number determines when to fire colours
int supbright;
int interntimer;

//--------------------------------------------------//  The following values are for the "Shimmer" routine
int PixRay[NUMPIXELS][7];
long oldPosition  = 255;                            //  integer to store encoder value
int firstrun = 0;                                   //  array to write first run values to pixels
int randosparkle =0;                                //  integer to hold the random number value

//--------------------------------------------------// LIGHTNING
int boltgo[8];
int boltloc[8];
int boltbright[8];
int boltupdn[8];
int boltready[8];

void setup() {                                      //-----------------------------------------------(Setup)
    pinMode(butpin, INPUT);                         //  Set buttonpin as input
 //   Serial.begin(9600);                           //  Initialize Serial Debug
   pixels.begin();                                  //  This initializes the NeoPixel library.

    for (int i=0; i <= (NUMPIXELS-1); i++){             //  set magic number for all pixray locations
      PixRay[i][5] = {i};
    }

}

void loop() {                                       //-----------------------------------------------(Main Loop)

  if (firstrun == 0){
  }
  
  butcheck();

unsigned int tnow = millis();

   // some other logic here would decide when to change the LED state
   // For this sample, just toggle every 4 seconds
  if((tnow - previousMillis) >= interval[0]) {        
      
      if (oldPosition > 1 && oldPosition < 255){
        supbright = oldPosition;
      }
    
      if (oldPosition < 0){
        supbright = 0;
      }
    
      if (oldPosition > 255){
        supbright = 255;
      }
      
      
      if (butsel == 1){
          clearscreen();
          shimmer();
      }
      
      if (butsel == 2){
          clearscreen();
          colourfade();
      }
    
      if (butsel == 3){
          clearscreen();
      }
    
      pixels.show();                                  // This sends the updated pixel color to the hardware.
    previousMillis = tnow;
}
  
}

void clearscreen(){                                 //-----------------------------------------------(clears the strip)
    for(int i=0;i<NUMPIXELS;i++){
        pixels.setPixelColor(i, pixels.Color(0,0,0));
    }  
}

void butcheck(){                                    //-----------------------------------------------(Button Check Function)
  

    
    long newPosition = myEnc.read();
    if (newPosition != oldPosition) {


    if (newPosition >= 100){
      myEnc.write(100);
    }
    if (newPosition < 0){
      myEnc.write(0);
    }
        oldPosition = newPosition;
       // Serial.println(newPosition);
    }

    interval[0] = map(newPosition, 0, 100, 80, 0);
    maxb = map(newPosition, 0, 100, 0, 255);
    
    pinMode(butpin, INPUT);
    butval = digitalRead(butpin);                   // check button pin for a high


    
    if (butval == HIGH){                            // if high signal read add one to the button selection variable
      butbounce = 1;
    }

    if (butbounce == 1 && butval == LOW){
     butsel = butsel + 1;
     butbounce = 0; 
    }

    if (butsel > 2){                               //  resets the button value variable. I use this method so I can add more modes if I want later.
    butsel = 1;
    }
}

void colourfade(){                                  //-----------------------------------------------(Colour Fade Function)

    magicnumber = random(0,30);                     //creates a magic number

    if (magicnumber == 3 && rgo == 0){              //checks to see if magic number has selected red and red is not currently firing
      rgo = 1;                                      //sets the red go bit to 1 meaning positive alternation
    }
    
    if (magicnumber == 10 && ggo == 0){              //checks to see if magic number has selected green
      ggo = 1;
    }
    
    if (magicnumber == 20 && bgo == 0){             //checks to see if magic number has selected blue
      bgo = 1;        
    }


    // ---------------------------------------------// Write Red
      
    if (rgo == 1){                                  // checks for ascent stage bit, adds one to red brightness 
      rval = rval + 1;
    }
    
    if (rgo == 2){                                  // checks for descent stage bit, subtracts one from brightness
      rval = rval - 1;
    }
    
    if (rval >= maxb){                               // sets red bit to 2 meaning negative alternation
      rgo = 2;
    }
    
    if (rval == 0 && rgo == 2){                     // when counter runs out and go bit is 2, reset everything.
      rgo = 0;
      rval = 0;
    }

    // ---------------------------------------------// Write Blue
      
    if (bgo == 1){                                  // checks for ascent stage bit, adds one to blue brightness 
      bval = bval + 1;
    }
    
    if (bgo == 2){                                  // checks for descent stage bit, subtracts one from brightness
      bval = bval - 1;
    }
    
    if (bval >= maxb){
      bgo = 2;
    }
    
    if (bval == 0 && bgo == 2){
      bgo = 0;
      bval = 0;
    }

    // ---------------------------------------------// Write Green
      
    if (ggo == 1){                                  // checks for ascent stage bit, adds one to green brightness 
      gval = gval + 1;
    }
    
    if (ggo == 2){                                  // checks for descent stage bit, subtracts one from brightness
      gval = gval - 1;
    }
    
    if (gval >= maxb){
      ggo = 2;
    }
    
    if (gval == 0 && ggo == 2){
      ggo = 0;
      gval = 0;
    }
    
    

    
    for(int i=LOWPIXELS;i<NUMPIXELS;i++){
        pixels.setPixelColor(i, pixels.Color(rval,gval,bval));
    }

  
}

void shimmer() {                                    //-----------------------------------------------(Shimmer Function)

    
     randosparkle = random(0,(NUMPIXELS*10));                   //picks a random number
      

                                                    
    for (int i=0; i <= (NUMPIXELS-1); i++){                     //For loop to check random number against magic numbers and set the pixel for activation
      if (PixRay[i][5] == randosparkle && PixRay[i][0] == 0){                       
        PixRay[i][0] = 1;
        PixRay[i][1] = random(0,3);
      }   
    } 

//    unsigned int tnow = millis();
    
 //   if((tnow - previousMillis) >= interval[) { 
      for (int i=0; i <= (NUMPIXELS-1); i++){                     //For loop to activate a pixel
          if (PixRay[i][0] == 1){                       
              drawPixel(i);
          }
      }
//      previousMillis = tnow;
//    }                                
}

void drawPixel(int pixelv){              //-----------------------------------------------(Red Pixel Draw Function)



    if (PixRay[pixelv][4] == 0){                      //check if counter is within envelope
        PixRay[pixelv][2] = PixRay[pixelv][2] + 1; 
    }
  
    if (PixRay[pixelv][4] == 1){                      //check if counter is within envelope
        PixRay[pixelv][2] = PixRay[pixelv][2] - 1; 
    }

    if (PixRay[pixelv][2] >= maxb){                    //check if counter is within second half of fade    
      PixRay[pixelv][4] = 1;
      PixRay[pixelv][2] = maxb;     
    }

    if (PixRay[pixelv][1] == 0){
      pixels.setPixelColor(pixelv, pixels.Color(PixRay[pixelv][2],0,0));
    }  

    if (PixRay[pixelv][1] == 1){
      pixels.setPixelColor(pixelv, pixels.Color(0,PixRay[pixelv][2],0));
    }  

    if (PixRay[pixelv][1] == 2){
      pixels.setPixelColor(pixelv, pixels.Color(0,0,PixRay[pixelv][2]));
    }     
  
    
    if (PixRay[pixelv][2] <= 0 && PixRay[pixelv][4] == 1){               //check if counter is done alternation                                                                      
       pixels.setPixelColor(pixelv, pixels.Color(0,0,0));                //turn selected pixel off
       PixRay[pixelv][2] = 0;                                            //set brightness to zero
       PixRay[pixelv][0] = 0;                                            //deactivate pixel activate bit
       PixRay[pixelv][4] = 0;                                            //reset counter
    }
  
}

