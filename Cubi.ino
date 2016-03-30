//Cubi, a playful LED powered alarm clock.
//Zac Lucarelli 2k16

//So many libraries the Citadel is jelly
#include <Adafruit_GFX.h>
#include "Classes.h"
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#include <EEPROM.h>
#include <Time.h>
#include <Audio.h>
#include <Wire.h>
#include <DS3232RTC.h>
#include <SPI.h>
#include <SD.h>
#include <LinkedList.h>
#include <gamma.h>
#include <Encoder.h>

#pragma region Audio Connections
// GUItool: begin automatically generated code
AudioInputI2S            i2sIn;          //xy=246,183
AudioPlaySdWav           playWav1;       //xy=261,318
AudioMixer4              mixer1;         //xy=451,323
AudioMixer4              mixer2;         //xy=452,195;
AudioOutputI2S           i2sOut;         //xy=604,3246
AudioAnalyzeFFT1024      fft1024;         //xy=640,221
AudioConnection          patchCord1(i2sIn, 0, mixer2, 0);
AudioConnection          patchCord2(i2sIn, 1, mixer2, 1);
AudioConnection          patchCord3(playWav1, 0, mixer1, 0);
AudioConnection          patchCord4(playWav1, 1, mixer1, 1);
AudioConnection          patchCord5(mixer1, 0, i2sOut, 0);
AudioConnection          patchCord6(mixer1, 0, i2sOut, 1);
AudioConnection          patchCord7(mixer2, fft1024);
AudioControlSGTL5000     sgtl5000_1;     //xy=254,414
// GUItool: end automatically generated code

#pragma endregion

#pragma region Definitions
#define ENCODER_OPTIMIZE_INTERRUPTS
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

#define SDCARD_CS_PIN    10
#define SDCARD_MOSI_PIN  7
#define SDCARD_SCK_PIN   14
#pragma endregion

#pragma region Colours
//Default display colours
int displayCol1 = 9;
int displayCol2 = 3;

int ColorToChoose = 1;

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
#pragma endregion

Encoder rotary(5, 4);  //Rotary Encoder
long oldPosition = -999;

#pragma region TimeSet
int hr, mn;
int sec;

int hrDisplay1, hrDisplay2, mnDisplay1, mnDisplay2;

int temphrDisplay1, temphrDisplay2, tempmnDisplay1, tempmnDisplay2;

boolean HourMode24 = false;

int selectedNum = 0;
int val;

boolean canPress = true;
#pragma endregion

#pragma region Alarm
bool AlarmKill = false;

int AlarmTime[] = { 0, 0 };
int time[] = { 0, 0, 0, 0 };

bool bigMode = true;

int songToPlay;
int moveNum = 3;

int targetBrightness = 20;
int currentBrightness = 0;

int daylevel = 20;
#pragma endregion

#pragma region Snake
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
#pragma endregion

#pragma region File Count
File root;
int fileCount;
#pragma endregion

#pragma region Menu stuff
int conveyorBelt = 0;
int conveyorTarget = 0;
#pragma endregion

#pragma region Alarm Volume
float targetVolume = 1;
float volume = 0;

int sec2 = 1;
int sec3;
#pragma endregion

#pragma region Should put this in a list ayy
NumConveyor numConvey1;
NumConveyor numConvey2;
NumConveyor numConvey3;
NumConveyor numConvey4;

NumConveyor setConvey1;
NumConveyor setConvey2;
NumConveyor setConvey3;
NumConveyor setConvey4;
#pragma endregion

#pragma region BrightnessProfile Stuff
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
#pragma endregion

#pragma region Time Stuff
bool hourBelow10;
bool redded = false;
int fakeTime = 0;
#pragma endregion

#pragma region Nap timer stuff
int napTimerMin;
bool napOn = false;
int pastSec;
int napTimerSeconds;
#pragma endregion

#pragma region Per day alarm
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
#pragma endregion

#pragma region Spectrum stuff
const int myInput = AUDIO_INPUT_MIC;
float scale = 60.0;
float level[8];
int   shown[8];
#pragma endregion

#pragma region Date Scroll
int datePage;
#pragma endregion

