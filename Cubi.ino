//Cubi, a playful LED powered alarm clock.
//Zac Lucarelli 2k15

/*
EEPROM Memory allocation
0 - null
1 - DisplayCol1
2 - DisplayCol2
3 - AlarmTime1
4 - AlarmTime2
5 - AlarmIsSet
*/

//So many libraries the Citadel is jelly
#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>

#include <EEPROM.h>

#include <SoftI2C.h>
#include <DS3232RTC.h>

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>

#include <LinkedList.h>

#define ENCODER_OPTIMIZE_INTERRUPTS
#include <Encoder.h>
//#include <avr/power.h>

// Audio stuff
AudioPlaySdWav           playWav1;       //xy=154,78
AudioOutputI2S           i2s1;           //xy=334,89
AudioConnection          patchCord1(playWav1, 0, i2s1, 0);
AudioConnection          patchCord2(playWav1, 1, i2s1, 1);
AudioControlSGTL5000     sgtl5000_1;     //xy=240,153

										 //Declcare instances
										 //RTC stuff
SoftI2C i2c(A3, A2);
DS3232RTC rtc(i2c);
RTCTime rtcTime;
RTCDate rtcDate;

Encoder rotary(5, 4);  //Rotary Encoder

					   //Definitions
#define PIN 6

#define Green matrix.Color(0,255,0)
#define LightGreen matrix.Color(100,255,100)
#define Yellow matrix.Color(255,255,0)
#define LightOrange matrix.Color(255,200,0)
#define Orange matrix.Color(255,100,0)
#define Red matrix.Color(255,50,50)
#define Purple matrix.Color(255,0,255)
#define Cyan matrix.Color(0,255,255)
#define White matrix.Color(255,255,255)

#define Blue matrix.Color(0,150,255)

Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(8, 8, PIN,
	NEO_MATRIX_TOP + NEO_MATRIX_LEFT +
	+NEO_MATRIX_PROGRESSIVE,
	NEO_GRB + NEO_KHZ800);

//Default display colours
int displayCol1 = 9;
int displayCol2 = 3;

uint16_t colors[] =
{
	matrix.Color(0, 0, 0),
	Green,
	LightGreen,
	Yellow,
	LightOrange,
	Orange,
	Red,
	Purple,
	Blue,
	Cyan,
};

uint16_t colorsStore[] =
{
	matrix.Color(0, 0, 0),
	Green,
	LightGreen,
	Yellow,
	LightOrange,
	Orange,
	Red,
	Purple,
	Blue,
	Cyan,
};

int x = matrix.width();
int pass = 0;

int xx = 0;
int yy = 0;
int Minute1s = 0;
int Minute10s = 0;

boolean blinkk = true;
int blinkCount = 0;
int wheel = 0;

int red, green, blue;
int iteration;

int hr, mn;
int sec;

long oldPosition = -999;

int hrDisplay1, hrDisplay2, mnDisplay1, mnDisplay2;

#define melodyPin 3

int temphrDisplay1, temphrDisplay2, tempmnDisplay1, tempmnDisplay2;

boolean HourMode24 = false;

//TimeSet Stuff
int selectedNum = 0;
int val;

int hr1temp;
int hr2temp;

int mn1temp;
int mn2temp;

int numToSet;
int pos = 0;

boolean canPress = true;

bool AlarmisSet = false;
int AlarmTime[] = { 0, 0 };

int time[] = { hr1temp, hr2temp, mn1temp, mn2temp };

int realHr;

bool bigMode = true;

int ColorToChoose = 1;

int pirranaPos = 46;

enum eState
{
	TimeSetMode,	// 1
	DisplayTime,	// 2
	Menu,			// 3
	SetAlarm,		// 4
	MenuAlarm,		// 5
	Alarm,			// 6
	ChangeColor,	// 7
	DisplayTest,	// 8
	Snake,			// 9
	StaticDisplay,	//10
};

//
///
////
eState State = DisplayTime;
////
///
//

int textCursor = 8;
int menuPage = 0;
long Rpos = -999;
int songToPlay;
int moveNum = 3;

int targetBrightness = 17;
int currentBrightness = 0;

bool displayoff = true;
int nightLevel = 0;
int daylevel = 17;
int displayOnOff = 0;
bool dimTrigger = false;
int dimtime = 0;

//Snake
LinkedList<int> snakeBodyX = LinkedList<int>();
LinkedList<int> snakeBodyY = LinkedList<int>();

enum eDirection { Up, Right, Down, Left };
eDirection dir = Right;
LinkedList<eDirection> dirList = LinkedList<eDirection>();

int inputDirMod;
int inputDirIndex;

long tempTimer4 = 0;

int snakeLength = 2;
int foodx;
int foody;
bool ate = true;
bool inBody = false;
int score = -1;

bool gameOverloop = true;

int headx;
int heady;

int timerTemp;

bool mouth;
///

void setup()
{
	matrix.begin();
	matrix.setTextWrap(false);

	matrix.setBrightness(50);
	Serial.begin(9600);
	pinMode(2, INPUT);  //Buton
	pinMode(3, OUTPUT); //Transistor
	pinMode(12, OUTPUT);
	pinMode(13, OUTPUT);

	matrix.setCursor(matrix.width(), 0);
	//servo.attach(3);

	pinMode(13, OUTPUT);

	//Sets inital values if eeprom is blank
	Serial.println("Reading from Eeprom");
	displayCol1 = EEPROM.read(1);
	displayCol2 = EEPROM.read(2);

	AlarmTime[0] = EEPROM.read(3);
	AlarmTime[1] = EEPROM.read(4);
	AlarmisSet = EEPROM.read(5);
	songToPlay = EEPROM.read(6);
	Serial.println("Done reading from Eeprom!");

	//Audio Stuff
	AudioMemory(5);

	sgtl5000_1.enable();
	sgtl5000_1.volume(0.5);

	pinMode(13, OUTPUT);

	SPI.setMOSI(7);
	SPI.setSCK(14);

	if (!(SD.begin(10)))
	{
		// stop here, but print a message repetitively
		while (!(SD.begin(10)))
		{
			Serial.println("Unable to access the SD card");
			delay(500);
		}
	}

	//Snake stuff
	snakeBodyX.add(3);
	snakeBodyX.add(3);

	snakeBodyY.add(1);
	snakeBodyY.add(2);

	dirList.add(Up);
	dirList.add(Right);
	dirList.add(Left);
	dirList.add(Down);
	dirList.add(Left);
}

