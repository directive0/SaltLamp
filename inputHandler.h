
/* INPUT CHECK */
int buttonCheck()
{
	buttonVal = digitalRead(buttonPin);         // Read the button pin
    if (buttonVal == HIGH)                    // If button pressed...
    {
    buttonDeb = 1;                            // Flip debounce bit.
    }
    if (buttonVal == LOW && buttonDeb == 1)   // If button released, and debounce set...
    {
    menuSelect = menuSelect + 1;              // increment the menu by one.
    buttonDeb = 0;                            // and reset the debounce.
    }
    if (menuSelect > 4)                       //  If menu index greater than 2...
    {
    menuSelect = 1;                           // reset it to one.
    }
    return menuSelect;    

}


/* CHECK ROTARY ENCODER */
void rotaryCheck()
{

  
    rotaryPosition[1] = myEnc.read();      // gather current rotary position, keep it in range

    if (rotaryPosition[1] >= 255){
      myEnc.write(255);
    }
    if (rotaryPosition[1] < 0){
      myEnc.write(0);
    }
      
      if (rotaryPosition[1] != rotaryPosition[0])             // If it differs from previous position...
      {
        rotaryPosition[0] = rotaryPosition[1];                // save current position
        Serial.println(rotaryPosition[1]);                    // print the position to serial
      }

//    interval[0] = map(rotaryPosition[1], 0, 255, 1000, 0);      //  scale position, save it in first slot of interval[]
  	
  	
//
//    if (rotaryPosition[0] >= 0 && rotaryPosition[0] <= 5)
//    {
//      interval[0] = 100000;
//    } 
//        
//  	if (rotaryPosition[0] >= 6 && rotaryPosition[0] <= 50)
//    {
//      interval[0] = 33000;
//    } 
//
//    if (rotaryPosition[0] >= 51 && rotaryPosition[0] <= 100)
//    {
//      interval[0] = 1000;
//    } 
//
//    if (rotaryPosition[0] >= 101 && rotaryPosition[0] <= 255)
//    {
//      interval[0] = 0;
//    } 
//  
//    
  	
  	//Serial.println(interval[0]);
  	
  	/* ISN'T INTERNAL[0] STORING YOUR TIME ELAPSED NUMBER ON LINE 95?*/
    maxBright = map(rotaryPosition[1], 0, 255, 0, 255); // map rotary encoder to LED scale 100 ~ 255
    return;
}
