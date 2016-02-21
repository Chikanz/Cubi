//Alarm shit
int timesPlayed;

void PlayAlarm()
{
	digitalWrite(3, HIGH);

	/*matrix.setCursor(textCursor, 0);*/
	//if (--textCursor == -50)
	//	textCursor = matrix.width();
	//matrix.print(("Wake Up!"));
	
	//mixer2.update

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
	
	//matrix.setTextColor(colors[displayCol1]);

	delay(100);

	if (buttonPressed())
	{
		digitalWrite(3, LOW);
		oke(2);
		playWav1.stop();
	}
}

int alarmMenuPage = 0;
void alarmMenu()
{
	alarmMenuPage = ReadRotary(alarmMenuPage, 0, 1);

	if (AlarmKill)
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

void AlarmPageChange()
{
	switch (alarmMenuPage)
	{
		case 0:
			AlarmKill = true;
			EEPROM.write(5, true);
			oke(0);
			break;

		case 1:
			AlarmKill = false;
			EEPROM.write(5, true);
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
		displayTimeSimple(Alarms[cursorPos].time, Med, false, false);
	else if (!Alarms[cursorPos].active)
		DisplayDay(cursorPos,3 ,1, 2, colors[displayCol1], colors[displayCol2]);

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
	matrix.drawPixel(cursorPos, 7, matrix.Color(0, 0, 0));

	//Button press
	if (buttonPressed())
	{
		if (cursorPos == 7)
		{
			EEPROM.put(10 + sizeof(BProfile), Alarms);
			EEPROM.write(5, true);
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