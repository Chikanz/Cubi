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
		hourBelow10 = true;
	else
		hourBelow10 = false;

	//Night Mode
	if (brightnessLevel == 6)
	{
		//Drawing the preset night mode time
		if (hour() >= 20 || hour() == 0)
		{
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
		else
		{
			//displayNum(hr1, 1, 0, colors[displayCol1]);
			displayNum(hr2, 0 + xmod, 0, colors[displayCol2], Big);
			displayNum(mn1, 3 + xmod, 0, colors[displayCol1], Big);
			displayNum(mn2, 6 + xmod, 0, colors[displayCol2], Big);
		}
	} //Night
	else//Normals
	{
		if (hourBelow10)
		{
			minuteAlert(displayCol1, xmod);

			numConvey2.Update(hr2, 1 + conveyorBelt, 60, colors[displayCol2], Big);
			numConvey3.Update(mn1, 4 + conveyorBelt, 60, colors[displayCol1], Big);
			numConvey4.Update(mn2, 6 + conveyorBelt, 50, colors[displayCol2], Big);
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

void displayTimeSimple(HrMn time, efontSize size, bool scroll, bool hr24)
{
	if (hour() > 12 && hr24)
		time.Hr -= 12;

	int hr1, hr2, mn1, mn2;

	hr1 = time.Hr / 10 % 10;
	hr2 = time.Hr % 10;

	mn1 = time.Mn / 10 % 10;
	mn2 = time.Mn % 10;

	if (scroll)
	{
		numConvey1.Update(hr1, 0, 10, colors[displayCol1], size);
		numConvey2.Update(hr2, 2, 75, colors[displayCol2], size);
		numConvey3.Update(mn1, 4, 50, colors[displayCol1], size);
		numConvey4.Update(mn2, 6, 50, colors[displayCol2], size);
	}
	else
	{
		if (hr1 != 0)
		{
			displayNum(hr1, 0, 0, colors[displayCol1], size);
			displayNum(hr2, 2, 0, colors[displayCol2], size);
			displayNum(mn1, 4, 0, colors[displayCol1], size);
			displayNum(mn2, 6, 0, colors[displayCol2], size);
		}
		else
		{
			displayNum(hr2, 1, 0, colors[displayCol2], size);
			displayNum(mn1, 4, 0, colors[displayCol1], size);
			displayNum(mn2, 6, 0, colors[displayCol2], size);
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

HrMnSplit split(HrMn t)
{
	HrMnSplit r;
	r.hr1 = t.Hr / 10 % 10;
	r.hr2 = t.Hr % 10;

	r.mn1 = t.Mn / 10 % 10;
	r.mn2 = t.Mn % 10;

	return r;
}

int tempNumToSet;
void TimeSet()
{
	HrMn t;
	Date d;
	d = DateReturn();
	t = TimeSetReturn(false, { 0, 0 });

	

	setTime(
		t.Hr,
		t.Mn,
		0,
		d.day,
		d.month,
		d.year
		);

	RTC.set(now());
	oke();
}

HrMn TimeSetReturn(bool justMins, HrMn set)
{
	int flashTimer;

	int numToSet;
	int NumSetPos;

	int a; //Max
	int b; //Min
	int timeToSet[4];

	HrMnSplit s = split(set);

	time[0] = s.hr1;
	time[1] = s.hr2;
	time[2] = s.mn1;
	time[3] = s.mn2;

	for (int i = 0; i < 4; i++)
		timeToSet[i] = time[i];

	numToSet = timeToSet[NumSetPos];
	
	while (1 < 2)
	{
		matrix.fillScreen(0);

		switch (NumSetPos)
		{
			case 0:
			{
				a = 2;
				b = 0;

				if (!justMins)
					a = 2;
				else
					a = 6;
			}
			break;

			case 1:
			{
				a = 4;
				b = 0;

				if (time[0] == 2)
					a = 4;
				else
					a = 9;

				if (justMins)
					a = 9;
			}
			break;

			case 2:
			{
				a = 5;
				b = 0;
			}
			break;

			case 3:
			{
				a = 9;
				b = 0;
			}
			break;
		}

		numToSet = ReadRotary(numToSet, false);
		numToSet = constrain(numToSet, b, a);

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

		if (buttonPressed())
		{
			numToSet = timeToSet[NumSetPos + 1];
			NumSetPos++;
			flashTimer = 700;
		}

		//Flash
		if (flashTimer > 0)
		{
			matrix.drawRect((NumSetPos - 1) * 2 , 0, 2, 8, matrix.Color(0, 0, 0));
			flashTimer -= 51;
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

Date DateReturn()
{
	int NumSetPos = 0;
	int numToSet = 0;
	Date r;

	HrMn year; //THIS MAKES SENSE I DO THE GOOD CODE
	year.Hr = 20;
	year.Mn = 16;
	int yr1 = 1, yr2 = 6; 
	int day1, day2;
	int dayMax, day1Max, day2Max;

	while (1 < 2)
	{
		SetLight();
		matrix.fillScreen(0);

		if (buttonPressed())
		{
			numToSet = 0;
			NumSetPos++;

			//One time events
			switch (NumSetPos)
			{
				case 3:
					dayMax = calcDays(r.month, 2016); //?
					Serial.println(dayMax);
					day1Max = dayMax / 10 % 10;
					day2Max = dayMax % 10;
					break;
			}
		}

		switch (NumSetPos)
		{
			case 0:
			{
				//Year1
				yr1 = ReadRotary(yr1, false);
				yr1 = constrain(yr1, 1, 9);

				year.Mn = (yr1 * 10) + yr2; //Combine
				displayTimeSimple(year, Med, false, false);
			}
			break;

			case 1:
			{
				//Year2
				if (yr2 >= 0 && yr2 <= 9)
					yr2 = ReadRotary(yr2, false);
				else
					yr2 = constrain(yr2, 0, 9);

				year.Mn = (yr1 * 10) + yr2; //Combine
				displayTimeSimple(year, Med, false, false);
			}
			break;

			case 2:
			{
				//Month
				numToSet = ReadRotary(numToSet, false);
				numToSet = constrain(numToSet, 0, 11);

				r.month = numToSet;
				displayMonth(r.month);

				//Faff
				displayNum(0, 0, 3, colors[displayCol2], Small);
				displayNum(0, 3, 3, colors[displayCol2], Small);
			}
			break;

			case 3:
			{
				//Day1
				numToSet = ReadRotary(numToSet, false);
				numToSet = constrain(numToSet, 0, day1Max);

				day1 = numToSet;

				displayNum(numToSet, 0, 3, colors[displayCol2], Small);
				displayNum(0, 3, 3, colors[displayCol2], Small);

				//Faff
				displayMonth(r.month);
			}
			break;

			case 4:
			{
				//Day2
				int m;

				if (day1 == day1Max)
					m = day2Max;
				else
					m = 9;

				numToSet = ReadRotary(numToSet, false);
				numToSet = constrain(numToSet, 0, m);

				day2 = numToSet;

				displayNum(day1, 0, 3, colors[displayCol2], Small);
				displayNum(numToSet, 3, 3, colors[displayCol2], Small);

				//Faff
				displayMonth(r.month);
			}
			break;
		}

		if (NumSetPos >= 5)
		{
			r.year = year.Mn;
			//r.month
			r.day = day1 * 10 + day2;

			NumSetPos = 0;
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

//Thanks to Collin Biedenkapp from Stack overflow
int calcDays(int month, int year) {
	if (month == 0 || month == 2 || month == 4 || month == 6 || month == 7 || month == 9 || month == 11)
		return 31;
	else if (month == 3 || month == 5 || month == 8 || month == 10)
		return 30;
	else {
		if (year % 4 == 0) {
			if (year % 100 == 0) {
				if (year % 400 == 0)
					return 29;
				return 28;
			}
			return 29;
		}
		return 28;
	}
}