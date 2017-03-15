
/* COLOUR FADE FUNCTION */
void colourfade()
{
randomNumber = random(0,30); // generates a 'random' number

/* based on the random number, flag specific colour for effect */
if (randomNumber > 0 && randomNumber < 10 && redGo == 0){redGo = 1;}
if (randomNumber > 10 && randomNumber < 20 && greenGo == 0){greenGo = 1;}
if (randomNumber > 20 && randomNumber < 30 && blueGo == 0){blueGo = 1;}

/* RED */
if (redGo == 1){rval++;} // if flag set to increment, add to val
if (redGo == 2){rval--;} // if flag set to decrement, subract from val
if (rval >= maxBright){redGo = 2;} // if val reaches max, switch to decrement
if (rval == 0 && redGo == 2){redGo = 0;} // once val reaches zero, cease updating

/* BLUE */
if (blueGo == 1){bval++;}
if (blueGo == 2){bval--;}
if (bval >= maxBright){blueGo = 2;}
if (bval == 0 && blueGo == 2){blueGo = 0;}

/* GREEN */
if (greenGo == 1){gval++;}
if (greenGo == 2){gval--;}
if (gval >= maxBright){greenGo = 2;}
if (gval == 0 && greenGo == 2){greenGo = 0;}

for(int i=0;i<stripMax;i++){
  pixels.setPixelColor(i, pixels.Color(rval,gval,bval));
}

}


/* SHIMMER FUNCTION */
void shimmer()
{
  int randosparkle = random(0,(stripMax*10)); // picks a random number
  for (int i=0; i <= (stripMax-1); i++)//For loop to check random number against magic numbers and set the pixel for activation
  {
    if (pixelData[i][5] == randosparkle && pixelData[i][0] == 0)
    {
      pixelData[i][0] = 1;
      pixelData[i][1] = random(0,3);
    }
  }
  long presentTime = millis();
  if((presentTime - previousTime) >= interval[0])
  {
        for (int i=0; i <= (stripMax-1); i++)
        {
            if (pixelData[i][0] == 1)
            {
                drawPixel(i);
            }
        }
        previousTime = presentTime;
  }
}