int lastNum = 0;
bool servoPos = false;

bool hourBelow10;
bool canTest = true;

int fakeTime = 0;


void loop()
{
	matrix.fillScreen(0);

	/*
	if (onSecond() && canTest)
	{
		testLight();
		canTest = false;
	}
	else if (!onSecond())
		canTest = true;
	*/

	fixedLight();
	SetLight();

#pragma region Serialread
	if (Serial.available() > 0)
	{
		int incomingByte = Serial.read();
		Serial.println("    ");
		Serial.println("    ");
		Serial.println("I GOT");
		Serial.println(incomingByte);
		Serial.println("    ");
		Serial.println("    ");

		switch (incomingByte - 50)
		{
		case 1:
			State = TimeSetMode;
			break;

		case 2:
			State = DisplayTime;
			break;

		case 3:
			State = Menu;
			break;

		case 4:
			State = SetAlarm;
			break;

		case 5:
			State = Menu;
			break;

		case 6:
			State = TimeSetMode;
			break;

		case 7:
			State = TimeSetMode;
			break;

		case 8:
			State = TimeSetMode;
			break;
		}

		incomingByte = 0;
	}
#pragma #endregion

#pragma region Alarm Stuff
	if (AlarmisSet && State != Alarm && realHr == AlarmTime[0] && mn == AlarmTime[1] && sec == 0)
	{
		State = Alarm;
		textCursor = 0;
		matrix.fillScreen(0);
		targetBrightness = 17;
	}

	if (State == Alarm)
	{
		displayoff = false;
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

			if (songToPlay > 11)
			{
				songToPlay = 0;
				EEPROM.write(6, 0);
				Serial.println("Song Playing: ");
				Serial.println(songToPlay);
			}

			Serial.print(songToPlay);
			switch (songToPlay)
			{
			case 1:
				playFile("(1).wav");
				break;

			case 2:
				playFile("(2).wav");
				break;

			case 3:
				playFile("(3).wav");
				break;

			case 4:
				playFile("(4).wav");
				break;

			case 5:
				playFile("(5).wav");
				break;

			case 6:
				playFile("(6).wav");
				break;

			case 7:
				playFile("(7).wav");
				break;

			case 8:
				playFile("(8).wav");
				break;

			case 9:
				playFile("(9).wav");
				break;

			case 10:
				playFile("(10).wav");
				break;

			case 11:
				playFile("(11).wav");
break;
			}
		}
		//Serial.println(displayCol1);
		//Serial.println(displayCol2);
		matrix.setTextColor(colors[displayCol1]);

		delay(100);

		if (buttonPressed())
		{
			targetBrightness = daylevel;
			digitalWrite(3, LOW);
			matrix.fillScreen(0);
			State = DisplayTime;
			canPress = false;
			//servo.write(21);
			playWav1.stop();
		}
	}
#pragma #endregion

	if (State == MenuAlarm)
	{
		AlarmMenu();

		if (buttonPressed())
		{
			canPress = false;
			AlarmPageChange();
		}

		delay(100);
	}

	if (State == SetAlarm)
	{
		AlarmSet();
	}

	if (State == TimeSetMode)
	{
		TimeSet();
	}

	if (State == DisplayTime)
	{
		UpdateTime();

		if (bigMode)
			minuteAlert(displayCol2);
		if (hourBelow10)
			minuteAlert(displayCol1);

		DisplayCurrentTime(hrDisplay1, hrDisplay2, mnDisplay1, mnDisplay2, false, 2);

		displayOnOff = ReadRotary(displayOnOff, 0, 1);

		if (displayOnOff == 1)
			displayoff = false;

		if (displayOnOff == 0)
			displayoff = true;

		if (buttonPressed())
		{
			x = matrix.width();
			State = Menu;
			matrix.fillScreen(0);
			canPress = false;
		}

		////Dimmer
		//if (rtcTime.hour > 0 && rtcTime.hour < 6 && displayoff == false && dimTrigger == false)
		//{
		//	dimTrigger = true;
		//	dimtime = 3;
		//	Serial.println("TRIGGERED");
		//}

		//if (dimtime > 0 && onSecond())
		//{
		//	dimtime -= 3;
		//	Serial.println("count...");
		//}

		//if (dimtime = 0 && displayoff == false)
		//{
		//	dimTrigger = false;
		//	displayoff = true;
		//	Serial.println("off");
		//}

		//Handles displayoff
		if (displayoff)
		{
			if (rtcTime.hour > 20 || rtcTime.hour < 7)
			{
				matrix.fillScreen(0);
				targetBrightness = nightLevel;
			}
			else
			{
				targetBrightness = 1;
			}
		}
		else
		{
			if (rtcTime.hour > 20 || rtcTime.hour < 7)
			{
				targetBrightness = nightLevel;
				RedColours();
			}
			else
			{
				unRedColours();
				targetBrightness = daylevel;
			}
		}

		delay(100);
	}

	if (State == Menu)
	{
		UpdateTime();
		menu();
		//delay(100);

		if (buttonPressed())
		{
			menuPageChange();
			menuPage = 0;
		}

		//Reset rotary every hour so it doesn't overflow
		if (mn == 0 && sec == 0)
		{
			rotary.write(0);
		}
	}

	if (State == ChangeColor)
	{
		if (!bigMode)
			minuteAlert(displayCol2);
		if (hourBelow10)
			minuteAlert(displayCol1);

		UpdateTime();
		changeColor();
		delay(10);
	}

	if (State == DisplayTest)
	{
		//pirrana(500);
	}

	if (State == Snake)
	{
		snake();
	}

	if (State == StaticDisplay)
	{
		targetBrightness = ReadRotary(targetBrightness, 0, 30);

		if (buttonPressed())
			fakeTime += 1;
		

		if (fakeTime > 4)
			fakeTime = 0;

		switch (fakeTime)
		{
		case 0:
			DisplayCurrentTime(0, 4, 2, 0, false, 0);
			break;

		case 1:
			DisplayCurrentTime(1, 1, 3, 7, false, 0);
			break;

		case 2:
			DisplayCurrentTime(1, 3, 3, 7, false, 0);
			break;

		case 3:
			DisplayCurrentTime(0, 2, 4, 7, false, 0);
			break;

		case 4:
			DisplayCurrentTime(1, 2, 3, 1, false, 0);
			break;

		}
	}

	//Display Changes
	matrix.show();

	if (digitalRead(2) == LOW)
		canPress = true;
}

