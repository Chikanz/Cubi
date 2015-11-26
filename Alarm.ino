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
		State = DisplayTime;
		canPress = false;
		playWav1.stop();
	}
}
