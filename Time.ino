void DisplayTime(int hr1, int hr2, int mn1, int mn2, int xmod)
{
	displayTimeInner(hr1, hr2, mn1, mn2, xmod);
}

void DisplayTime(HrMn time)
{
	int hr1, hr2, mn1, mn2, xmod;
	hr1 = time.Hr / 10 % 10;
	hr2 = time.Hr % 10;
	
	mn1 = time.Mn / 10 % 10;
	mn2 = time.Mn % 10;
	
	displayTimeInner(hr1, hr2, mn1, mn2, xmod);
}

void displayTimeInner(int hr1, int hr2, int mn1, int mn2, int xmod)
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
				displayNum(mn1, 3 + xmod, 3, Red, Small);
				displayNum(mn2, 6 + xmod, 3, Red, Small);
			}
			else	//Align
			{
				displayNum(mn1, 4 + xmod, 3, Red, Small);
				displayNum(mn2, 6 + xmod, 3, Red, Small);
			}
		}
	}
	else if (colors[1] == matrix.Color(255, 0, 0) && hour() > 0) //If display is redded
	{
		//displayNum(hr1, 1, 0, colors[displayCol1]);
		displayNum(hr2, 0 + xmod, 0, colors[displayCol2], Big);
		displayNum(mn1, 3 + xmod, 0, colors[displayCol1], Big);
		displayNum(mn2, 6 + xmod, 0, colors[displayCol2], Big);
	}
	else
	{
		if (hourBelow10)
		{
			minuteAlert(displayCol1, xmod);

			numConvey2.Update(hr2, 1 + conveyorBelt, 60, colors[displayCol2],Big);
			numConvey3.Update(mn1, 4 + conveyorBelt, 60, colors[displayCol1],Big);
			numConvey4.Update(mn2, 6 + conveyorBelt, 50, colors[displayCol2],Big);			
		}
		else
		{
			numConvey1.Update(hr1, 0 + conveyorBelt, 10, colors[displayCol1], Big);
			numConvey2.Update(hr2, 2 + conveyorBelt, 75, colors[displayCol2], Big);
			numConvey3.Update(mn1, 4 + conveyorBelt, 50, colors[displayCol1], Big);
			numConvey4.Update(mn2, 6 + conveyorBelt, 50, colors[displayCol2], Big);
		}
	}
}

void displayTimeSimple(HrMn time, efontSize size, bool scroll)
{
	int hr1, hr2, mn1, mn2;

	hr1 = time.Hr / 10 % 10;
	hr2 = time.Hr % 10;

	mn1 = time.Mn / 10 % 10;
	mn2 = time.Mn % 10;

	if (scroll)
	{
		numConvey1.Update(hr1, 0 + conveyorBelt, 10, colors[displayCol1], size);
		numConvey2.Update(hr2, 2 + conveyorBelt, 75, colors[displayCol2], size);
		numConvey3.Update(mn1, 4 + conveyorBelt, 50, colors[displayCol1], size);
		numConvey4.Update(mn2, 6 + conveyorBelt, 50, colors[displayCol2], size);
	}
	else
	{
		displayNum(hr2, 2, 0, colors[displayCol2], size);
		displayNum(hr1, 0, 0, colors[displayCol1], size);
		displayNum(mn1, 4, 0, colors[displayCol1], size);
		displayNum(mn2, 6, 0, colors[displayCol2], size);
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
	NewTime = TimeSetReturn(false);

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

HrMn TimeSetReturn(bool justMins)
{
	while (1 < 2)
	{
		matrix.fillScreen(0);

		if (buttonPressed())
			NumSetPos++;

		switch (NumSetPos)
		{
		case 0:
		{
			int a = 2;
			int b = 0;

			if (!justMins)
				a = 2;
			else
				a = 6;

			if (numToSet >= b && numToSet <= a)
				numToSet = ReadRotary(numToSet);
			else
				numToSet = force(numToSet, b, a);
		}
		break;

		case 1:
		{
			int a = 4;
			int b = 0;

			if (time[0] == 2)
				a = 4;
			else
				a = 9;

			if (justMins)
				a = 9;

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

		time[NumSetPos] = numToSet;

		if (justMins)
		{
			setConvey3.Update(time[0], 2, 400, colors[displayCol1], Big);
			setConvey4.Update(time[1], 4, 400, colors[displayCol2], Big);
		}
		else
		{
			setConvey1.Update(time[0], 0, 400, colors[displayCol1], Big);
			setConvey2.Update(time[1], 2, 400, colors[displayCol2], Big);
			setConvey3.Update(time[2], 4, 400, colors[displayCol1], Big);
			setConvey4.Update(time[3], 6, 400, colors[displayCol2], Big);
		}

		if (NumSetPos >= 4 && !justMins)
		{
			//Todo reset conveyor target

			NumSetPos = 0;
			HrMn r;

			String hrr1 = String(time[0]);
			String hrr2 = String(time[1]);
			String mnn1 = String(time[2]);
			String mnn2 = String(time[3]);

			String Newhr = String(hrr1 + hrr2);
			String Newmn = String(mnn1 + mnn2);

			r.Hr = Newhr.toInt();
			r.Mn = Newmn.toInt();

			for (int i = 0; i <= 4; i++)
				time[i] = 0;

			return r;
		}

		if (NumSetPos >= 2 && justMins)
		{
			//Todo reset conveyor target

			NumSetPos = 0;
			HrMn r;

			String m1 = String(time[0]);
			String m2 = String(time[1]);

			String Newmn = String(m1 + m2);

			r.Mn = Newmn.toInt();

			for (int i = 0; i <= 4; i++)
				time[i] = 0;

			return r;
		}

		matrix.show();
		if (digitalRead(2) == LOW)
			canPress = true;
	}
}

void minuteAlert(int col, int xmod)
{
	if (!(sec % 2))
		matrix.drawPixel(0 + xmod, 0, colors[col]);
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

void menuTime()
{
	DisplayTime(hrDisplay1, hrDisplay2, mnDisplay1, mnDisplay2, conveyorBelt);
}