int menuVal;
int lastVal = 5;
int newPos;
int conveyorBelt = -8;
int conveyorTarget = 0;
bool canBounce = true;

void menu()
{
	menuPage = ReadRotary(menuPage);

	//Bounce
	if (menuPage < 0 && canBounce)
	{
		conveyorTarget = -5;
		canBounce = false;
	}

	if (menuPage > 4 && canBounce)
	{
		conveyorTarget = 53;
		canBounce = false;
	}

	if (conveyorBelt < -4)
	{
		conveyorTarget = 0;
		menuPage = 0;
	}

	if (conveyorBelt > 52)
	{
		conveyorTarget = 48;
		menuPage = 4;
	}

	//Serial.println(conveyorTarget);

	if (lastVal != menuPage)
	{
		switch (menuPage)
		{
		case 0:
			//colourIcon();
			conveyorTarget = 0;

			lastVal = 0;
			break;

		case 1:
			//speakerIcon();
			conveyorTarget = 12;
			lastVal = 1;
			break;

		case 2:
			//timeIcon();
			conveyorTarget = 24;
			lastVal = 2;
			break;

		case 3:
			//piranna();
			conveyorTarget = 36;
			lastVal = 3;
			break;

		case 4:
			//backIcon()
			conveyorTarget = 48;
			lastVal = 4;
			break;
		}
	}

	conveyBelt();

	colourIcon();
	speakerIcon(12, true);
	timeIcon(24);
	pirrana(36, 500);
	backIcon(48);
}

void menuPageChange()
{
	switch (menuPage)
	{
	case 0:
		State = ChangeColor;
		matrix.fillScreen(0);
		break;

	case 1:
		State = MenuAlarm;
		matrix.fillScreen(0);
		break;

	case 2:
		State = TimeSetMode;
		matrix.fillScreen(0);
		break;

	case 3:
		State = Snake;
		matrix.fillScreen(0);
		break;

	case 4:
		State = DisplayTime;
		matrix.fillScreen(0);
		break;
	}

	//Reset belt
	conveyorBelt = -8;
}

long timer;
void conveyBelt()
{
	timer += deltaTime();
	if (timer > 50)
	{
		if (conveyorBelt < conveyorTarget)
			conveyorBelt += 1;

		if (conveyorBelt > conveyorTarget)
			conveyorBelt -= 1;

		if (conveyorBelt == conveyorTarget)
			canBounce = true;

		timer = 0;
	}
}

//Function depricated as I̶ ̶w̶a̶s̶ ̶t̶o̶o̶ ̶l̶a̶z̶y̶ ̶t̶o̶ ̶d̶r̶i̶l̶l̶ ̶a̶ ̶h̶o̶l̶e̶  a hole for the light sensor would ruin the aesthetic of the box, Might include in future versions
void testLight()
{
	float sensor = analogRead(A7);
	if (sensor > 30)
		targetBrightness = 17;
	else if (sensor > 15)
		targetBrightness = 8;
	/*else
	targetBrightness = 1;*/

	//Serial.println(targetBrightness);
	//Serial.println(sensor);
}

void SetLight()
{
	//Serial.println(targetBrightness);
	//Serial.println(currentBrightness);

	if (currentBrightness != targetBrightness)
	{
		if (currentBrightness < targetBrightness)
		{
			currentBrightness += 1;
		}

		if (currentBrightness > targetBrightness)
		{
			currentBrightness -= 1;
		}

		if (currentBrightness < 1 && millis() > 1000)
			currentBrightness = 1;

		matrix.setBrightness(currentBrightness);
	}
}

void RedColours()
{
	for (int i = 0; i < 10; i++)
	{
		colors[i] = matrix.Color(255, 0, 0);
	}
}

void unRedColours()
{
	for (int i = 0; i < 10; i++)
	{
		colors[i] = colorsStore[i];
	}
}

void fixedLight()
{
	//Serial.println(hr);
	//Serial.println(sec);

	//Set the light to 1 at 8:00pm

	if ((rtcTime.hour > 20 && rtcTime.hour < 6))
	{
		targetBrightness = 1;
	}

	//If the alarm isn't set, set the birghtness back up to normal at 8
	if (!AlarmisSet && rtcTime.hour > 8 && rtcTime.hour < 20)
		targetBrightness = 17;
}

