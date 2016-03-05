//Alarm shit
int timesPlayed;

void PlayAlarm()
{
	menuTime(7);
	digitalWrite(3, HIGH);	

	if (!playWav1.isPlaying())
	{		
		if (songToPlay > fileCount)
		{
			songToPlay = 1;
			EEPROM.write(6, 1);
			Serial.println("Reset Song Count");
		}

		Serial.print("Playing Song: ");
		Serial.println(songToPlay);

		playFile("(" + String(songToPlay) + ")" + ".wav");
	}

	delay(100);

	if (buttonPressed())
	{
		volume = 0;
		songToPlay += 1;
		EEPROM.write(6, songToPlay);
		digitalWrite(3, LOW);
		oke(2);
		playWav1.stop();                
	}
}

int alarmMenuPage = 0;
void alarmMenu()
{
	alarmMenuPage = ReadRotary(alarmMenuPage, 0, 1);

	if (!AlarmKill)
		matrix.drawPixel(7, 0, colors[displayCol2]);

	switch (alarmMenuPage)
	{
		case 0:
			moveNum = -3;
			speakerIcon(false, true);
			matrix.drawLine(0, 0, 7, 7, Red);
			break;

		case 1:
			speakerIcon(true, true);
			break;
	}
}

void AlarmPageChange() //fix dis
{
	switch (alarmMenuPage)
	{
		case 0:
			AlarmKill = true;
			EEPROM.write(5, true);
			alarmMenuPage = 0;
			oke(0);
			break;

		case 1:
			EEPROM.write(5, false);
			alarmMenuPage = 0;
			State = SetAlarm;
			break;
	}
}

HrMn prevTime;
void perDayAlarm()
{
	cursorPos = ReadRotary(cursorPos, true);
	cursorPos = constrain(cursorPos, 0, 7);

	//Display above selection
	if (cursorPos == 7)
		backIcon(0, 0, colors[displayCol2], false);
	else if (Alarms[cursorPos].active)
		displayTimeSimple(Alarms[cursorPos].time, Med, false, true);
	else if (!Alarms[cursorPos].active)
		DisplayDay(cursorPos,0,1,1,colors[displayCol1], colors[displayCol2],Med);

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

	//cursor
	matrix.drawPixel(cursorPos, 7, matrix.Color(0, 0, 0));

	//blink current day
	if (onSecond())
		matrix.drawPixel(weekday() - 1, 7, matrix.Color(0, 0, 0));

	//Button press
	if (buttonPressed())
	{
		if (cursorPos == 7)
		{
			EEPROM.put(10 + sizeof(BProfile), Alarms);

			for (int i = 0; i < 6; i++)
			{
				if (Alarms[i].active)
				{
					EEPROM.write(5, false);
					AlarmKill = false;
					i = 7;
				}
			}

			cursorPos = 0;
			oke(0);
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
				Alarms[cursorPos].time = TimeSetReturn(false, prevTime);
				prevTime = Alarms[cursorPos].time;
				oke(1);
			}
		}
	}

	delay(50);
}