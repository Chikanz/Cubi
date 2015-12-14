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

//Phasing out
//#include <SoftI2C.h>
//#include <DS3232RTC.h>
//

#include <Time.h>

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>

#include <LinkedList.h>

#define ENCODER_OPTIMIZE_INTERRUPTS
#include <Encoder.h>
//#include <avr/power.h>

// Audio stuff
//AudioPlaySdWav           playWav1;       //xy=154,78
//AudioOutputI2S           i2s1;           //xy=334,89
//AudioConnection          patchCord1(playWav1, 0, i2s1, 0);
//AudioConnection          patchCord2(playWav1, 1, i2s1, 1);
//AudioControlSGTL5000     sgtl5000_1;     //xy=240,153

// GUItool: begin automatically generated code
AudioPlaySdWav           playWav1;     //xy=253,243
AudioMixer4              mixer1;         //xy=446,247
AudioOutputI2S           i2s1;           //xy=604,246
AudioConnection          patchCord1(playWav1, 0, mixer1, 0);
AudioConnection          patchCord2(mixer1, 0, i2s1, 0);
AudioConnection          patchCord3(mixer1, 0, i2s1, 1);
AudioControlSGTL5000     sgtl5000_1;     //xy=254,324
										 // GUItool: end automatically generated code


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
	TimeSetMode,	
	Main,	
	SetAlarm,		
	MenuAlarm,	
	Brightness,
	Alarm,			
	ChangeColor,	
	DisplayTest,	
	Snake,			
	SnakeGameOver,	
	StaticDisplay,	
};

//
///
////
eState State = Main;
////
///
//

int textCursor = 8;
long Rpos = -999;
int songToPlay;
int moveNum = 3;

int targetBrightness = 17;
int currentBrightness = 0;

//bool displayoff = true;
bool display = true;
int displayOnOff = 0;

//int nightLevel = 0;
int daylevel = 17;

//bool dimTrigger = false;
int dimtime = 0;

//Snake
LinkedList<int> snakeBodyX = LinkedList<int>();
LinkedList<int> snakeBodyY = LinkedList<int>();

enum eDirection { Up, Right, Down, Left};
eDirection dir = Up;
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


File root;
int fileCount;


//Menu
int conveyorBelt = 0;
int conveyorTarget = 0;
bool canBounce = false;
int menuPage = -1;

float targetVolume = 1;
float volume = 0;
int fadeTimer = 0;

//Compiler didn't like it when class was moved to another place, temp fix
class NumConveyor
{
	int belt, target;
	int ymod = 0;
	int speed;

	uint16_t col;

	long timer;

	int velocity;

public:

	void Update(int numTarget, int x, int speed, uint16_t colour)
	{
		target = numTarget * 11;

		num0Big(x, 0 - ymod, colour);
		num1Big(x, 11 - ymod, colour);
		num2Big(x, 22 - ymod, colour);
		num3Big(x, 33 - ymod, colour);
		num4Big(x, 44 - ymod, colour);
		num5Big(x, 55 - ymod, colour);
		num6Big(x, 66 - ymod, colour);
		num7Big(x, 77 - ymod, colour);
		num8Big(x, 88 - ymod, colour);
		num9Big(x, 99 - ymod, colour);

		timer += 51;

		if (timer > speed)
		{
			if (belt != target)
			{
				if (belt < target)
					belt++;

				if (belt > target)
					belt--;
			}
			timer = 0;
		}		

		ymod = belt;

		Serial.print("x");
		Serial.println(x);

		Serial.print("ymod");
		Serial.println(ymod);

		Serial.print("belt");
		Serial.println(belt);
		Serial.println("--------");
	}
};

NumConveyor numConvey1;
NumConveyor numConvey2;
NumConveyor numConvey3;
NumConveyor numConvey4;