int ColPos = 0;

void changeColor()
{
	DisplayCurrentTime(hrDisplay1, hrDisplay2, mnDisplay1, mnDisplay2, false, 2);

	//Code will continually think the rotary encoder is turning left unless serial.print or delay(1) is used. Wtf.
	Serial.println(ColorToChoose);
	delay(1);

	ColorToChoose = ReadRotary(ColorToChoose, 1, 9);

	if (ColPos == 0)
		displayCol2 = ColorToChoose;

	if (ColPos == 1)
		displayCol1 = ColorToChoose;

	if (buttonPressed())
	{
		canPress = false;
		switch (ColPos)
		{
		case 1:
			displayCol1 = ColorToChoose;
			EEPROM.write(1, displayCol1);

			break;

		case 2:
			EEPROM.write(2, displayCol2);
			break;
		}
		ColPos++;
	}

	if (ColPos == 2)
	{
		//Serial.println("Back To displaying time");
		ColPos = 0;
		State = DisplayTime;
		matrix.fillScreen(0);
		EEPROM.write(2, displayCol2);
	}
}

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

int colpos2;
int colpos3;

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

int alarmMenuPage = 0;
int loopint = -50;
String text = "ayy";
void AlarmMenu()
{
	alarmMenuPage = ReadRotary(alarmMenuPage, 0, 1);

	if (AlarmisSet)
	{
		matrix.drawPixel(7, 0, colors[displayCol2]);
	}

	switch (alarmMenuPage)
	{
	case 0:
		/*
		x = matrix.width();
		matrix.fillScreen(0);
		matrix.setTextColor(colors[displayCol1]);
		text = "Off";
		loopint = -15;
		lastVal = 0;
		//Serial.println("reset");
		*/
		moveNum = -3;
		speakerIcon(false);
		matrix.drawLine(0, 0, 7, 7, Red);
		break;

	case 1:
		/*
		text = "On";
		matrix.fillScreen(0);
		x = matrix.width();
		matrix.setTextColor(colors[displayCol2]);
		lastVal = 1;
		loopint = -15;
		//Serial.println("reset");
		*/
		speakerIcon(true);
		break;
	}
	//matrix.print(text);
}

void AlarmPageChange()
{
	switch (alarmMenuPage)
	{
	case 0:
		AlarmisSet = false;
		EEPROM.write(5, false);
		State = DisplayTime;
		matrix.fillScreen(0);
		break;

	case 1:
		Serial.println("Setting Alarm");
		State = SetAlarm;
		AlarmisSet = true;
		EEPROM.write(5, true);
		matrix.fillScreen(0);
		break;
	}
}

void AlarmSet()
{
	if (buttonPressed())
	{
		pos++;
		canPress = false;
	}

	delay(100);

	switch (pos)
	{
	case 0:
		numToSet = ReadRotary(numToSet, 0, 2);
		break;

	case 1:
		if (time[0] == 2)
		{
			numToSet = ReadRotary(numToSet, 0, 4);
		}
		else
			numToSet = ReadRotary(numToSet, 1, 9);
		break;

	case 2:
		numToSet = ReadRotary(numToSet, 0, 5);
		break;

	case 3:
		numToSet = ReadRotary(numToSet, 0, 9);
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

		AlarmTime[0] = Newhr.toInt();
		AlarmTime[1] = Newmn.toInt();

		//Serial.println(AlarmTime[0]);
		//Serial.println(AlarmTime[1]);

		EEPROM.write(3, AlarmTime[0]);
		EEPROM.write(4, AlarmTime[1]);

		matrix.fillScreen(0);
		delay(500);
		//matrix.fillScreen(colors[displayCol2]);
		//delay(500);
		State = DisplayTime;
	}
}

boolean onSecond()
{
	rtc.readTime(&rtcTime);
	int secs = rtcTime.second;
	//Serial.println(secs);
	if (secs % 2 == 0)
	{
		return true;
	}
	else
		return false;
}

int sec2 = 1;
boolean onHalfSecond()
{
	rtc.readTime(&rtcTime);
	int secs = rtcTime.second;
	sec2++;
	//Serial.println(secs);
	if (secs % 2 == 0 && sec2 % 2 == 0)
	{
		return false;
	}
	else
		return true;
}

#pragma region	numberzzz

void displayNum(int num, int x, int y, uint16_t col)
{
	if (bigMode)
	{
		switch (num)
		{
		case 1:
			num1Big(x, y, col);
			break;

		case 2:
			num2Big(x, y, col);
			break;

		case 3:
			num3Big(x, y, col);
			break;

		case 4:
			num4Big(x, y, col);
			break;

		case 5:
			num5Big(x, y, col);
			break;

		case 6:
			num6Big(x, y, col);
			break;

		case 7:
			num7Big(x, y, col);
			break;

		case 8:
			num8Big(x, y, col);
			break;

		case 9:
			num9Big(x, y, col);
			break;

		case 0:
			num0Big(x, y, col);
			break;
		}
	}
	else
	{
		switch (num)
		{
		case 1:
			num1(x, y, col);
			break;

		case 2:
			num2(x, y, col);
			break;

		case 3:
			num3(x, y, col);
			break;

		case 4:
			num4(x, y, col);
			break;

		case 5:
			num5(x, y, col);
			break;

		case 6:
			num6(x, y, col);
			break;

		case 7:
			num7(x, y, col);
			break;

		case 8:
			num8(x, y, col);
			break;

		case 9:
			num9(x, y, col);
			break;

		case 0:
			num0(x, y, col);
			break;
		}
	}
}

void num1(int x, int y, uint16_t col)
{
	int xmod = x * 4;
	int ymod = y * 4;
	matrix.drawLine(2 + xmod, 0 + ymod, 2 + xmod, 3 + ymod, col);
	//matrix.drawPixel(2 + xmod,1 + ymod,col);
	matrix.drawPixel(1 + xmod, 1 + ymod, col);
}

