//Cubi, a playful LED powered alarm clock.
//Zac Lucarelli 2k16

/*
EEPROM Memory allocation
0 - null
1 - DisplayCol1
2 - DisplayCol2
3 - AlarmTime1
4 - AlarmTime2
5 - AlarmIsSet
10- BrightnessProfileArray
*/

//So many libraries the Citadel is jelly
#include "Classes.h"
#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#include <EEPROM.h>
#include <Time.h>
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <LinkedList.h>

#define ENCODER_OPTIMIZE_INTERRUPTS
#include <Encoder.h>

// GUItool: begin automatically generated code
AudioPlaySdWav           playWav1;     //xy=253,243
AudioMixer4              mixer1;         //xy=446,247
AudioOutputI2S           i2s1;           //xy=604,246
AudioConnection          patchCord1(playWav1, 0, mixer1, 0);
AudioConnection          patchCord2(mixer1, 0, i2s1, 0);
AudioConnection          patchCord3(mixer1, 0, i2s1, 1);
AudioControlSGTL5000     sgtl5000_1;    //xy=254,324
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
	Green,					//1
	LightGreen,				//2
	Yellow,					//3
	LightOrange,			//4
	Orange,					//5
	Red,					//6
	Purple,					//7
	Blue,					//8
	Cyan,					//9
	White,					//10
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
	White,
};

int hr, mn;
int sec;

long oldPosition = -999;

int hrDisplay1, hrDisplay2, mnDisplay1, mnDisplay2;

int temphrDisplay1, temphrDisplay2, tempmnDisplay1, tempmnDisplay2;

boolean HourMode24 = false;

//TimeSet Stuff
int selectedNum = 0;
int val;
int numToSet;
int NumSetPos = 0;

boolean canPress = true;

bool AlarmisSet = false;

int AlarmTime[] = { 0, 0 };
int time[] = { 0, 0, 0, 0 };

bool bigMode = true;

int ColorToChoose = 1;

int textCursor = 8;
long Rpos = -999;
int songToPlay;
int moveNum = 3;

int targetBrightness = 20;
int currentBrightness = 0;

int daylevel = 20;

//Snake
LinkedList<int> snakeBodyX = LinkedList<int>();
LinkedList<int> snakeBodyY = LinkedList<int>();

enum eDirection { Up, Right, Down, Left };
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

//File count
File root;
int fileCount;

//Menu stuff
int conveyorBelt = 0;
int conveyorTarget = 0;
bool canBounce = false;
int menuPage = -1;

float targetVolume = 1;
float volume = 0;
int fadeTimer = 0;

int sec2 = 1;
int sec3;

//Should put this in a list ayy
NumConveyor numConvey1;
NumConveyor numConvey2;
NumConveyor numConvey3;
NumConveyor numConvey4;

NumConveyor setConvey1;
NumConveyor setConvey2;
NumConveyor setConvey3;
NumConveyor setConvey4;

//BrightnessProfile Stuff
BrigtnessContainer BProfile[]
{
	{ true,{ 8,0 },20 },
	{ true,{ 9,0 }, 0 },
	{ false,{ 0,0 },0 },
	{ false,{ 0,0 },0 },
	{ false,{ 0,0 },0 },
	{ false,{ 0,0 },0 },
	{ false,{ 0,0 },0 }, //Back button, just in case.
};
int cursorPos = 0;
int brightnessGuage = 2;
int brightnessLevel = 2;

//Time Stuff
bool hourBelow10;
bool redded = false;
int fakeTime = 0;

//Nap timer stuff
HrMn napTimer;
bool napOn = false;
int pastSec;
int napTimerSeconds;

//Per day alarm
AlarmContainer Alarms[]
{
	{ false,{ 0,0 } },
	{ false,{ 0,0 } },
	{ false,{ 0,0 } },
	{ false,{ 0,0 } },
	{ false,{ 0,0 } },
	{ false,{ 0,0 } },
	{ false,{ 0,0 } },
};

int fadeMod = 101;
int cursorFadeTimer;
int prevCursorPos;
bool fadeDirection = true; //true = up, false = down

