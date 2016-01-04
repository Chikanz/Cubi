//TEEEEMP
int brightnessLevels[] =
{
	50,
	30,
	20,
	10,
	5,
	2,
	1,
	0,
};
uint16_t dotCol = colors[displayCol2];

int BrightnessReturn(bool setNow)
{	
	int prevbright = targetBrightness;
	int bright;

	//TODO: Find the set brightness and set it to brightnessGuage
	while (1 < 2)
	{
		matrix.fillScreen(0);

		pirrana(-2, 0, 500);

		if (brightnessGuage >= 0 && brightnessGuage <= 7)
			brightnessGuage = ReadRotary(brightnessGuage);
		else
			brightnessGuage = force(brightnessGuage, 0, 7);

		matrix.drawLine(0, 0, 0, 8, colors[displayCol2]);
		matrix.drawPixel(0, brightnessGuage, dotCol);

		if (brightnessGuage == 7)
			matrix.fillScreen(0);

		targetBrightness = brightnessLevels[brightnessGuage];
		bright = brightnessLevels[brightnessGuage];
		SetLight();

		if (brightnessGuage == 6)
			RedColours();
		else
			unRedColours();

		if (buttonPressed())
		{
			
			if(!setNow)
				targetBrightness = prevbright;

			brightnessGuage = 2;
			return bright;
		}

		if (digitalRead(2) == LOW)
			canPress = true;

		delay(20);
		matrix.show();
	}
}