void num2(int x, int y, uint16_t col)
{
	int xmod = x * 4;
	int ymod = y * 4;
	matrix.drawLine(1 + xmod, 0 + ymod, 2 + xmod, 0 + ymod, col);
	matrix.drawPixel(2 + xmod, 1 + ymod, col);
	matrix.drawPixel(1 + xmod, 2 + ymod, col);
	matrix.drawLine(1 + xmod, 3 + ymod, 2 + xmod, 3 + ymod, col);
}

void num3(int x, int y, uint16_t col)
{
	int xmod = x * 4;
	int ymod = y * 4;
	matrix.drawLine(2 + xmod, 0 + ymod, 2 + xmod, 3 + ymod, col);
	matrix.drawPixel(1 + xmod, 0 + ymod, col);
	matrix.drawPixel(1 + xmod, 3 + ymod, col);
}

void num4(int x, int y, uint16_t col)
{
	int xmod = x * 4;
	int ymod = y * 4;

	matrix.drawLine(2 + xmod, 1 + ymod, 2 + xmod, 3 + ymod, col);
	matrix.drawLine(1 + xmod, 0 + ymod, 1 + xmod, 1 + ymod, col);
}

void num5(int x, int y, uint16_t col)
{
	int xmod = x * 4;
	int ymod = y * 4;

	matrix.drawLine(1 + xmod, 0 + ymod, 2 + xmod, 0 + ymod, col);
	matrix.drawPixel(1 + xmod, 1 + ymod, col);
	matrix.drawPixel(2 + xmod, 2 + ymod, col);
	matrix.drawLine(1 + xmod, 3 + ymod, 2 + xmod, 3 + ymod, col);
}

void num6(int x, int y, uint16_t col)
{
	int xmod = x * 4;
	int ymod = y * 4;

	matrix.drawLine(1 + xmod, 0 + ymod, 1 + xmod, 3 + ymod, col);
	matrix.drawLine(2 + xmod, 2 + ymod, 2 + xmod, 3 + ymod, col);
	matrix.drawPixel(2 + xmod, 0 + ymod, col);
}

void num7(int x, int y, uint16_t col)
{
	int xmod = x * 4;
	int ymod = y * 4;
	matrix.drawLine(2 + xmod, 0 + ymod, 2 + xmod, 3 + ymod, col);
	matrix.drawPixel(1 + xmod, 0 + ymod, col);
}

void num8(int x, int y, uint16_t col)
{
	int xmod = x * 4;
	int ymod = y * 4;
	matrix.drawLine(2 + xmod, 0 + ymod, 2 + xmod, 3 + ymod, col);
	matrix.drawLine(1 + xmod, 0 + ymod, 1 + xmod, 3 + ymod, col);
}

void num9(int x, int y, uint16_t col)
{
	int xmod = x * 4;
	int ymod = y * 4;
	matrix.drawLine(2 + xmod, 0 + ymod, 2 + xmod, 3 + ymod, col);
	matrix.drawPixel(1 + xmod, 0 + ymod, col);
	matrix.drawPixel(1 + xmod, 1 + ymod, col);
}

void num0(int x, int y, uint16_t col)
{
	int xmod = x * 4;
	int ymod = y * 4;
	matrix.drawLine(1 + xmod, 0 + ymod, 2 + xmod, 0 + ymod, col);
	matrix.drawLine(1 + xmod, 0 + ymod, 1 + xmod, 3 + ymod, col);
	matrix.drawLine(1 + xmod, 3 + ymod, 2 + xmod, 3 + ymod, col);
}

void num0Big(int x, int y, uint16_t col)
{
	matrix.drawLine(0 + x, 0, 0 + x, 7, col);
	matrix.drawLine(1 + x, 0, 1 + x, 7, col);

	//matrix.drawLine(0 + x, 0, 1 + x, 0, col);
	//matrix.drawLine(0 + x, 7, 1 + x, 7, col);
}

void num1Big(int x, int y, uint16_t col)
{
	if (x > 4)
		matrix.drawLine(1 + x, 0, 1 + x, 7, col);
	else
		matrix.drawLine(0 + x, 0, 0 + x, 7, col);
}

void num2Big(int x, int y, uint16_t col)
{
	matrix.drawLine(0 + x, 0, 1 + x, 0, col);
	matrix.drawLine(1 + x, 0, 1 + x, 4, col);
	matrix.drawLine(0 + x, 4, 0 + x, 7, col);
	matrix.drawPixel(1 + x, 7, col);
}

void num3Big(int x, int y, uint16_t col)
{
	matrix.drawLine(0 + x, 0, 1 + x, 0, col);
	matrix.drawLine(1 + x, 0, 1 + x, 7, col);
	matrix.drawLine(0 + x, 4, 0 + x, 4, col);
	matrix.drawLine(0 + x, 7, 1 + x, 7, col);
}

void num4Big(int x, int y, uint16_t col)
{
	matrix.drawLine(0 + x, 0, 0 + x, 4, col);
	matrix.drawLine(1 + x, 4, 1 + x, 7, col);
}

void num5Big(int x, int y, uint16_t col)
{
	matrix.drawLine(0 + x, 0, 1 + x, 0, col);
	matrix.drawLine(0 + x, 0, 0 + x, 4, col);
	matrix.drawLine(1 + x, 4, 1 + x, 7, col);
	matrix.drawPixel(0 + x, 7, col);
}

void num6Big(int x, int y, uint16_t col)
{
	matrix.drawLine(0 + x, 0, 1 + x, 0, col);
	matrix.drawLine(0 + x, 0, 0 + x, 7, col);
	matrix.drawLine(1 + x, 4, 1 + x, 7, col);
}

