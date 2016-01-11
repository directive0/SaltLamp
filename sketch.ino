
// Neopixel Salt Lamp v1.2
// By Chris Barrett - Polaris Interplanetary - Special Projects Division
// Supports two modes; All pixel colour fade, and 8 pixel RGB mixer. All modes are asynchronous, and compartmentalized into functions.
// January 2016 - Shimmer has been overhauled. Cleaner, faster and more productive. NO MORE MID FADE FLICKER! 
//              - Collision detection has been implemented, not sure if it works, will need to check further.

#include <Adafruit_NeoPixel.h>
#include <avr/power.h>
#include <Encoder.h>

// Which pin on the Arduino is connected to the NeoPixels?
#define PIN            6

// what address are the pixels going to range? This was helpful in prototyping because I could use a long strip of lights and only address the last few and stick them in the salt crystal.
#define NUMPIXELS      133
#define LOWPIXELS      90 



// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

Encoder myEnc(2, 3);
int butpin = 5;                                     // integer to store button pin
int butval;                                         // Button digital read variable
int butsel = 2;                                     // Button mode selection variable.
int butbounce;                                      // button debounce
int rcount;                                         // Counter for red
int gcount;                                         // Counter for green
int bcount;                                         // Counter for blue
int rval;                                           // int stores red brightness
int gval;                                           // int stores green brightness
int bval;                                           // int stores blue brightness
int rgo;                                            // red go int, 0 for off, 1 for upwards, 2 for downwards
int ggo;                                            // green go int, 0 for off, 1 for upwards, 2 for downwards
int bgo;                                            // blue go int, 0 for off, 1 for upwards, 2 for downwards
int magicnumber;                                    // magic number determines when to fire colours


//--------------------------------------------------// The following values are for the "Shimmer" routine
int pixelvalue[8];                                  // array to store the address location of each pixel event, 8 in total
int pixelgo[8];                                     // array to store the activation bit for each pixel event, 8 in total
int pixmax = 512;                                   // int to store the max animation keyframe for each pixel event
int pixelcol[8];                                    // array to store the colour decision
int pixcoldic[8];                                   // array to store the colour decision bit
int pixmat[8];                                      // array to indicate whether a pixel is stepping on another pixel
int delayval;                                       // integer to hold delay value
int randosparkle =0;                                // integer to hold the random number value
int randosparklenum[] = {1,30,54,43,2,100,150,97};  // integer to hold the magic numbers governing which pixel fires next
int maxbright;                                      // integer to determine max bright
long oldPosition  = 255;                            // integer to store encoder value
int pixupdn[8];                                     // array to store the pixel ascent/descent for each possible pixel
int pixbrt[8];                                      // array to store the pixel brightness for each possible pixel
int pixpos;                                         // array to store the pixel brightness for each possible pixel
int firstrun = 0;                                   //array to write first run values to pixels
int shimspd[8];                                    //array to set random speed value for shimmer
int pixchck[8];

void setup() {                                      //-----------------------------------------------(Setup)
    pinMode(butpin, INPUT);                         // Set buttonpin as input
    Serial.begin(9600);                             // Initialize Serial Debug
    pixels.begin();                                 // This initializes the NeoPixel library.



}

void loop() {                                       //-----------------------------------------------(Main Loop)

  if (firstrun == 0){
    for (int i=0; i <= 7; i++){                      //For loop to write initial random position
        pixelvalue[i] = random(LOWPIXELS,NUMPIXELS);
        firstrun = 1;
        }
    
  }
  butcheck();

  if (butsel == 1){
      clearscreen();
      colourfade();
  }
  
  if (butsel == 2){
      clearscreen();
      shimmer();
  }
  
 
  pixels.show();                                  // This sends the updated pixel color to the hardware.

  
  

  delay(30);
  
}

void clearscreen(){                                 //-----------------------------------------------(clears the strip)
    for(int i=0;i<NUMPIXELS;i++){
        pixels.setPixelColor(i, pixels.Color(0,0,0));
    }  
}