void setup()
{
	matrix.begin();
	matrix.setTextWrap(false);

	matrix.setBrightness(50);
	Serial.begin(115200);
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

	EEPROM.get(10, BProfile);      
	EEPROM.get(10 + sizeof(BProfile), Alarms);

	if (BProfile == 0)
	{
		//TODO: Setup default Bprofile state if EEPROM is empty
	}

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

void NumConveyor::Update(int numTarget, int x, int speed, uint16_t colour, efontSize size)
{
	target = numTarget * 11;

	/*
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
	*/

	for (int i = 0; i < 10; i++)
		displayNum(i, x, i * 11 - ymod, colour, size);

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
}

//
///
////
eState State = DisplayTest;
////
///
//

class MenuContainer
{
public:

	int posList;
	int posActual;
	void(*f)(int, uint16_t);
	void(*f2)(int, uint16_t, uint16_t);
	void(*f3)();
	eState state;
	bool normalCase;
	uint16_t col1;
	uint16_t col2;

	int type;

	MenuContainer(int _pos, void(*_f)(int, uint16_t), eState _state)
	{
		type = 1; //Single Colour

		posList = _pos;
		posActual = _pos * 12;
		f = _f;
		state = _state;
		normalCase = true;

		if (posList % 2 == 0)
			col1 = colors[displayCol1];
		else
			col1 = colors[displayCol2];
	}

	MenuContainer(int _pos, void(*_f)(int, uint16_t, uint16_t), eState _state)
	{
		type = 2; //Bi-Colour

		posList = _pos;
		posActual = _pos * 12;
		f2 = _f;
		state = _state;
		normalCase = true;

		if (posList % 2 == 0)
		{
			col1 = colors[displayCol1];
			col2 = colors[displayCol2];
		}
		else
		{
			col1 = colors[displayCol2];
			col2 = colors[displayCol1];
		}
	}

	MenuContainer(int _pos, void(*_f)(), eState _state)
	{
		type = 3; //Time

		posList = _pos;
		posActual = _pos * 12;
		f3 = _f;
		state = _state;
		normalCase = false;
	}

	void Update()
	{
		switch (type)
		{
		case 1:
			(*f)(posActual, col1);
			break;

		case 2:
			(*f2)(posActual, col1, col2);
			break;

		case 3:
			(*f3)();
			break;
		}
	}
};

//Teeeemp

//

void loop()
{
	matrix.fillScreen(0);

	SetLight();

	//Alarm
	for (int i = 0; i < 7; i++)
	{
		if (day() == i && Alarms[i].active && Alarms[i].time.Hr == hour() && Alarms[i].time.Mn == minute() && second() == 0)
		{
			State = Alarm;
			textCursor = 0;
			SetBrightness(2);
		}
	}

	//Nap
	if (napOn)
	{
		if (napTimerSeconds == 0)
		{
			State = Alarm;
			napOn = false;
		}
		else
		{
			if (second() != pastSec)
			{
				pastSec = second();
				napTimerSeconds -= 1;
			}
		}
	}

	//Brightness
	for (int i = 0; i < 6; i++)
	{
		if (BProfile[i].active && BProfile[i].time.Hr == hour() && BProfile[i].time.Mn == minute() && second() == 0)
		{
			SetBrightness(BProfile[i].level);
			if (BProfile[i].level == 6)
				redColours();
			else
				unRedColours();
		}
	}

	switch (State)
	{
	case Main:
	{
		switch (brightnessGuage)
		{
		case 7:
		{
			matrix.fillScreen(0);

			if (buttonPressed())
				State = Brightness;
		}
		break;

		default:
		{
			UpdateTime();
			menu();

			if (buttonPressed())
				menuPageChange();
		}
		break;
		}
	}
	break;

	case Brightness: //Note: Delay is in function
	{
		SetBrightness(BrightnessReturn(true));
		oke();
	}
	break;

	case BrightnessProfile:
	{
		brightnessProfile();
	}
	break;

	case Alarm:
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
	break;

	case AlarmMenu:
	{
		alarmMenu();

		if (buttonPressed())
		{
			AlarmPageChange();
		}

		delay(100);
	}
	break;

	case SetAlarm:
	{
		//AlarmSet();
		perDayAlarm();
	}
	break;

	case ChangeColor:
	{
		if (hourBelow10)
			minuteAlert(displayCol1, 0);

		//UpdateTime();
		changeColor();
		delay(10);
	}
	break;

	case TimeSetMode:
	{
		TimeSet();
	}
	break;

	case Snake:
	{
		snake();
	}
	break;

	case SnakeGameOver:
	{
		displayNum(score / 10 % 10, 1, 0, colors[displayCol1], Big);
		displayNum(score % 10, 4, 0, colors[displayCol2], Big);

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
	break;

	case DisplayTest:
	{
		currentBrightness = 30;
		Date test;
		test = DateReturn();
	}
	break;

	case StaticDisplay:
	{
		staticDisplay();
	}
	break;

	case NapSet:
	{
		napTimer = TimeSetReturn(true);
		napOn = true;
		napTimerSeconds = napTimer.Mn * 60;
		pastSec = second();
		oke();
	}
	break;
	}

	matrix.show();

	if (digitalRead(2) == LOW)
		canPress = true;
}
//

int force(int num, int min, int max)
{
	if (num < min)
	{
		return min;
	}

	if (num > max)
	{
		return max;
	}
}

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

void redColours()
{
	for (int i = 0; i < 11; i++)
	{
		colors[i] = matrix.Color(255, 0, 0);
	}
}

void unRedColours()
{
	for (int i = 0; i < 11; i++)
	{
		colors[i] = colorsStore[i];
	}
}

int ColPos = 0;

void changeColor()
{
	//Check if this works ayyy lmao
	HrMn t = { hour(),minute()};
	displayTimeSimple(t, Big, true, false);

	if (hourBelow10)
		minuteAlert(displayCol1, 0);

	//DisplayCurrentTime(hrDisplay1, hrDisplay2, mnDisplay1, mnDisplay2, false, 2);
	menuTime();

	//Code will continually think the rotary encoder is turning left unless serial.print or delay(1) is used. Wtf.
	Serial.println(ColorToChoose);
	delay(1);

	ColorToChoose = ReadRotary(ColorToChoose, 1, 10);

	if (ColPos == 0)
		displayCol2 = ColorToChoose;

	if (ColPos == 1)
		displayCol1 = ColorToChoose;

	if (buttonPressed())
	{
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
		ColPos = 0;
		oke();
		EEPROM.write(2, displayCol2);
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

int okeX = -7;
void oke()
{
	while (okeX < 0)
	{
		matrix.fillScreen(0);
		delay(50);

		okeX++;

		matrix.drawPixel(2 + okeX, 4, colors[2]);
		matrix.drawLine(3 + okeX, 5, 5 + okeX, 3, colors[2]);

		matrix.show();
	}

	delay(1000);
	okeX = -7;
	State = Main;
}

void oke(bool returnToMain)
{
	while (okeX < 0)
	{
		matrix.fillScreen(0);
		delay(50);

		okeX++;

		matrix.drawPixel(2 + okeX, 4, colors[2]);
		matrix.drawLine(3 + okeX, 5, 5 + okeX, 3, colors[2]);

		matrix.show();
	}

	delay(1000);
	okeX = -7;
	if (returnToMain)
		State = Main;
}

//Not really on half second, but makes a cool blink effect
unsigned long previousMillis;
bool halfSecondBlink;
boolean onHalfSecond()
{
	unsigned long currentMillis = millis();
	if (currentMillis - previousMillis >= 500)
	{
		previousMillis = currentMillis;

		if (halfSecondBlink)
			halfSecondBlink = false;
		else
			halfSecondBlink = true;

		return halfSecondBlink;
	}
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

//Loops
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

//Doesn't loop
int ReadRotary(int varToChange, bool invert)
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
			if(!invert)
				return varToChange += 1;
			else
				return varToChange -= 1;
		}

		if ((newPosition / 4) < (oldPosition / 4))
		{
			Serial.println("Right");
			oldPosition = newPosition;
			if (!invert)
				return varToChange -= 1;
			else
				return varToChange += 1;
		}
	}
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
				while (foodx != snakeBodyX.get(i))
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
	int inputDirMod = ReadRotary(inputDirMod, false);

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