//Alarm shit
int timesPlayed;

void PlayAlarm()
{
	targetBrightness = daylevel;
	unRedColours();

	digitalWrite(3, HIGH);
	UpdateTime();

	matrix.setCursor(textCursor, 0);
	if (--textCursor == -50)
		textCursor = matrix.width();
	matrix.print(("Wake Up!"));

	if (!playWav1.isPlaying())
	{
		EEPROM.write(6, songToPlay += 1);

		if (songToPlay > fileCount)
		{
			songToPlay = 0;
			EEPROM.write(6, 0);
			Serial.println("Song Playing: ");
			Serial.println(songToPlay);
		}

		Serial.print(songToPlay);

		playFile("(" + String(songToPlay) + ")" + ".wav");
	}
	matrix.setTextColor(colors[displayCol1]);

	delay(100);

	if (buttonPressed())
	{
		targetBrightness = daylevel;
		digitalWrite(3, LOW);
		matrix.fillScreen(0);
		State = Main;
		canPress = false;
		playWav1.stop();
	}
}

int alarmMenuPage = 0;
void alarmMenu()
{
	alarmMenuPage = ReadRotary(alarmMenuPage, 0, 1);

	if (AlarmisSet)
		matrix.drawPixel(7, 0, colors[displayCol2]);

	switch (alarmMenuPage)
	{
	case 0:
		moveNum = -3;
		speakerIcon(false);
		matrix.drawLine(0, 0, 7, 7, Red);
		break;

	case 1:
		speakerIcon(true);
		break;
	}
}

void AlarmPageChange()
{
	switch (alarmMenuPage)
	{
	case 0:
		AlarmisSet = false;
		EEPROM.write(5, false);
		oke();
		break;

	case 1:
		State = SetAlarm;
		AlarmisSet = true;
		break;
	}
}

void AlarmSet()
{
	HrMn alarm;
	alarm = TimeSetReturn(false);

	AlarmTime[0] = alarm.Hr;
	AlarmTime[1] = alarm.Mn;

	EEPROM.write(3, AlarmTime[0]);
	EEPROM.write(4, AlarmTime[1]);
	EEPROM.write(5, true);
	//AlarmisSet = true;
	//matrix.drawPixel (0, 0, Red);
	oke();
	matrix.fillScreen(0);
	
}

void perDayAlarm()
{
	//Per day alarm

	//Force the cursor pos and get input
	if (cursorPos >= 0 && cursorPos <= 7)
		cursorPos = ReadRotary(cursorPos, true);
	else
		cursorPos = constrain(cursorPos, 0, 7);

	//Button press
	if (buttonPressed())
	{
		if (cursorPos == 7)
		{
			EEPROM.put(10 + sizeof(BProfile), Alarms);
			EEPROM.write(5, true);
			cursorPos = 0;
			oke(true);
		}
		else
		{
			if (Alarms[cursorPos].active)
			{
				Alarms[cursorPos].active = false;
				Alarms[cursorPos].time = { 0,0 };
			}
			else
			{
				Alarms[cursorPos].active = true;
				Alarms[cursorPos].time = TimeSetReturn(false);
				oke(false);
			}
		}
	}

	//Display above selection
	if (cursorPos == 7)
		backIcon(0, 0, colors[displayCol2], false);
	else if (Alarms[cursorPos].active)
		displayTimeSimple(Alarms[cursorPos].time, Med, false, false);
	else if (!Alarms[cursorPos].active)
		switch (cursorPos) //TODO: replace with something scrolly someday
		{
		case 0:
			sunday(1, 2, colors[displayCol1], colors[displayCol2]);
			break;
		case 1:
			monday(1, 2, colors[displayCol1], colors[displayCol2]);
			break;
		case 2:
			tuesday(1, 2, colors[displayCol1], colors[displayCol2]);
			break;
		case 3:
			wednesday(1, 2, colors[displayCol1], colors[displayCol2]);
			break;
		case 4:
			thursday(1, 2, colors[displayCol1], colors[displayCol2]);
			break;
		case 5:
			friday(1, 2, colors[displayCol1], colors[displayCol2]);
			break;
		case 6:
			saturday(1, 2, colors[displayCol1], colors[displayCol2]);
			break;
		}

	//Draw Pixels
	for (int i = 0; i < 7; i++)
	{
		if (i == cursorPos)
		{
			if (Alarms[i].active)
				matrix.drawPixel(i, 7, colors[1] - fadeMod);
			else
				matrix.drawPixel(i, 7, colors[6] - fadeMod);
		}
		else
		{
			if (Alarms[i].active)
				matrix.drawPixel(i, 7, colors[1]);
			else
				matrix.drawPixel(i, 7, colors[6]);
		}
	}
	matrix.drawPixel(7, 7, colors[displayCol1]);

	//Create a blink effect
	//if (onHalfSecond())
	//matrix.drawPixel(cursorPos, 7, matrix.Color(0, 0, 0));
	cursorFadeTimer += 51;

	if (cursorFadeTimer > 100)
	{
		if (fadeDirection)
			fadeMod += 2;
		else
			fadeMod -= 1;

		cursorFadeTimer = 0;
	}

	//if (fadeMod <= 100)
	//	fadeDirection = true;

	if (fadeMod >= 120)
		fadeMod = 100;
	//fadeDirection = false;

	Serial.println(fadeMod);

	delay(50);
}