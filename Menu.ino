//Displays the menu pages and conveyorbelts the icons
int menuVal;
int lastVal = 5;
int newPos;
int lastPos = 60;
int menuSpeed = 50; //Lower is faster
int resetTimer;

//Zac's EZ menu system TM
//First parameter must be in order
MenuContainer menuList[] =
{
	MenuContainer(0,menuTime,Brightness),
	MenuContainer(1,speakerIconMenu,AlarmMenu),
	MenuContainer(2,bedIcon,NapSet),
	MenuContainer(3,colourIcon,ChangeColor),
	MenuContainer(4,pirranaMenu,Snake),
	MenuContainer(5,timeIcon,TimeSetMode),
	MenuContainer(6,backIcon,BrightnessProfile),
};

int menuSize = sizeof(menuList) / sizeof(menuList[0]);

void menu()
{	
	lastPos = (menuSize - 1) * 12;

	menuPage = ReadRotary(menuPage, false);

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

	//Reset Pos Timer (tried to use a delta time function but behaved weirdly)
	if (lastVal == menuPage && conveyorTarget != 0)
		resetTimer += 52;
	
	if (conveyorTarget == 0)
		resetTimer = 0;

	if (resetTimer > 10000) // 10 Seconds
	{
		conveyorTarget = 0;
		menuPage = 0;
	}

	conveyBelt(menuSpeed);

	for (int i = 0; i < menuSize; i++)	
		menuList[i].Update();

	delay(menuSpeed);
}

void menuPageChange()
{
	State = menuList[menuPage].state;
	conveyorBelt = -8;
}

long timer;
void conveyBelt(int speed)
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
}