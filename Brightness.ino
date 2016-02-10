//TEEEEMP
int brightnessLevels[] =
{
	50,
	30,
	20,
	10,
	5,
	1,
	1, // Redded, 6
	0,
};
uint16_t dotCol = colors[displayCol2];

int BrightnessReturn(bool setNow)
{
	int prevbright = brightnessLevel;
	brightnessGuage = brightnessLevel;

	while (1 < 2)
	{
		matrix.fillScreen(0);

		pirrana(-2, 0, 500);

		brightnessGuage = ReadRotary(brightnessGuage, false);
		brightnessGuage = constrain(brightnessGuage, 0, 7);

		matrix.drawLine(0, 0, 0, 8, colors[displayCol2]);
		matrix.drawPixel(0, brightnessGuage, dotCol);

		if (brightnessGuage == 7)
			matrix.fillScreen(0);

		SetBrightness(brightnessGuage, true);

		SetLight();

		if (brightnessGuage == 6)
			redColours();
		else
			unRedColours();

		if (buttonPressed())
		{
			if (!setNow)
			{
				SetBrightness(prevbright);
				unRedColours();
			}

			return brightnessGuage;
		}

		if (digitalRead(2) == LOW)
			canPress = true;

		delay(20);
		matrix.show();
	}
}

void brightnessProfile()
{
	//Brightness Profile

	//Force the cursor pos and get input
	cursorPos = ReadRotary(cursorPos, true);
	cursorPos = constrain(cursorPos, 0, 6);

	//Button press
	if (buttonPressed())
	{
		if (cursorPos == 6)
		{
			EEPROM.put(10, BProfile);
			cursorPos = 0;
			oke(true);
		}
		else
		{
			if (BProfile[cursorPos].active)
			{
				BProfile[cursorPos].active = false;
				BProfile[cursorPos].time = { 0,0 };
			}
			else
			{
				BProfile[cursorPos].active = true;
				BProfile[cursorPos].time = TimeSetReturn(false, {0, 0});
				BProfile[cursorPos].level = BrightnessReturn(false);
				oke(false);
			}
		}
	}

	//Display the time above selection
	if (cursorPos == 6)
		backIcon(1, 0, colors[displayCol2], false);
	else if (BProfile[cursorPos].active)
		displayTimeSimple(BProfile[cursorPos].time, Med, false, false);

	//Draw Pixels
	for (int i = 0; i < 6; i++)
	{
		if (BProfile[i].active)
			matrix.drawPixel(i + 1, 7, colors[1]);
		else
			matrix.drawPixel(i + 1, 7, colors[6]);
	}
	matrix.drawPixel(7, 7, colors[displayCol1]);

	//Create a blink effect
	if (onHalfSecond())
		matrix.drawPixel(cursorPos + 1, 7, matrix.Color(0, 0, 0));

	delay(50);
}

void SetBrightness(int level)
{
	brightnessLevel = level;
	targetBrightness = brightnessLevels[level];
	EEPROM.write(6, brightnessLevel);
	Serial.println("Writing brightnes to EEPROM");
}

//Don't write to the eepRom 
void SetBrightness(int level,bool dontWrite)
{
	brightnessLevel = level;
	targetBrightness = brightnessLevels[level];
}