void num7Big(int x, int y, uint16_t col)
{
	matrix.drawLine(1 + x, 0, 1 + x, 7, col);
	matrix.drawPixel(0 + x, 0, col);
}

void num8Big(int x, int y, uint16_t col)
{
	matrix.drawRect(0 + x, 0, 2, 4, col);
	matrix.drawRect(0 + x, 5, 2, 3, col);
}

void num9Big(int x, int y, uint16_t col)
{
	matrix.drawLine(0 + x, 0, 0 + x, 4, col);
	matrix.drawLine(1 + x, 0, 1 + x, 7, col);
}

#pragma endregion

void playFile(String filename)
{
	Serial.print("Playing file: ");
	Serial.println(filename);

	// Start playing the file.  This sketch continues to
	// run while the file plays.
	playWav1.play(filename.c_str());

	// A brief delay for the library read WAV info
	delay(5);

	// Simply wait for the file to finish playing.
	//while (playWav1.isPlaying()) {
	// uncomment these lines if you audio shield
	// has the optional volume pot soldered
	//float vol = analogRead(15);
	//vol = vol / 1024;
	// sgtl5000_1.volume(vol);
	//}
}

boolean buttonPressed()
{
	if (digitalRead(2) == HIGH && canPress)
	{
		canPress = false;
		return true;
		Serial.print("Button Pressed");
	}
	else
		return false;
}

int ReadRotary(int varToChange, int min, int max)
{
	long newPosition = rotary.read();
	// Serial.println(newPosition);
	if (newPosition != oldPosition)
	{
		int mod;
		//delay(5);
		if ((newPosition / 4) < (oldPosition / 4))
		{
			Serial.println("Left");
			oldPosition = newPosition;
			varToChange += 1;
		}

		if ((newPosition / 4) > (oldPosition / 4))
		{
			Serial.println("Right");
			oldPosition = newPosition;
			varToChange -= 1;
		}

		if (varToChange > max)
		{
			varToChange = min;
		}

		if (varToChange < min)
		{
			varToChange = max;
		}

		return varToChange;
	}
}

int ReadRotary(int varToChange)
{
	long newPosition = rotary.read();
	// Serial.println(newPosition);
	if (newPosition != oldPosition)
	{
		//int mod;
		//delay(5);
		if ((newPosition / 4) > (oldPosition / 4))
		{
			Serial.println("Left");
			oldPosition = newPosition;
			return varToChange += 1;
		}

		if ((newPosition / 4) < (oldPosition / 4))
		{
			Serial.println("Right");
			oldPosition = newPosition;
			return varToChange -= 1;
		}
	}
}

void GetTemperature()
{
	int t1 = rtc.readTemperature(); // retrieve the value from the DS3232
	float temp = t1 / 4; // temperature in Celsius stored in temp
	delay(100);
	Serial.print(temp);
	//return temp;
}

long tempTimer = 0;

int deltaTime()
{
	/*
	?????????????????? ????? ?????????????????
	~ ~ ~ ~ ~ ~ ~ ~ ~ Repost this if ~ ~ ~ ~ ~ ~ ~
	~ ~ ~ ~ you are a beautiful strong millis()~ ~
	~ ~ ~ ~ ~ ~ ~ who don’t need no delay()  ~ ~ ~
	?????????????????? ????? ?????????????????
	*/

	int deltaTime;

	deltaTime = millis() - tempTimer;
	tempTimer = millis();
	return deltaTime;
}

long tempTimer2 = 0;
int deltaTime2()
{
	int deltaTime;

	deltaTime = millis() - tempTimer2;
	tempTimer2 = millis();
	return deltaTime;
}

long tempTimer3 = 0;

int deltaTime3()
{
	int deltaTime;
	deltaTime = millis() - tempTimer3;
	tempTimer3 = millis();
	return deltaTime;
}

int deltaTime4()
{
	int deltaTime;

	deltaTime = millis() - tempTimer4;
	tempTimer4 = millis();
	return deltaTime;
}

int colSwitch1;
int colSwitch2;

int timer3 = 0;
bool timerSwitch = false;
void colourIcon()
{
	//Serial.println(timer3);
	if (timerSwitch)
		timer3 += deltaTime3();

	if (timer3 > 1000)
	{
		srandom(millis());
		int rnd;
		rnd = random(1, 4);
		colSwitch1 += rnd;

		//Serial.println("Random: ");
		//Serial.println(rnd);

		rnd = random(1, 4);
		colSwitch2 += rnd;
		//Serial.println("Random: ");
		//Serial.println(rnd);

		//Serial.println();
		//Serial.print("ColSwitch1:");
		//Serial.println(colSwitch1);
		//Serial.println();
		//Serial.print("ColSwitch2:");
		//Serial.println(colSwitch2);

		timer3 = 0;

		Serial.println("ayy");
	}

	if (!timerSwitch)
	{
		colSwitch1 = displayCol1;
		colSwitch2 = displayCol2;

		timerSwitch = true;
	}

	if (colSwitch1 == colSwitch2)
		colSwitch2 += 5;

	if (colSwitch1 > 8)
	{
		colSwitch1 -= 8;
		//Serial.println("Ayy");
	}

	if (colSwitch2 > 8)
	{
		//Serial.println("Ayy");
		colSwitch2 -= 8;
	}

	matrix.fillRoundRect(conveyorBelt + 1, 1, 6, 6, 1, colors[colSwitch1]);

	matrix.fillTriangle(conveyorBelt + 2, 5, conveyorBelt + 5, 2, conveyorBelt + 2, 2, colors[colSwitch2]);

	matrix.drawLine(conveyorBelt + 2, 1, conveyorBelt + 5, 1, colors[colSwitch2]);
	matrix.drawLine(conveyorBelt + 1, 2, conveyorBelt + 1, 5, colors[colSwitch2]);
}

