//Displays the menu pages and conveyorbelts the icons
int menuVal;
int lastVal = 5;
int newPos;

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
	}

	if (menuPage > 4)
		menuPage = 4;

	if (menuPage < 0)
		menuPage = 0;

	Serial.println(conveyorBelt);

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

	delay(55);
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
	if (timer > 55)
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
