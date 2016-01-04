// Classes.h
#ifndef CLASSES_H
#define CLASSES_H
#include "Adafruit_GFX.h"
#include "Adafruit_NeoMatrix.h"
#include "Adafruit_NeoPixel.h"

struct HrMn
	{
		int Hr;
		int Mn;
	};

enum efontSize
{
	Small,
	Med,
	Big
};

enum eState
{
	Main,
	Alarm,

	AlarmMenu,

	Brightness,
	BrightnessProfile,

	Snake,
	SnakeGameOver,

	TimeSetMode,
	NapSet,
	SetAlarm,
	ChangeColor,

	DisplayTest,
	StaticDisplay,
};

class NumConveyor
{
	int belt, target;
	int ymod = 0;
	int speed;

	uint16_t col;

	long timer;

	int velocity;

public:
	void Update(int numTarget, int x, int speed, uint16_t colour, efontSize size);
};

struct BrigtnessContainer
{
	bool active;
	HrMn time;
	int level;
};
#endif