
void staticDisplay()
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