void setup()
{
	matrix.begin();
	matrix.setBrightness(30);

#pragma region Audio
	AudioMemory(30);

	sgtl5000_1.enable();
	sgtl5000_1.inputSelect(myInput);
	sgtl5000_1.volume(0.5);

	mixer1.gain(0, 0.5);
	mixer1.gain(1, 0.5);
#pragma endregion

#pragma region PinMode
	pinMode(2, INPUT);  //Buton
	pinMode(3, OUTPUT); //Transistor
	pinMode(12, OUTPUT);
	//pinMode(13, INPUT); // RX pin audio shield, LED
#pragma endregion

#pragma region EEPROM startup
	/*
	EEPROM Memory allocation
	0 - null
	1 - DisplayCol1
	2 - DisplayCol2
	3 - AlarmTime1
	4 - AlarmTime2
	5 - AlarmIsSet
	6 - SongToPlay
	7 - PrevBrightness
	10- BrightnessProfileArray
	AlarmArray
	*/
	Serial.println("Reading from Eeprom");
	displayCol1 = EEPROM.read(1);
	displayCol2 = EEPROM.read(2);

	AlarmTime[0] = EEPROM.read(3);
	AlarmTime[1] = EEPROM.read(4);

	AlarmKill = EEPROM.read(5);
	songToPlay = EEPROM.read(6);
	SetBrightness(EEPROM.read(7)); //Remember prev brightness

	EEPROM.get(10, BProfile);
	EEPROM.get(10 + sizeof(BProfile), Alarms);

	if (BProfile == 0)
	{
		//TODO: Setup default Bprofile state if EEPROM is empty
	}

	Serial.println("Done reading from Eeprom!");
#pragma endregion

#pragma region SD card

	SPI.setMOSI(SDCARD_MOSI_PIN);
	SPI.setSCK(SDCARD_SCK_PIN);
	if (!(SD.begin(SDCARD_CS_PIN)))
	{
		// stop here, but print a message repetitively
		while (1) {
			Serial.println("Unable to access the SD card");
			delay(500);
		}
	}
#pragma endregion

#pragma region Snake
	snakeBodyX.add(3);
	snakeBodyX.add(3);

	snakeBodyY.add(1);
	snakeBodyY.add(2);

	dirList.add(Up);
	dirList.add(Right);
	dirList.add(Left);
	dirList.add(Down);
	dirList.add(Left);
#pragma endregion

#pragma region Directory Read SD card
	root = SD.open("/");
	countDirectory(root);
	fileCount -= 1;
	Serial.println("Files in dir:");
	Serial.print(fileCount);
#pragma endregion

	setSyncProvider(RTC.get);
}

void NumConveyor::Update(int numTarget, int x, int speed, uint16_t colour, efontSize size)
{
	target = numTarget * 11;

	//Draw all the numbers
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
eState State = Main;
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
	void(*f3)(int);
	eState state;
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
	}

	MenuContainer(int _pos, void(*_f)(int, uint16_t, uint16_t), eState _state)
	{
		type = 2; //Bi-Colour

		posList = _pos;
		posActual = _pos * 12;
		f2 = _f;
		state = _state;
	}

	MenuContainer(int _pos, void(*_f)(int), eState _state)
	{
		type = 3; //Time

		posList = _pos;
		posActual = _pos * 12;
		f3 = _f;
		state = _state;
	}

	void Update()
	{
		switch (type)
		{
			case 1:
				(*f)(posActual, col1);

				if (posList % 2 == 0)
					col1 = colors[displayCol1];
				else
					col1 = colors[displayCol2];
				break;

			case 2:
				(*f2)(posActual, col1, col2);

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
				break;

			case 3:
				(*f3)(posActual);
				break;
		}
	}
};

class Menu
{
public:
	int menuVal;
	int lastVal = 5;
	int newPos;
	int lastPos = 60;
	int menuSpeed = 45; //Lower is faster
	int resetTimer;
	int menuSize;
	int homePage;

	bool canBounce = false;
	int menuPage;

	//int conveyorBelt;
	//int conveyorTarget;

	MenuContainer* menuList;

	Menu(MenuContainer* _menuList, int _menuSpeed, int _size)
	{
		menuList = _menuList;
		menuSpeed = _menuSpeed;
		menuSize = _size;
	}

