//Alarm shit
int timesPlayed;

void PlayAlarm()
{
	display = true;
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
void AlarmMenu()
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
		State = Main;
		matrix.fillScreen(0);
		break;

	case 1:
		Serial.println("Setting Alarm");
		State = SetAlarm;
		AlarmisSet = true;
		matrix.fillScreen(0);
		break;
	}
}

void AlarmSet()
{
	HrMn alarm;
	alarm = TimeSetReturn();

	AlarmTime[0] = alarm.Hr;
	AlarmTime[1] = alarm.Mn;

	EEPROM.write(3, AlarmTime[0]);
	EEPROM.write(4, AlarmTime[1]);
	EEPROM.write(5, true);

	oke();
}
