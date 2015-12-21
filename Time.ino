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
	if (hr1 == 0)
	{
		hourBelow10 = true;
	}
	else
		hourBelow10 = false;

	//Night Mode
	if ((hour() > 21 || hour() == 0) && (State == Main || State == Brightness || State == DisplayTest))
	{
		//Drawing the preset night mode time

		//"1"
		if (hr2 != 2)
			matrix.drawLine(0 + xmod, 0, 0 + xmod, 2, Red);
		else
			matrix.drawLine(0 + xmod, 0, 0 + xmod, 1, Red);
		//1,2 or 3
		switch (hr2)
		{
		case 0:
			matrix.drawRect(2 + xmod, 0, 2, 3, Red);
			break;

		case 1:
			matrix.drawLine(2 + xmod, 0, 2 + xmod, 2, Red);
			break;

		case 2:
			matrix.drawLine(2 + xmod, 0, 3 + xmod, 0, Red);
			matrix.drawLine(3 + xmod, 1, 4 + xmod, 1, Red);
			break;
		}

		if (!hourBelow10)
		{
			if (mn2 != 1)	//Normal
			{
				displayNum(mn1, 3 + xmod, 3, Red, false);
				displayNum(mn2, 6 + xmod, 3, Red, false);
			}
			else	//Align
			{
				displayNum(mn1, 4 + xmod, 3, Red, false);
				displayNum(mn2, 6 + xmod, 3, Red, false);
			}
		}
	}
	else if (colors[1] == matrix.Color(255, 0, 0) && hour() > 0) //If display is redded
	{
		//displayNum(hr1, 1, 0, colors[displayCol1]);
		displayNum(hr2, 0 + xmod, 0, colors[displayCol2], true);
		displayNum(mn1, 3 + xmod, 0, colors[displayCol1], true);
		displayNum(mn2, 6 + xmod, 0, colors[displayCol2], true);
		Serial.println("ayyyyy");
	}
	else
	{
		/*
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
		*/

		if (hourBelow10)
		{
			minuteAlert(displayCol1, xmod);

			numConvey2.Update(hr2, 1 + conveyorBelt, 60, colors[displayCol2]);
			numConvey3.Update(mn1, 4 + conveyorBelt, 60, colors[displayCol1]);
			numConvey4.Update(mn2, 6 + conveyorBelt, 50, colors[displayCol2]);

			Serial.println("1");
		}
		else
		{
			numConvey1.Update(hr1, 0 + conveyorBelt, 100, colors[displayCol1]);
			numConvey2.Update(hr2, 2 + conveyorBelt, 75, colors[displayCol2]);
			numConvey3.Update(mn1, 4 + conveyorBelt, 50, colors[displayCol1]);
			numConvey4.Update(mn2, 6 + conveyorBelt, 50, colors[displayCol2]);

			if (State == Brightness)
			{
				displayNum(hr1, 0, 0, colors[displayCol1], true);
				displayNum(hr2, 2, 0, colors[displayCol2], true);
				displayNum(mn1, 4, 0, colors[displayCol1], true);
				displayNum(mn2, 6, 0, colors[displayCol2], true);
			}
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

	if (hour() > 21 && State == Main)
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
				displayNum(mn1, posX[2 + posOffset], 0, colors[displayCol1], true);
				displayNum(mn2, posX[3 + posOffset], 0, colors[displayCol2], true);
			}
			else
			{
				displayNum(hr1, posX[0 + posOffset], 0, colors[displayCol1], true);
				displayNum(hr2, posX[1 + posOffset], 0, colors[displayCol2], true);
				displayNum(mn1, posX[2 + posOffset], 0, colors[displayCol1], true);
				displayNum(mn2, posX[3 + posOffset], 0, colors[displayCol2], true);
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
				displayNum(mn1, posX[2 + posOffset], 0, colors[displayCol1], true);
				displayNum(mn2, posX[3 + posOffset], 0, colors[displayCol2], true);
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
				displayNum(mn1, posX[2 + posOffset], 0, colors[displayCol1], true);
				displayNum(mn2, posX[3 + posOffset], 0, colors[displayCol2], true);
				break;

			case 2:
				if (onHalfSecond())
					displayNum(mn1, posX[2 + posOffset], 1, colors[displayCol1], true);

				displayNum(hr2, posX[1 + posOffset], 0, colors[displayCol2], true);
				displayNum(hr1, posX[0 + posOffset], 0, colors[displayCol1], true);
				displayNum(mn2, posX[3 + posOffset], 0, colors[displayCol2], true);
				break;

			case 3:
				if (onHalfSecond())
					displayNum(mn2, posX[3 + posOffset], 1, colors[displayCol2], true);

				displayNum(hr1, posX[0 + posOffset], 0, colors[displayCol1], true);
				displayNum(hr2, posX[1 + posOffset], 0, colors[displayCol2], true);
				displayNum(mn1, posX[2 + posOffset], 0, colors[displayCol1], true);
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
	HrMn NewTime;
	NewTime = TimeSetReturn();

	setTime(
		NewTime.Hr,
		NewTime.Mn,
		0,
		0,
		0,
		2015
		);

	Teensy3Clock.set(now());

	if (hour() > 7 && hour() < 20)
		unRedColours();
	
	oke();
}

HrMn TimeSetReturn()
{
	int b = 0;

	if (buttonPressed())
	{
		pos++;
		canPress = false;
	}

	switch (pos)
	{
	case 0:
	{
		int a = 2;
		int b = 0;

		if (numToSet >= b && numToSet <= a)
			numToSet = ReadRotary(numToSet);
		else
			numToSet = force(numToSet, b, a);
	}
	break;

	case 1:
		if (time[0] == 2)
		{
			int a = 4;
			int b = 0;

			if (numToSet >= b && numToSet <= a)
				numToSet = ReadRotary(numToSet);
			else
				numToSet = force(numToSet, b, a);
		}
		else
		{
			int a = 9;
			int b = 0;

			if (numToSet >= b && numToSet <= a)
				numToSet = ReadRotary(numToSet);
			else
				numToSet = force(numToSet, b, a);
		}
		break;

	case 2:
	{
		int a = 5;
		int b = 0;

		if (numToSet >= b && numToSet <= a)
			numToSet = ReadRotary(numToSet);
		else
			numToSet = force(numToSet, b, a);
	}
	break;

	case 3:
	{
		int a = 9;
		int b = 0;

		if (numToSet >= b && numToSet <= a)
			numToSet = ReadRotary(numToSet);
		else
			numToSet = force(numToSet, b, a);
	}
	break;
	}

	time[pos] = numToSet;

	//DisplayCurrentTime(time[0], time[1], time[2], time[3], true, pos);

	setConvey1.Update(time[0], 0, 200, colors[displayCol1]);
	setConvey2.Update(time[1], 2, 250, colors[displayCol2]);
	setConvey3.Update(time[2], 4, 300, colors[displayCol1]);
	setConvey4.Update(time[3], 6, 400, colors[displayCol2]);

	if (pos >= 4)
	{
		for (int i = 0; i <= 4; i++)
		{
			time[i] = 0;
		}

		pos = 0;
		HrMn r;

		String hrr1 = String(time[0]);
		String hrr2 = String(time[1]);
		String mnn1 = String(time[2]);
		String mnn2 = String(time[3]);

		String Newhr = String(hrr1 + hrr2);
		String Newmn = String(mnn1 + mnn2);

		r.Hr = Newhr.toInt();
		r.Mn = Newmn.toInt();

		return r;
	}
}

void minuteAlert(int col, int xmod)
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

//Teensy RTC Stuff
time_t getTeensy3Time()
{
	return Teensy3Clock.get();
}

//Serial Stuff
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