
void DisplayCurrentTime(int hr1, int hr2, int mn1, int mn2, boolean Blink, int blinkpos)
{
	if (bigMode)
	{
		posOffset = 4;
		colpos2 = displayCol2;
		colpos3 = displayCol1;
		if (hr1 == 0)
		{
			hourBelow10 = true;
		}
		else
			hourBelow10 = false;
	}
	else
	{
		hourBelow10 = false;
		posOffset = 0;
		colpos2 = displayCol1;
		colpos3 = displayCol2;
	}

	if (!Blink)
	{
		if (hourBelow10 && bigMode)
		{
			//displayNum(hr1, 1, 0, colors[displayCol1]);
			displayNum(hr2, 1, 0, colors[colpos2]);
			displayNum(mn1, posX[2 + posOffset], 1, colors[colpos3]);
			displayNum(mn2, posX[3 + posOffset], 1, colors[displayCol2]);
		}
		else
		{
			displayNum(hr1, posX[0 + posOffset], 0, colors[displayCol1]);
			displayNum(hr2, posX[1 + posOffset], 0, colors[colpos2]);
			displayNum(mn1, posX[2 + posOffset], 1, colors[colpos3]);
			displayNum(mn2, posX[3 + posOffset], 1, colors[displayCol2]);
		}
	}
	else
	{
		switch (blinkpos)
		{
		case 0:
			if (onHalfSecond())
				displayNum(hr1, posX[0 + posOffset], 0, colors[displayCol1]);
			/*
			else
			displayNum(hr1, posX[0 + posOffset], 0, colors[0]);
			*/

			displayNum(hr2, posX[1 + posOffset], 0, colors[colpos2]);
			displayNum(mn1, posX[2 + posOffset], 1, colors[colpos3]);
			displayNum(mn2, posX[3 + posOffset], 1, colors[displayCol2]);
			break;

		case 1:
			if (onHalfSecond())
			{
				displayNum(hr2, posX[1 + posOffset], 0, colors[colpos2]);
			}
			/*
			else
			{
			//Serial.println("off");
			displayNum(hr2, posX[1 + posOffset], 0, colors[0]);
			}
			*/

			displayNum(hr1, posX[0 + posOffset], 0, colors[displayCol1]);
			displayNum(mn1, posX[2 + posOffset], 1, colors[colpos3]);
			displayNum(mn2, posX[3 + posOffset], 1, colors[displayCol2]);
			break;

		case 2:
			if (onHalfSecond())
				displayNum(mn1, posX[2 + posOffset], 1, colors[colpos3]);

			displayNum(hr2, posX[1 + posOffset], 0, colors[colpos2]);
			displayNum(hr1, posX[0 + posOffset], 0, colors[displayCol1]);
			displayNum(mn2, posX[3 + posOffset], 1, colors[displayCol2]);
			break;

		case 3:
			if (onHalfSecond())
				displayNum(mn2, posX[3 + posOffset], 1, colors[displayCol2]);

			displayNum(hr1, posX[0 + posOffset], 0, colors[displayCol1]);
			displayNum(hr2, posX[1 + posOffset], 0, colors[colpos2]);
			displayNum(mn1, posX[2 + posOffset], 1, colors[colpos3]);
			break;
		}
	}
}

void UpdateTime()
{
	rtc.readDate(&rtcDate);

	rtc.readTime(&rtcTime);
	hr = rtcTime.hour;
	realHr = rtcTime.hour;
	mn = rtcTime.minute;
	sec = rtcTime.second;

	/*
	Serial.println("Hr:"); Serial.println(hr);
	Serial.println("Mn:"); Serial.println(mn);
	Serial.println("Sec:"); Serial.println(sec);
	*/

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

		//Serial.println(Newhr.toInt());
		/*

		setTime(
		Newhr.toInt() //hr
		, Newmn.toInt() //mn
		, 0 //sec
		, 28
		, 2
		, 2015);
		*/
		Serial.println(Newhr);

		rtcTime.hour = Newhr.toInt();
		rtcTime.minute = Newmn.toInt();
		rtcTime.second = 0;
		rtc.writeTime(&rtcTime);

		unRedColours();
		targetBrightness = daylevel;

		State = DisplayTime;
		matrix.fillScreen(0);
	}
}

void minuteAlert(int col)
{
	//Serial.println(sec % 2);
	if (!(sec % 2))
	{
		matrix.drawPixel(0, 0, colors[col]);
	}

	if (sec == 0)
	{
		matrix.drawPixel(0, 0, matrix.Color(0, 255, 200));
	}
}

