//The test mode used for photos and stuff
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
		DisplayTime(0, 4, 2, 0, Big);
		break;

	case 1:
		DisplayTime(1, 1, 3, 7, Big);
		break;

	case 2:
		DisplayTime(1, 3, 3, 7, Big);
		break;

	case 3:
		DisplayTime(0, 2, 4, 7, Big);
		break;

	case 4:
		DisplayTime(1, 2, 3, 1, Big);
		break;

	}
}