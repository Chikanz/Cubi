//Displays the menu pages and conveyorbelts the icons
int menuVal;
int lastVal = 5;
int newPos;
int lastPos = 60;
int menuSpeed = 50; //Lower is faster

int resetTimer;

/*
colourIcon(12);
speakerIcon(24, true);
timeIcon(36);
pirrana(48, conveyorBelt, 500);
backIcon(60);
*/

MenuContainer menuList[] =
{
	MenuContainer(0,menuTime,Brightness),
	MenuContainer(1,colourIcon,ChangeColor),
	MenuContainer(2,speakerIconMenu,MenuAlarm),
	MenuContainer(3,timeIcon,TimeSetMode),
	MenuContainer(4,pirranaMenu,Snake),
	MenuContainer(5,backIcon,Main),
};

void menu()
{	
	menuPage = ReadRotary(menuPage);

	//Bounce
	if (menuPage < 0 && canBounce)
	{
		conveyorTarget = -5;
		canBounce = false;		
	}

	if (menuPage > 5 && canBounce)
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

	if (menuPage > 5)
		menuPage = 5;

	if (menuPage < 0)
		menuPage = 0;

	if (lastVal != menuPage)
	{
		/**
		switch (menuPage)
		{

		case 0:
			//Time
			conveyorTarget = 0;
			break;

		case 1:
			//colourIcon();
			conveyorTarget = 12;
			break;

		case 2:
			//speakerIcon();
			conveyorTarget = 24;
			
			break;

		case 3:
			//timeIcon();
			conveyorTarget = 36;
			
			break;

		case 4:
			//piranna();
			conveyorTarget = 48;
		
			break;

		case 5:
			//backIcon()
			conveyorTarget = 60;
			break;
		}
		*/

		conveyorTarget = menuList[menuPage].posActual;

		lastVal = menuPage;
		resetTimer = 0;
	}

	//Reset Pos Timer (tried to use a delta time function but behaved weirdly)
	if (lastVal == menuPage && conveyorTarget != 0)
	{
		resetTimer += 52;
	}

	if (conveyorTarget == 0)
	{
		resetTimer = 0;
	}

	if (resetTimer > 10000) // 10 Seconds
	{
		conveyorTarget = 0;
		menuPage = 0;
	}

	conveyBelt(menuSpeed);

	for (int i = 0; i < 5; i++)
	{
		menuList[i].Update();
	}

	delay(menuSpeed);
}

void menuPageChange()
{
	/*switch (menuPage)
	{
	case 0:
		State = Brightness;
		break;

	case 1:
		State = ChangeColor;
		matrix.fillScreen(0);
		break;

	case 2:
		State = MenuAlarm;
		matrix.fillScreen(0);
		break;

	case 3:
		State = TimeSetMode;
		matrix.fillScreen(0);
		break;

	case 4:
		State = Snake;
		matrix.fillScreen(0);
		break;

	case 5:
		State = Main;
		matrix.fillScreen(0);
		break;
	}*/
	State = menuList[menuPage].state;

	//Reset belt
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
