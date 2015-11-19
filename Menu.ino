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