void butcheck(){                                    //-----------------------------------------------(Button Check Function)
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

    long newPosition = myEnc.read();
    if (newPosition != oldPosition) {
        oldPosition = newPosition;
        Serial.println(newPosition);
    }
    if (newPosition > 255){
      myEnc.write(1);
    }
    if (newPosition < 1){
      myEnc.write(255);
    }
    
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
    
    if (rval >= 255){                               // sets red bit to 2 meaning negative alternation
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
    
    if (bval >= 255){
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
    
    if (gval >= 255){
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

    pixels.setBrightness(oldPosition); 
    
    randosparkle = random(0,150);                   //creates a magic number
    
                                                    
    for (int i=0; i <= 7; i++){                     //For loop to check random number against magic number 
        
        if (randosparkle == randosparklenum[i] && pixelgo[i] == 0){
            pixcoldic[i] = random(0,9);             //Creates a magic number 
            pixelgo[i] = 1;                         //Activates a pixel for draw
            shimspd[i] = random(1,5);

            while (pixchck[i] == 0){                //collision detection
              pixmat[i] == 0;
              randomSeed(analogRead(0));
              pixelvalue[i] = random(LOWPIXELS,NUMPIXELS);
              //Serial.println("repositioning");
              //Serial.println(i);
               
              for (int b=0; b <= 7; b++){                      //For loop to write initial random position
                if (pixelvalue[i] == pixelvalue[b]){
                  pixmat[i] = pixmat[i] + 1;
                }

                if (pixmat[i] <= 1){
                  pixchck[i] = 1;
                }
              }

              
            }

        
        }
    } 
    
    for (int i=0; i <= 7; i++){                     //For loop to activate a pixel
        if (pixelgo[i] == 1 && pixcoldic[i] >= 0 && pixcoldic[i] < 3){                       
            drawPixel(i,2,shimspd[i]);
            
        }
        if (pixelgo[i] == 1 && pixcoldic[i] >= 3 && pixcoldic[i] < 6){                       
            drawPixel(i,1,shimspd[i]);
            
        }
          if (pixelgo[i] == 1 && pixcoldic[i] >= 6 && pixcoldic[i] < 9){                       
            drawPixel(i,0,shimspd[i]);
            
        }
        
}
                                     
}

void drawPixel(int pixelv,int coldic, int rate){              //-----------------------------------------------(Pixel Draw Function)



    if (pixupdn[pixelv] == 0){                      //check if counter is within envelope
        pixbrt[pixelv] = pixbrt[pixelv] + rate; 
    }
  
    if (pixupdn[pixelv] == 1){                      //check if counter is within second half of fade    
      pixbrt[pixelv] = pixbrt[pixelv] - rate;     
    }

    if (pixbrt[pixelv] >= 255){                     //check if counter is within second half of fade    
      pixupdn[pixelv] = 1;
      pixbrt[pixelv] = 255;     
    }

    if (coldic == 0){
      pixels.setPixelColor(pixelvalue[pixelv], pixels.Color(pixbrt[pixelv],0,0));
    }  

    if (coldic == 1){
      pixels.setPixelColor(pixelvalue[pixelv], pixels.Color(0,pixbrt[pixelv],0));
    }  

    if (coldic == 2){
      pixels.setPixelColor(pixelvalue[pixelv], pixels.Color(0,0,pixbrt[pixelv]));
    }     
  
    
      if (pixbrt[pixelv] <= 0 && pixupdn[pixelv] == 1){                   //check if counter is within second half of fade                                                                          
        pixels.setPixelColor(pixelvalue[pixelv], pixels.Color(0,0,0));    //turn selected pixel off
        pixelvalue[pixelv] = random(LOWPIXELS,NUMPIXELS);                 //create new random pixel location
        pixelgo[pixelv] = 0;                                              //deactivate pixel activate bit
        pixupdn[pixelv] = 0;                                              //reset counter
        pixchck[pixelv] = 0;
      }
  
}
