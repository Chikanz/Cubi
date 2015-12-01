int posOffset = 0;
int posX[] =
{
	0,
	1,
	0,
	1,

	0,
	2,
	4,
	6,
};

//Normal
void DisplayCurrentTime(int hr1, int hr2, int mn1, int mn2, int xmod)
{
	if (hourBelow10)
		minuteAlert(displayCol1, xmod);

	if (hr1 == 0)
	{
		hourBelow10 = true;
	}
	else
		hourBelow10 = false;

	//Night Mode
	if (hour() > 21 && State == DisplayTime)
	{
		//Drawing the preset night mode time

		//"1"
		matrix.drawLine(0, 0, 0, 2, Red);
		//1,2 or 3
		switch (hr2)
		{
		case 0:
			matrix.drawLine(2, 0, 2, 2, Red);
			matrix.drawLine(3, 0, 3, 2, Red);
			break;

		case 1:
			matrix.drawLine(2, 0, 2, 2, Red);
			break;

		case 2:
			matrix.drawLine(3, 0, 3, 2, Red);
			matrix.drawPixel(2, 0, Red);
			matrix.drawPixel(4, 2, Red);
			break;
		}

		if (!hourBelow10)
		{
			displayNum(mn1, 2, 4, Red, false);
			displayNum(mn2, 5, 4, Red, false);
		}
	}
	else
	{
		if (hourBelow10)
		{
			//displayNum(hr1, 1, 0, colors[displayCol1]);
			displayNum(hr2, 1 + xmod, 0, colors[displayCol2], true);
			displayNum(mn1, 4 + xmod, 1, colors[displayCol1], true);
			displayNum(mn2, 6 + xmod, 1, colors[displayCol2], true);
		}
		else
		{
			displayNum(hr1, 0 + xmod, 0, colors[displayCol1], true);
			displayNum(hr2, 2 + xmod, 0, colors[displayCol2], true);
			displayNum(mn1, 4 + xmod, 1, colors[displayCol1], true);
			displayNum(mn2, 6 + xmod, 1, colors[displayCol2], true);
		}
	}
}

//Blink
void DisplayCurrentTime(int hr1, int hr2, int mn1, int mn2, boolean Blink, int blinkpos)
{
		posOffset = 4;
		if (hr1 == 0)
		{
			hourBelow10 = true;
		}

	if (hour() > 21 && State == DisplayTime)
	{
		//Drawing the preset night mode time

		//"1"
		matrix.drawLine(0, 0, 0, 2, Red);
		//1,2 or 3
		switch (hr2)
		{
		case 0:
			matrix.drawLine(2, 0, 2, 2, Red);
			matrix.drawLine(3, 0, 3, 2, Red);
			break;

		case 1:
			matrix.drawLine(2, 0, 2, 2, Red);
			break;

		case 2:
			matrix.drawLine(3, 0, 3, 2, Red);
			matrix.drawPixel(2, 0, Red);
			matrix.drawPixel(4, 2, Red);
			break;
		}

		if (!hourBelow10)
		{
			displayNum(mn1, 2, 4, Red, false);
			displayNum(mn2, 5, 4, Red, false);
		}
	}
	else
	{
		//Normal display
		if (!Blink)
		{
			if (hourBelow10 && bigMode)
			{
				//displayNum(hr1, 1, 0, colors[displayCol1]);
				displayNum(hr2, 1, 0, colors[displayCol2], true);
				displayNum(mn1, posX[2 + posOffset], 1, colors[displayCol1], true);
				displayNum(mn2, posX[3 + posOffset], 1, colors[displayCol2], true);
			}
			else
			{
				displayNum(hr1, posX[0 + posOffset], 0, colors[displayCol1], true);
				displayNum(hr2, posX[1 + posOffset], 0, colors[displayCol2], true);
				displayNum(mn1, posX[2 + posOffset], 1, colors[displayCol1], true);
				displayNum(mn2, posX[3 + posOffset], 1, colors[displayCol2], true);
			}
		}
		else
		{
			switch (blinkpos)
			{
			case 0:
				if (onHalfSecond())
					displayNum(hr1, posX[0 + posOffset], 0, colors[displayCol1], true);
				/*
				else
				displayNum(hr1, posX[0 + posOffset], 0, colors[0]);
				*/

				displayNum(hr2, posX[1 + posOffset], 0, colors[displayCol2], true);
				displayNum(mn1, posX[2 + posOffset], 1, colors[displayCol1], true);
				displayNum(mn2, posX[3 + posOffset], 1, colors[displayCol2], true);
				break;

			case 1:
				if (onHalfSecond())
				{
					displayNum(hr2, posX[1 + posOffset], 0, colors[displayCol2], true);
				}
				/*
				else
				{
				//Serial.println("off");
				displayNum(hr2, posX[1 + posOffset], 0, colors[0]);
				}
				*/

				displayNum(hr1, posX[0 + posOffset], 0, colors[displayCol1], true);
				displayNum(mn1, posX[2 + posOffset], 1, colors[displayCol1], true);
				displayNum(mn2, posX[3 + posOffset], 1, colors[displayCol2], true);
				break;

			case 2:
				if (onHalfSecond())
					displayNum(mn1, posX[2 + posOffset], 1, colors[displayCol1], true);

				displayNum(hr2, posX[1 + posOffset], 0, colors[displayCol2], true);
				displayNum(hr1, posX[0 + posOffset], 0, colors[displayCol1], true);
				displayNum(mn2, posX[3 + posOffset], 1, colors[displayCol2], true);
				break;

			case 3:
				if (onHalfSecond())
					displayNum(mn2, posX[3 + posOffset], 1, colors[displayCol2], true);

				displayNum(hr1, posX[0 + posOffset], 0, colors[displayCol1], true);
				displayNum(hr2, posX[1 + posOffset], 0, colors[displayCol2], true);
				displayNum(mn1, posX[2 + posOffset], 1, colors[displayCol1], true);
				break;
			}
		}
	}
}