NumConveyor setConvey1;
NumConveyor setConvey2;
NumConveyor setConvey3;
NumConveyor setConvey4;


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

	//EEPROM startup
	Serial.println("Reading from Eeprom");
	displayCol1 = EEPROM.read(1);
	displayCol2 = EEPROM.read(2);

	AlarmTime[0] = EEPROM.read(3);
	AlarmTime[1] = EEPROM.read(4);

	AlarmisSet = EEPROM.read(5);
	songToPlay = EEPROM.read(6);
	Serial.println("Done reading from Eeprom!");

	//Audio
	AudioMemory(5);

	sgtl5000_1.enable();
	mixer1.gain(0, 0);

	//SD card
	SPI.setMOSI(7);
	SPI.setSCK(14);

	if (!(SD.begin(10)))
	{
		while (!(SD.begin(10)))
		{
			Serial.println("Unable to access the SD card");
			delay(500);
		}
	}

	//Snake
	snakeBodyX.add(3);
	snakeBodyX.add(3);

	snakeBodyY.add(1);
	snakeBodyY.add(2);

	dirList.add(Up);
	dirList.add(Right);
	dirList.add(Left);
	dirList.add(Down);
	dirList.add(Left);

	//Directory Read SD card
	root = SD.open("/");
	countDirectory(root);
	fileCount -= 1;
	Serial.println("Files in dir:");
	Serial.print(fileCount);

	//Teensy RTC setup
	setSyncProvider(getTeensy3Time);

}

//int lastNum = 0;
bool servoPos = false;

bool hourBelow10;
bool canTest = true;

int fakeTime = 0;

bool redded = false;

//

int i = 0;
void loop()
{
	matrix.fillScreen(0);

	fixedLight();
	SetLight();

	if (Serial.available() > 0)
	{
		time_t t = processSyncMessage();
		if (t != 0) {
			Teensy3Clock.set(t); // set the RTC
			setTime(t);
		}
	}

	//Alarm
	if (AlarmisSet && State != Alarm && realHr == AlarmTime[0] && mn == AlarmTime[1] && sec == 0)
	{
		State = Alarm;
		textCursor = 0;
		matrix.fillScreen(0);
		targetBrightness = 17;
	}

	if (State == Alarm)
	{
		if (volume < targetVolume)
		{
			volume += 0.002;
			fadeTimer = 0;
		}
		else
			fadeTimer += 51;

		Serial.print("Volume:");
		Serial.println(volume);
		mixer1.gain(0, volume);

		PlayAlarm();
	}

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

	if (State == Main)
	{
		DisplayCurrentTime(hrDisplay1, hrDisplay2, mnDisplay1, mnDisplay2, conveyorBelt);

		UpdateTime();
		menu();
		

		if (buttonPressed())
		{
			menuPageChange();
			//Reset rotary to hopefully avoid overflow
			//rotary.write(0);
		}
	}

	if (State == Brightness)
	{
		UpdateTime();
		//DisplayCurrentTime(hrDisplay1, hrDisplay2, mnDisplay1, mnDisplay2, false,0);
		DisplayCurrentTime(hrDisplay1, hrDisplay2, mnDisplay1, mnDisplay2, 8);

		if (buttonPressed())
		{
			State = Main;
			Serial.print("button");
			Serial.println(State);
			matrix.fillScreen(0);
			//delay(100);
			canPress = false;
		}
		delay(100);
	}

	if (State == ChangeColor)
	{
		if (hourBelow10)
			minuteAlert(displayCol1,0);

		//UpdateTime();
		changeColor();
		delay(10);
	}

	if (State == DisplayTest)
	{
		numConvey1.Update(2,0,50,colors[displayCol1]);		

		delay(100);
	}

	if (State == Snake)
	{
		snake();
	}

	if (State == StaticDisplay)
	{
		staticDisplay();
	}

	if (State == SnakeGameOver)
	{
		displayNum(score / 10 % 10, 1, 0, colors[displayCol1],true);
		displayNum(score % 10, 4, 0, colors[displayCol2],true);

		matrix.drawLine(7, 0, 7, 5, colors[displayCol1]);
		matrix.drawPixel(7, 7, colors[displayCol1]);
		matrix.show();

		if (buttonPressed())
		{
			headx = 3;
			heady = 3;
			snakeLength = 3;
			score = 0;

			State = Main;
		}
	}

	//Display Changes
	matrix.show();

	if (digitalRead(2) == LOW)
		canPress = true;
}
//