	Menu(MenuContainer* _menuList, int _menuSpeed, int _size, int _homePage)
	{
		menuList = _menuList;
		menuSpeed = _menuSpeed;
		menuSize = _size;
		homePage = _homePage;
		menuPage = homePage - 1;
		conveyorBelt = homePage * 12;
		conveyorTarget = homePage * 12;
	}

	void Update(int page)
	{
		lastPos = (menuSize - 1) * 12;

		if (page == 666)
			menuPage = ReadRotary(menuPage, false);
		else
			menuPage = page;

		//Bounce
		if (menuPage < 0 && canBounce)
		{
			conveyorTarget = -5;
			canBounce = false;
		}

		if (menuPage > menuSize - 1 && canBounce)
		{
			conveyorTarget = lastPos + 5;
			canBounce = false;
		}

		if (conveyorBelt < -4)
		{
			conveyorTarget = 0;
			menuPage = 0;
		}

		if (conveyorBelt > lastPos + 4)
		{
			conveyorTarget = lastPos;
		}

		if (menuPage > menuSize - 1)
			menuPage = menuSize - 1;

		if (menuPage < 0)
			menuPage = 0;

		if (lastVal != menuPage)
		{
			conveyorTarget = menuList[menuPage].posActual;

			lastVal = menuPage;
			resetTimer = 0;
		}

		//Reset Pos Timer
		if (lastVal == menuPage && menuPage != homePage && page == 666)
			resetTimer += 52;

		if (menuPage == homePage)
			resetTimer = 0;

		if (resetTimer > 10000) // 10 Seconds-ish
		{
			conveyorTarget = 0;
			menuPage = homePage;
		}

		ConveyBelt(menuSpeed);

		for (int i = 0; i < menuSize; i++)
			menuList[i].Update();

		delay(menuSpeed);
	}

	long timer;
	void ConveyBelt(int speed)
	{
		timer += deltaTime();
		if (timer > speed)
		{
			if (conveyorBelt < conveyorTarget)
				conveyorBelt += 1;

			if (conveyorBelt > conveyorTarget)
				conveyorBelt -= 1;

			if (conveyorBelt == conveyorTarget)
				canBounce = true;

			timer = 0;
		}

		conveyorBelt = conveyorBelt;
		conveyorTarget = conveyorTarget;
	}

	void PageChange()
	{
		if (menuList[menuPage].state != Null)
		{
			State = menuList[menuPage].state;
			conveyorBelt = -8;
		}
		else
			datePage += 1;
	}
};

MenuContainer MainMenuList[] =
{
	MenuContainer(0,dateScroll,Null),
	MenuContainer(1,menuTime,Brightness),
	MenuContainer(2,speakerIconMenu,AlarmMenu),
	MenuContainer(3,bedIcon,NapSet),
	MenuContainer(4,musicIcon,Party),
	MenuContainer(5,colourIcon,ChangeColor),
	MenuContainer(6,pirranaMenu,Snake),
	MenuContainer(7,timeIcon,TimeSetMode),
	MenuContainer(8,backIcon,BrightnessProfile),
};

Menu MainMenu(MainMenuList, 50, 9, 1);

MenuContainer PartyMenuList[3] =
{
	MenuContainer(0,menuTime,Party),
	MenuContainer(1,PartyMode,Party),
	MenuContainer(2,TempDisplay,Party)
};

Menu PartyMenu(PartyMenuList, 50, 3);

bool pageDirection = true; //True is up false is down
int scaleTimer = 0;
int lastScale = scale + 5;
int pageTimer;
int pageTarget;

