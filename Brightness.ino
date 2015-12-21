//TEEEEMP
int brightnessLevels[] =
{
	50,
	30,
	20,
	10,
	5,
	1,
	1,
	0,
};
uint16_t dotCol = colors[displayCol2];

void BrightnessSelect()
{
	pirrana(-2, 0, 500);

	if (brightnessGuage >= 0 && brightnessGuage <= 7)
		brightnessGuage = ReadRotary(brightnessGuage);
	else
		brightnessGuage = force(brightnessGuage, 0, 7);


	matrix.drawLine(0, 0, 0, 8, colors[displayCol2]);

	if (brightnessGuage == 7)
	{
		matrix.fillScreen(0);
	}

	matrix.drawPixel(0, brightnessGuage, dotCol);

	targetBrightness = brightnessLevels[brightnessGuage];

	if (brightnessGuage == 6)
		RedColours();
	else
		unRedColours();

	delay(20);
}
