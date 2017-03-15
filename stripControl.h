
/* CLEARS THE STRIP */
void clearStrip()
{
    for(int i=0;i<stripMax;i++)
	{
        pixels.setPixelColor(i, pixels.Color(0,0,0));
    }
}


/* PIXEL DRAW FUNCTION */
void drawPixel(int p)
{

    if (pixelData[p][4] == 0){pixelData[p][2] = pixelData[p][2] + 1;}
    if (pixelData[p][4] == 1){pixelData[p][2] = pixelData[p][2] - 1;}
    if (pixelData[p][2] >= maxBright){pixelData[p][4] = 1;pixelData[p][2] = maxBright;}

    if (pixelData[p][1] == 0){pixels.setPixelColor(p, pixels.Color(pixelData[p][2],0,0));}
    if (pixelData[p][1] == 1){pixels.setPixelColor(p, pixels.Color(0,pixelData[p][2],0));}
    if (pixelData[p][1] == 2){pixels.setPixelColor(p, pixels.Color(0,0,pixelData[p][2]));}

    if (pixelData[p][2] <= 0 && pixelData[p][4] == 1)
    {
       pixels.setPixelColor(p, pixels.Color(0,0,0));                //turn selected pixel off
       pixelData[p][2] = 0;                                            //set brightness to zero
       pixelData[p][0] = 0;                                            //deactivate pixel activate bit
       pixelData[p][4] = 0;                                            //reset counter
    }
}