void loop()
{
	matrix.fillScreen(0);
	SetLight();

	//Alarm 
	//Maybe use RTC functions instead?
	for (int i = 0; i < 7; i++)
	{
		if (!AlarmKill && weekday() == i + 1 && Alarms[i].active && Alarms[i].time.Hr == hour() && Alarms[i].time.Mn == minute() && second() == 0)
		{
			State = Alarm;
			SetBrightness(2); //normal Brightness
			unRedColours();
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
				napTimerMin = (napTimerSeconds / 60) + 1;
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
					MainMenu.Update(666);

					if (buttonPressed())
						MainMenu.PageChange();
					break;
				}
			}
		}
		break;

		case Brightness:
		{
			SetBrightness(BrightnessReturn(true));
			oke(0);
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
				volume += 0.002;

			Serial.print("Volume:");
			Serial.println(volume);

			mixer1.gain(0, volume);
			mixer1.gain(1, volume);

			PlayAlarm();
		}
		break;

		case AlarmMenu:
		{
			alarmMenu();
			if (buttonPressed()) AlarmPageChange();
			delay(10);
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
			delay(5);
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

				oke(2);
			}
		}
		break;

		case DisplayTest:
		{
			/*sunIcon(12, Yellow);
			delay(100);*/

			//uint16_t n = matrix.Color(0, 0, 0);
			//uint32_t i, j;
			//int x, y;
			//matrix.setBrightness(30);
			//for (i = 0; i < 256; i++) {
			//	for (j = 0; j < matrix.numPixels(); j++)
			//	{
			//		//x = j;
			//		matrix.setPixelColor(j, draw565to32(Wheel((j + i) & 255)));
			//		//matrix.drawPixel(j, y, draw565to32(Wheel((j + i) & 255)));
			//		//x++;
			//	}
			//	matrix.drawLine(0, 5, 0, 7, n);
			//	matrix.drawLine(1, 0, 1, 3, n);
		}
		break;

		case StaticDisplay:
		{
			staticDisplay();
		}
		break;

		case NapSet:
		{
			if (!napOn)
			{
				HrMn m;
				m = TimeSetReturn(true, { 0, 0 });
				napTimerMin = m.Mn;
				napOn = true;
				napTimerSeconds = napTimerMin * 60;
				pastSec = second();
				oke(0);
			}
			else
			{
				napOn = false;
				oke(0);
			}
		}
		break;

		case Party:
		{
			pageTimer += 51;
			if (pageTimer > 10000)
			{
				if (pageDirection)
					pageTarget += 1;
				else
					pageTarget -= 1;
				pageTimer = 0;

				if (pageTarget == 2)
					pageDirection = false;

				if (pageTarget == 0)
					pageDirection = true;
			}

			if (pageTarget == 1)
			{
				scale = ReadRotary(scale, 5.0, false);
				scale = constrain(scale, 0, 200);
			}

			UpdateTime();
			PartyMenu.Update(pageTarget);

			if (buttonPressed())
			{
				if (pageTarget != 0)
				{
					pageTarget = 0;
					pageDirection = true;
				}
				else
				{
					oke(2);
					pageTimer = 0;
				}
			}
		}
		break;
	}

	matrix.show();

	if (digitalRead(2) == LOW)
		canPress = true;
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

void changeColor() //Make scrolly in future
{
	HrMn t = { hour(),minute() };
	displayTimeSimple(t, Big, false, false);

	if (hourBelow10)
		minuteAlert(displayCol1, 0);

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
		oke(0);
		EEPROM.write(2, displayCol2);
	}

	delay(2);
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
//0 == Normal 1 == doesn't go to main 2 == shows no tick
void oke(int type)
{
	if (type == 0 || type == 1)
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
	}

	if (type == 0 || type == 2)
	{
		matrix.fillScreen(0);
		State = Main;
		MainMenu.menuPage = MainMenu.homePage;
		conveyorBelt = MainMenu.homePage * 12 - 5;
	}
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
			if (!invert)
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