void SetLight()
{
	/*Serial.print("Target:");
	Serial.println(targetBrightness);
	Serial.print("Current:");
	Serial.println(currentBrightness);*/

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
	//Red
	if (hour() > 21 || hour() < 7)
		RedColours();

	//Dimmer
	if (State == Brightness)
	{
		displayOnOff = ReadRotary(displayOnOff, 0, 1);

		if (displayOnOff == 1)
			display = false;

		if (displayOnOff == 0)
			display = true;

		Serial.println(displayOnOff);

		if (display)
		{
			if (hour() > 20 || hour() < 7) //on
			{
				targetBrightness = 1; //Display on, Night
				//Serial.println("Day, Night");
			}
			else
			{
				targetBrightness = 17; //Day, Display on
				//Serial.println("Day, Display on");
			}
		}
		else
		{
			if (hour() > 20 || hour() < 7) //off
			{
				matrix.fillScreen(0); //Off, Night
				//Serial.println("off night");
			}
			else
			{
				targetBrightness = 5; //Off, Day
			}
		}
	}
	
	if ((hour() == 20 && sec == 0)) //Set the light to 1 at 8:00pm
	{
		targetBrightness = 5;
	}


	if (!AlarmisSet && hour() == 8)
	{
		targetBrightness = 17; //If the alarm isn't set, set the birghtness back up to normal at 8
		unRedColours();
	}

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
		State = Main;
		matrix.fillScreen(0);
		EEPROM.write(2, displayCol2);
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
	if (buttonPressed())
	{
		pos++;
		canPress = false;
		numToSet = 0;
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
			numToSet = ReadRotary(numToSet, 0, 9);
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
		EEPROM.write(5, true);

		matrix.fillScreen(0);
		delay(500);
		//matrix.fillScreen(colors[displayCol2]);
		//delay(500);
		State = Main;
	}
}

boolean onSecond()
{
	//rtc.readTime(&rtcTime);
	//int secs = rtcTime.second;
	//Serial.println(secs);
	if (second() % 2 == 0)
	{
		return true;
	}
	else
		return false;
}

//Not really on half second, but makes a cool blink effect
int sec2 = 1;
boolean onHalfSecond()
{
	//rtc.readTime(&rtcTime);
	//int secs = second()
	sec2++;
	//Serial.println(secs);
	if (second() % 2 == 0 && sec2 % 2 == 0)
	{
		return false;
	}
	else
		return true;
}

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

//Will replace with thermister analog read some day
//void GetTemperature()
//{
//	int t1 = rtc.readTemperature(); // retrieve the value from the DS3232
//	float temp = t1 / 4; // temperature in Celsius stored in temp
//	delay(100);
//	Serial.print(temp);
//	//return temp;
//}

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
				Serial.println("Gayyyme over");
			}
		}
	}

	delay(300);
}

void gameOver()
{
	matrix.fillScreen(0);
	matrix.fillRect(0, 0, 8, 8, colors[displayCol1]);
	matrix.show();
	delay(1000);

	matrix.fillScreen(0);
	matrix.fillRect(0, 0, 8, 8, colors[displayCol2]);
	matrix.show();
	delay(1000);

	State = SnakeGameOver;
}

//Thanks to Carlotta Tatti of hack.hands()
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

void countDirectory(File dir) {
	while (true) 
	{
		File entry = dir.openNextFile();
		if (!entry) 
		{
			// no more files
			break;
		}

		fileCount += 1;

		entry.close();
	}
}

//From the teesny 3 RTC example code
unsigned long processSyncMessage() 
{
	unsigned long pctime = 0L;
	const unsigned long DEFAULT_TIME = 1357041600; // Jan 1 2013 

	if (Serial.find("T")) //Time header
	{
		pctime = Serial.parseInt();
		return pctime;
		if (pctime < DEFAULT_TIME) { // check the value is a valid time (greater than Jan 1 2013)
			pctime = 0L; // return 0 to indicate that the time is not valid
		}
	}
	return pctime;
}