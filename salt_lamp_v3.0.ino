/*
 $$$$$$\   $$$$$$\  $$\    $$$$$$$$\ $$\        $$$$$$\  $$\      $$\ $$$$$$$\
$$  __$$\ $$  __$$\ $$ |   \__$$  __|$$ |      $$  __$$\ $$$\    $$$ |$$  __$$\
$$ /  \__|$$ /  $$ |$$ |      $$ |   $$ |      $$ /  $$ |$$$$\  $$$$ |$$ |  $$ |
\$$$$$$\  $$$$$$$$ |$$ |      $$ |   $$ |      $$$$$$$$ |$$\$$\$$ $$ |$$$$$$$  |
 \____$$\ $$  __$$ |$$ |      $$ |   $$ |      $$  __$$ |$$ \$$$  $$ |$$  ____/
$$\   $$ |$$ |  $$ |$$ |      $$ |   $$ |      $$ |  $$ |$$ |\$  /$$ |$$ |
\$$$$$$  |$$ |  $$ |$$$$$$$$\ $$ |   $$$$$$$$\ $$ |  $$ |$$ | \_/ $$ |$$ |
 \______/ \__|  \__|\________|\__|   \________|\__|  \__|\__|     \__|\__|
*/

// Neopixel Salt Lamp v3.0
// By Chris Barrett - Guidestar Interplanetary
// Refactored 01/17/2017 - JusCoz - HIRE TECH :
// - New code base integrates new formatting and streamlining to reduce filesize and complexity.
// - Tries to eleviate some flickering with low brightness, still not perfect, but much improved!


/* Neopixel dependancies and variables */
#include <Adafruit_NeoPixel.h> //  Neopixel library
#define PIN            6 //  pin connected to the NeoPixels
#define stripMax      37 //  Highest pixel location.
#define stripMin      0 //  Lowest pixel location
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(stripMax, PIN, NEO_GRB + NEO_KHZ800); // specify neopixel info


/* Encoder dependancies and variables */
#include <Encoder.h>            //  Rotary encoder library.
Encoder myEnc(2, 3);           //  Encoder function variables


/* Button input variables */
int buttonPin = 5;            //  integer to store button pin
int buttonVal;                //  Button digital read
int buttonDeb;                //  button debounce
int menuSelect = 1;           //  Button mode selection variable.


/* Rotary encoder variables */
int rotaryPosition[] = {0,0}; // array to store rotary information, past and present


/* Light pattern variables */
unsigned long presentTime = millis(); // collect present uptime
unsigned long previousTime = 0;       // zero out 'previous' uptime
unsigned long interval[30];           // variable to control the wait time between each light update. Used to slow or speed light animation.
int maxBright;                        // integer to store maximum brightness
int rval;                             //  int stores red brightness
int gval;                             //  int stores green brightness
int bval;                             //  int stores blue brightness
int redGo = 0;                        //  red go int, 0 for off, 1 for upwards, 2 for downwards
int greenGo = 0;                      //  green go int, 0 for off, 1 for upwards, 2 for downwards
int blueGo = 0;                       //  blue go int, 0 for off, 1 for upwards, 2 for downwards
int randomNumber;                     //  random number determines when to fire colours

/* 2D Pixel Data Array */
int pixelData[stripMax][7];
/*
pixelData array definitions
pixelData[p][0] --> Activation
pixelData[p][1] --> Red (Unused?)
pixelData[p][2] --> Green (Unused?)
pixelData[p][3] --> Blue (Unused?)
pixelData[p][4] --> Up or Down 
pixelData[p][5] --> Magic number
pixelData[p][6] --> Unused
*/

/* Salt lamp dependancies */
#include "inputHandler.h" // fundamental control systems
#include "stripControl.h" // baseline strip control functions
#include "lightPatterns.h" // library of light patterns

/* MAIN PROGRAM SETUP*/
void setup()
{
    pinMode(buttonPin, INPUT);                //  initialize button input
    Serial.begin(9600);                       //  initialize Serial
    pixels.begin();                           //  initialize NeoPixel
    for (int i=0; i <= (stripMax-1); i++)     //  set magic number for all pixelData locations, this code can be improved to add complexity to the random numbers
    {                                         //  for now it just sets magic numbers incrementally
      pixelData[i][5] = {i};
    }
}


/* MAIN LOOP */
void loop()
{
    // to start, check the rotary input
    rotaryCheck();
    // then check the button input, change menuSelect accordingly
    buttonCheck();
    shimmer();
    // then
    switch (menuSelect)
    {
      case 1:
        interval[0] = 500;
        break;
      case 2:
        interval[0] = 800;
        break;
      case 3:
        interval[0] = 1000;
        break;
      case 4:
        interval[0] = 1500;
        break;
      default:
        clearStrip();
      break;
    }
      pixels.show(); // This sends the updated pixel color to the hardware.

}