void UpdateTime()
{
	hr = hour();
	mn = minute();
	sec = second();

	if (!HourMode24 && hr > 12)
	{
		hr -= 12;
	}

	if (!HourMode24 && hr == 0)
	{
		hr = 12;
	}

	if (hr < 10)
	{
		hrDisplay1 = 0;
		hrDisplay2 = hr;
	}
	else
	{
		hrDisplay1 = hr % 100 / 10;
		hrDisplay2 = hr % 10;
	}

	if (mn < 10)
	{
		mnDisplay1 = 0;
		mnDisplay2 = mn;
	}
	else
	{
		mnDisplay1 = mn % 100 / 10;
		mnDisplay2 = mn % 10;
	}
}

int tempNumToSet;
void TimeSet()
{
	if (buttonPressed())
	{
		pos++;
		canPress = false;
	}
	delay(100);

	//tempNumToSet = ReadRotary(tempNumToSet);
	/*
	if (ReadRotary() == 1)
	tempNumToSet += 1;

	if (ReadRotary() == 2)
	tempNumToSet -= 1;
	*/

	//Serial.println(val);
	switch (pos)
	{
	case 0:
		//1,0
		if (true)
		{
			int a = 2;
			int b = 0;

			tempNumToSet = ReadRotary(tempNumToSet, b, a);
			numToSet = tempNumToSet;
		}
		break;

	case 1:
		if (time[0] == 2)
		{
			int a = 4;
			int b = 0;

			tempNumToSet = ReadRotary(tempNumToSet, b, a);
			numToSet = tempNumToSet;
		}
		else
		{
			int a = 9;
			int b = 0;

			tempNumToSet = ReadRotary(tempNumToSet, b, a);
			numToSet = tempNumToSet;
		}
		break;

	case 2:
		if (true)
		{
			int a = 5;
			int b = 0;

			tempNumToSet = ReadRotary(tempNumToSet, b, a);
			numToSet = tempNumToSet;
		}
		break;

	case 3:
		if (true)
		{
			int a = 9;
			int b = 0;

			tempNumToSet = ReadRotary(tempNumToSet, b, a);
			numToSet = tempNumToSet;
		}
		break;
	}

	time[pos] = numToSet;

	DisplayCurrentTime(time[0], time[1], time[2], time[3], true, pos);

	if (pos >= 4)
	{
		pos = 0;
		String hrr1 = String(time[0]);
		String hrr2 = String(time[1]);
		String mnn1 = String(time[2]);
		String mnn2 = String(time[3]);

		String Newhr = String(hrr1 + hrr2);
		String Newmn = String(mnn1 + mnn2);

		Serial.println(Newhr);

		/*rtcTime.hour = Newhr.toInt();
		rtcTime.minute = Newmn.toInt();
		rtcTime.second = 0;
		rtc.writeTime(&rtcTime);*/

		setTime(
			Newhr.toInt(),
			Newmn.toInt(),
			0,
			0,
			0,
			2015
			);		

		Teensy3Clock.set(now());

		if (hour() > 7 && hour() < 20)
		{
			unRedColours();
		}

		//unRedColours();
		//targetBrightness = daylevel;

		State = DisplayTime;
		matrix.fillScreen(0);
		delay(100);
	}
}

void minuteAlert(int col,int xmod)
{
	//Serial.println(sec % 2);
	if (!(sec % 2))
	{
		matrix.drawPixel(0 + xmod, 0, colors[col]);
	}

	if (sec == 0)
	{
		matrix.drawPixel(0 + xmod, 0, matrix.Color(0, 255, 200));
	}
}

//Teensy time keeping stuff
time_t getTeensy3Time()
{
	return Teensy3Clock.get();
}

void digitalClockDisplay() {
	// digital clock display of the time
	Serial.print(hour());
	printDigits(minute());
	printDigits(second());
	Serial.print(" ");
	Serial.print(day());
	Serial.print(" ");
	Serial.print(month());
	Serial.print(" ");
	Serial.print(year());
	Serial.println();
}

void printDigits(int digits) {
	// utility function for digital clock display: prints preceding colon and leading 0
	Serial.print(":");
	if (digits < 10)
		Serial.print('0');
	Serial.print(digits);
}