//Multipler
int ReadRotary(int varToChange, float multipler, bool invert)
{
	long newPosition = rotary.read();
	if (newPosition != oldPosition)
	{
		if ((newPosition / 4) > (oldPosition / 4))
		{
			Serial.println("Left");
			oldPosition = newPosition;
			if (!invert)
				return varToChange += 1 * multipler;
			else
				return varToChange -= 1 * multipler;
		}

		if ((newPosition / 4) < (oldPosition / 4))
		{
			Serial.println("Right");
			oldPosition = newPosition;
			if (!invert)
				return varToChange -= 1 * multipler;
			else
				return varToChange += 1 * multipler;
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

int GetTemp()
{
	return RTC.temperature() / 4;
}

uint32_t draw565to32(uint16_t color)
{
	uint32_t bits = (uint32_t)color;
	uint32_t blue = bits & 0x001F;     // 5 bits blue
	uint32_t green = bits & 0x07E0;    // 6 bits green
	uint32_t red = bits & 0xF800;      // 5 bits red

									   // Return shifted bits with alpha set to 0xFF
	return (red << 8) | (green << 5) | (blue << 3) | 0xFF000000;
}

uint32_t Wheel(byte WheelPos)
{
	Serial.println(WheelPos);
	WheelPos = 255 - WheelPos;
	if (WheelPos < 85) {
		return matrix.Color(255 - WheelPos * 3, 0, WheelPos * 3);
	}
	else if (WheelPos < 170) {
		WheelPos -= 85;
		return matrix.Color(0, WheelPos * 3, 255 - WheelPos * 3);
	}
	else {
		WheelPos -= 170;
		return matrix.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
	}
}

void PartyMode(int ayy)
{
	if (fft1024.available())
	{
		level[0] = fft1024.read(7, 10);
		level[1] = fft1024.read(11, 15);
		level[2] = fft1024.read(16, 22);
		level[3] = fft1024.read(23, 32);
		level[4] = fft1024.read(33, 46);
		level[5] = fft1024.read(47, 66);
		level[6] = fft1024.read(67, 93);
		level[7] = fft1024.read(94, 131);
	}

	for (int i = 0; i < 8; i++)
	{
		int val = level[i] * scale;
		if (val > 8) val = 8;

		if (val >= shown[i])
		{
			shown[i] = val;
		}
		else
		{
			if (shown[i] > 0) shown[i] = shown[i] - 1;
			val = shown[i];
		}
		matrix.drawLine(conveyorBelt + i - 12, 8, conveyorBelt + i - 12, 8 - shown[i], colors[displayCol1]);
	}

	if (scale != lastScale)
	{
		scaleTimer = 2000;
		pageTimer = 0;
		lastScale = scale;
	}

	if (scaleTimer > 0 && scale == lastScale)
		scaleTimer -= 35;

	if (scaleTimer > 0)
	{
		matrix.drawPixel(7 + conveyorBelt - 12, 0, matrix.Color(
			map(scale, 0, 200, 0, 255),
			map(scale, 0, 200, 200, 255),
			0));
	}
	//delay(50);
}

int temp;
bool canGetTemp;
void TempDisplay(int cv, uint16_t col)
{
	if (onSecond() && canGetTemp && pageTarget != 1)
		temp = GetTemp();

	if (!onSecond())
		canGetTemp = true;

	int t1, t2;

	t1 = temp / 10 % 10;
	t2 = temp % 10;

	setConvey3.Update(t1, conveyorBelt + 0 - 22, 0, colors[displayCol1], Big);
	setConvey4.Update(t2, conveyorBelt + 2 - 22, 0, colors[displayCol2], Big);
	matrix.drawPixel(conveyorBelt + 5 - 22, 0, colors[displayCol1]);
}

void dateScroll(int offset)
{
	HrMn d;
	HrMnSplit ds;
	d.Mn = day();

	ds = split(d);

	DisplayDay(weekday() - 1, 1, conveyorBelt, 0, colors[displayCol1], colors[displayCol1], Small);

	//Yummy sphagetti alignment code
	if (ds.mn1 != 1) //Normal
	{
		if (ds.mn2 != 1)
		{
			displayNum(ds.mn1, conveyorBelt - offset, 3, colors[displayCol2], Small);
			displayNum(ds.mn2, conveyorBelt - offset + 3, 3, colors[displayCol2], Small);
		}
		else
		{
			displayNum(ds.mn1, conveyorBelt - offset, 3, colors[displayCol2], Small);
			displayNum(ds.mn2, conveyorBelt - offset + 2, 3, colors[displayCol2], Small);
		}
	}
	else //1st digit aligned
	{
		if (ds.mn2 != 1)
		{
			displayNum(ds.mn1, conveyorBelt - offset - 1, 3, colors[displayCol2], Small);
			displayNum(ds.mn2, conveyorBelt - offset + 2, 3, colors[displayCol2], Small);
		}
		else
		{
			displayNum(ds.mn1, conveyorBelt - offset - 1, 3, colors[displayCol2], Small);
			displayNum(ds.mn2, conveyorBelt - offset + 1, 3, colors[displayCol2], Small);
		}
	}
}