int timer2;
void speakerIcon(int startPos, bool play)
{
	if (play)
	{
		timer2 += deltaTime2();
		if (timer2 > 200)
		{
			moveNum--;
			timer2 = 0;
		}

		if (moveNum < -2)
		{
			moveNum = 3;
		}
	}

	int tempConveyor = conveyorBelt - startPos;

	if (moveNum > -1 || conveyorBelt != conveyorTarget)
	{
		matrix.drawLine(tempConveyor + moveNum, 2, tempConveyor + moveNum, 5, colors[displayCol1]);
	}

	if (moveNum > -2 || conveyorBelt != conveyorTarget)
	{
		matrix.drawPixel(tempConveyor + moveNum + 1, 1, colors[displayCol1]);
		matrix.drawPixel(tempConveyor + moveNum + 1, 6, colors[displayCol1]);
	}

	matrix.drawLine(tempConveyor + 3, 0, tempConveyor + 3, 7, colors[displayCol2]);
	matrix.drawLine(tempConveyor + 4, 1, tempConveyor + 4, 6, colors[displayCol2]);
	matrix.drawLine(tempConveyor + 5, 2, tempConveyor + 5, 5, colors[displayCol2]);
	matrix.drawRect(tempConveyor + 6, 3, tempConveyor + 2, 2, colors[displayCol2]);

	//Serial.println(moveNum);
}

void speakerIcon(bool play)
{
	if (play)
	{
		timer2 += deltaTime2();

		if (timer2 > 200)
		{
			moveNum--;
			timer2 = 0;
		}

		if (moveNum < -2)
		{
			moveNum = 3;
		}
	}

	if (moveNum > -1)
	{
		matrix.drawLine(moveNum, 2, moveNum, 5, colors[displayCol1]);
	}

	if (moveNum > -2)
	{
		matrix.drawPixel(moveNum + 1, 1, colors[displayCol1]);
		matrix.drawPixel(moveNum + 1, 6, colors[displayCol1]);
	}

	matrix.drawLine(3, 0, 3, 7, colors[displayCol2]);
	matrix.drawLine(4, 1, 4, 6, colors[displayCol2]);
	matrix.drawLine(5, 2, 5, 5, colors[displayCol2]);
	matrix.drawRect(6, 3, 2, 2, colors[displayCol2]);

	//Serial.println(moveNum);
}

void timeIcon(int startpos)
{
	//  int tempConveyor = conveyorBelt - 16;
	int tempConveyor = conveyorBelt - startpos;

	matrix.drawRoundRect(tempConveyor + 1, 1, 6, 6, 1, colors[displayCol1]);
	matrix.drawTriangle(tempConveyor + 3, 4, tempConveyor + 3, 3, tempConveyor + 4, 4, colors[displayCol1]);
}

int backMove = 0;

void backIcon(int startpos)
{
	/*
	timer3 += deltaTime3();

	if (timer3 > 500)
	{
	timer3 = 0;
	backMove--;
	}
	*/

	if (backMove < -1)
		backMove = 0;

	//int tempConveyor = conveyorBelt - 24;
	int tempConveyor = conveyorBelt - startpos;

	matrix.drawTriangle(tempConveyor + backMove + 2, 2, tempConveyor + backMove + 3, 1, tempConveyor + backMove + 3, 2, colors[displayCol2]);
	matrix.drawRect(tempConveyor + backMove + 1, 3, 6, 2, colors[displayCol2]);
	matrix.drawTriangle(tempConveyor + backMove + 2, 5, tempConveyor + backMove + 3, 5, tempConveyor + backMove + 3, 6, colors[displayCol2]);
}

void snake()
{
	//On second game gameover() doesn't work;
	if (ate == true)
	{
		score += 1;

		//Grow snake
		snakeBodyX.add(snakeBodyX.get(snakeLength - 1));
		snakeBodyY.add(snakeBodyY.get(snakeLength - 1));
		snakeLength += 1;

		//randomSeed(A0);
		//Spawn food
		Serial.println("Spawned new food");

		foodx = random(0, 7);
		foody = random(0, 7);

		//Make sure food doesn't spawn in body
		for (int i = 0; i < snakeLength; i++)
		{
			if (snakeBodyX.get(i) == foodx)
			{
				while(foodx != snakeBodyX.get(i))
					foodx = random(0, 7);
				//inBody = true;
				Serial.println("Inbodyx!");
			}

			if (snakeBodyY.get(i) == foodx)
			{
				while (foody != snakeBodyY.get(i))
					foody = random(0, 7);
				//inBody = true;
				Serial.println("Inbodyy!");
			}
		}

		inBody = true;
		ate = false;
	}

	//Render food
	matrix.drawPixel(foodx, foody, colors[displayCol2]);

	//Get input
	int inputDirMod = ReadRotary(inputDirMod);

	//Intuituve direction instead of fixed state direction
	switch (dir)
	{
	case Up:
		if (inputDirMod == 1)
		{
			dir = Left;
			inputDirMod = 0;
		}

		if (inputDirMod == -1)
		{
			dir = Right;
			inputDirMod = 0;
		}
		break;

	case Right:
		if (inputDirMod == 1)
		{
			dir = Up;
			inputDirMod = 0;
		}

		if (inputDirMod == -1)
		{
			dir = Down;
			inputDirMod = 0;
		}
		break;

	case Down:
		if (inputDirMod == -1)
		{
			dir = Left;
			inputDirMod = 0;
		}

		if (inputDirMod == 1)
		{
			dir = Right;
			inputDirMod = 0;
		}
		break;

	case Left:
		if (inputDirMod == -1)
		{
			dir = Up;
			inputDirMod = 0;
		}

		if (inputDirMod == 1)
		{
			dir = Down;
			inputDirMod = 0;
		}
		break;
	}

	//remove tail
	snakeBodyX.remove(snakeLength - 1);
	snakeBodyY.remove(snakeLength - 1);

	//Get head pos
	int headx = snakeBodyX.get(0);
	int heady = snakeBodyY.get(0);

	//get new head pos
	if (dir == Left)
	{
		headx = headx - 1;
	}
	if (dir == Right)
	{
		headx = headx + 1;
	}
	if (dir == Up)
	{
		heady = heady - 1;
	}
	if (dir == Down)
	{
		heady = heady + 1;
	}

	//Loop if out of bounds
	if (headx > 7)
		headx = 0;
	if (headx < 0)
		headx = 7;
	if (heady > 7)
		heady = 0;
	if (heady < 0)
		heady = 7;

	//Set new head pos
	snakeBodyX.unshift(headx);
	snakeBodyY.unshift(heady);

	//Check if eaten food
	if (headx == foodx && heady == foody)
	{
		ate = true;
	}

	//Render Snake && check for collision
	for (int i; i < snakeLength; i++)
	{
		matrix.drawPixel(snakeBodyX.get(i), snakeBodyY.get(i), colors[displayCol1]);

		if (i > 0)
		{
			if (headx == snakeBodyX.get(i) && heady == snakeBodyY.get(i))
			{
				gameOver();
				i = 999;
			}
		}
	}

	delay(300);
}

void gameOver()
{
	delay(2000);
	matrix.fillScreen(0);
	matrix.fillRect(0, 0, 8, 8, colors[displayCol1]);
	matrix.show();
	delay(1000);

	matrix.fillScreen(0);
	matrix.fillRect(0, 0, 8, 8, colors[displayCol2]);
	matrix.show();
	delay(1000);

	while (gameOverloop)
	{
		matrix.fillScreen(0);

		displayNum(score / 10 % 10, 1, 0, colors[displayCol1]);
		displayNum(score % 10, 4, 0, colors[displayCol2]);

		matrix.drawLine(7, 0, 7, 5, colors[displayCol1]);
		matrix.drawPixel(7, 7, colors[displayCol1]);
		matrix.show();

		if (buttonPressed())
		{
			headx = 3;
			heady = 3;
			snakeLength = 2;
			score = -1;

			State = DisplayTime;
			gameOverloop = false;
		}
	}
}

//Thanks to Carlotta Tatti of hack.hands() for this sweet function
void shiftBackwards(int myarray[], int size, int shiftBy)
{
	if (shiftBy > size) {
		shiftBy = shiftBy - size;
	}

	if (size == 1) {
		//do nothing
	}
	else {
		int temp = 0;
		//for loop to print the array with indexes moved up (to the left) <-- by 2
		for (int i = 0; i < size - shiftBy; i++)
		{//EXAMPLE shift by 3  for a c-string of 5
			temp = myarray[i];					//temp = myarray[0]
			myarray[i] = myarray[i + shiftBy];	//myarray[0] == myarray[2]
			myarray[i + shiftBy] = temp;		//myarray[2] = temp(value previously at index i)
		}
	}
}

void pirrana(int startpos, int timer)
{
	timerTemp += deltaTime4();

	if (timerTemp > timer)
	{
		if (mouth)
		{
			mouth = false;
		}
		else
		{
			mouth = true;
		}

		timerTemp = 0;
	}

	int tempConveyor = conveyorBelt - startpos;

	if (mouth)
	{
		//Stem
		matrix.drawLine(tempConveyor + 2, 7, tempConveyor + 4, 7, LightGreen);
		matrix.drawLine(tempConveyor + 3, 5, tempConveyor + 3, 7, LightGreen);
		matrix.drawPixel(tempConveyor + 1, 6, LightGreen);
		matrix.drawPixel(tempConveyor + 5, 6, LightGreen);

		//Head
		matrix.drawLine(tempConveyor + 2, 4, tempConveyor + 4, 4, colors[displayCol1]);
		matrix.drawTriangle(tempConveyor + 1, 2, tempConveyor + 1, 3, tempConveyor + 2, 3, colors[displayCol1]);
		matrix.drawTriangle(tempConveyor + 5, 2, tempConveyor + 5, 3, tempConveyor + 4, 3, colors[displayCol1]);

		matrix.drawLine(tempConveyor + 1, 1, tempConveyor + 3, 3, White);
		matrix.drawLine(tempConveyor + 4, 2, tempConveyor + 5, 1, White);
	}
	else
	{
		//Stem
		matrix.drawLine(tempConveyor + 2, 7, tempConveyor + 4, 7, LightGreen);
		matrix.drawPixel(tempConveyor + 1, 6, LightGreen);
		matrix.drawPixel(tempConveyor + 5, 6, LightGreen);
		matrix.drawLine(tempConveyor + 3, 5, tempConveyor + 3, 7, LightGreen);

		//Head
		matrix.drawLine(tempConveyor + 2, 4, tempConveyor + 4, 4, colors[displayCol1]);
		matrix.drawLine(tempConveyor + 1, 2, tempConveyor + 1, 3, colors[displayCol1]);
		matrix.drawLine(tempConveyor + 2, 1, tempConveyor + 2, 4, colors[displayCol1]);

		matrix.drawLine(tempConveyor + 4, 1, tempConveyor + 4, 4, colors[displayCol1]);
		matrix.drawLine(tempConveyor + 5, 2, tempConveyor + 5, 3, colors[displayCol1]);

		matrix.drawLine(tempConveyor + 3, 1, tempConveyor + 3, 3, White);
	}
}