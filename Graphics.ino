//Graphics functions like icons and numbers
#pragma region	numberzzz

void displayNum(int num, int x, int y, uint16_t col, bool big)
{
	if (big)
	{
		switch (num)
		{
		case 1:
			num1Big(x, y, col);
			break;

		case 2:
			num2Big(x, y, col);
			break;

		case 3:
			num3Big(x, y, col);
			break;

		case 4:
			num4Big(x, y, col);
			break;

		case 5:
			num5Big(x, y, col);
			break;

		case 6:
			num6Big(x, y, col);
			break;

		case 7:
			num7Big(x, y, col);
			break;

		case 8:
			num8Big(x, y, col);
			break;

		case 9:
			num9Big(x, y, col);
			break;

		case 0:
			num0Big(x, y, col);
			break;
		}
	}
	else
	{
		switch (num)
		{
		case 1:
			num1(x, y, col);
			break;

		case 2:
			num2(x, y, col);
			break;

		case 3:
			num3(x, y, col);
			break;

		case 4:
			num4(x, y, col);
			break;

		case 5:
			num5(x, y, col);
			break;

		case 6:
			num6(x, y, col);
			break;

		case 7:
			num7(x, y, col);
			break;

		case 8:
			num8(x, y, col);
			break;

		case 9:
			num9(x, y, col);
			break;

		case 0:
			num0(x, y, col);
			break;
		}
	}
}

#pragma region	Small
void num0(int x, int y, uint16_t col)
{
	matrix.drawRect(0 + x, 0 + y, 2, 5,col);
}

void num1(int x, int y, uint16_t col)
{
	matrix.drawLine(1 + x, 0 + y, 1 + x, 5 + y, col);
}
void num2(int x, int y, uint16_t col) //
{
	matrix.drawLine(1 + x, 0 + y, 1 + x, 2 + y, col);
	matrix.drawLine(0 + x, 2 + y, 0 + x, 4 + y, col);
	matrix.drawPixel(0 + x, 0 + y, col);
	matrix.drawPixel(1 + x, 4 + y, col);
}

void num3(int x, int y, uint16_t col)
{
	matrix.drawLine(1 + x, 0 + y, 1 + x, 4 + y, col);
	matrix.drawPixel(0 + x, 0 + y, col);
	matrix.drawPixel(0 + x, 2 + y, col);
	matrix.drawPixel(0 + x, 4 + y, col);
}

void num4(int x, int y, uint16_t col)
{
	matrix.drawLine(0 + x, 0 + y, 0 + x, 2 + y, col);
	matrix.drawLine(1 + x, 2 + y, 1 + x, 4 + y, col);
}

void num5(int x, int y, uint16_t col)
{
	matrix.drawLine(0 + x, 0 + y, 0 + x, 2 + y, col);
	matrix.drawLine(1 + x, 2 + y, 1 + x, 4 + y, col);
	matrix.drawPixel(1 + x, 0 + y, col);
	matrix.drawPixel(0 + x, 4 + y, col);
}

void num6(int x, int y, uint16_t col)
{
	matrix.drawLine(0 + x, 0 + y, 0 + x, 4 + y, col);
	matrix.drawLine(1 + x, 2 + y, 1 + x, 4 + y, col);
	matrix.drawLine(0 + x, 0 + y, 0 + x, 1 + y, col);
	matrix.drawPixel(1 + x, 0 + y, col);
}

void num7(int x, int y, uint16_t col)
{
	matrix.drawLine(1 + x, 0 + y, 1 + x, 4 + y, col);
	matrix.drawPixel(0 + x, 0 + y, col);
}

void num8(int x, int y, uint16_t col)
{
	matrix.drawRect(0 + x, 0 + y, 2, 2, col);
	matrix.drawRect(0 + x, 3 + y, 2, 2, col);
}

void num9(int x, int y, uint16_t col)
{
	matrix.drawLine(1 + x, 0 + y, 1 + x, 4 + y, col);
	matrix.drawLine(0 + x, 0 + y, 0 + x, 2 + y, col);
}
#pragma endregion

#pragma region	Big
void num0Big(int x, int y, uint16_t col)
{
	matrix.drawLine(0 + x, 0, 0 + x, 7, col);
	matrix.drawLine(1 + x, 0, 1 + x, 7, col);

	//matrix.drawLine(0 + x, 0, 1 + x, 0, col);
	//matrix.drawLine(0 + x, 7, 1 + x, 7, col);
}

void num1Big(int x, int y, uint16_t col)
{
	if (x > 4)
		matrix.drawLine(1 + x, 0, 1 + x, 7, col);
	else
		matrix.drawLine(0 + x, 0, 0 + x, 7, col);
}

void num2Big(int x, int y, uint16_t col)
{
	matrix.drawLine(0 + x, 0, 1 + x, 0, col);
	matrix.drawLine(1 + x, 0, 1 + x, 4, col);
	matrix.drawLine(0 + x, 4, 0 + x, 7, col);
	matrix.drawPixel(1 + x, 7, col);
}

void num3Big(int x, int y, uint16_t col)
{
	matrix.drawLine(0 + x, 0, 1 + x, 0, col);
	matrix.drawLine(1 + x, 0, 1 + x, 7, col);
	matrix.drawLine(0 + x, 4, 0 + x, 4, col);
	matrix.drawLine(0 + x, 7, 1 + x, 7, col);
}

void num4Big(int x, int y, uint16_t col)
{
	matrix.drawLine(0 + x, 0, 0 + x, 4, col);
	matrix.drawLine(1 + x, 4, 1 + x, 7, col);
}

void num5Big(int x, int y, uint16_t col)
{
	matrix.drawLine(0 + x, 0, 1 + x, 0, col);
	matrix.drawLine(0 + x, 0, 0 + x, 4, col);
	matrix.drawLine(1 + x, 4, 1 + x, 7, col);
	matrix.drawPixel(0 + x, 7, col);
}

void num6Big(int x, int y, uint16_t col)
{
	matrix.drawLine(0 + x, 0, 1 + x, 0, col);
	matrix.drawLine(0 + x, 0, 0 + x, 7, col);
	matrix.drawLine(1 + x, 4, 1 + x, 7, col);
}

void num7Big(int x, int y, uint16_t col)
{
	matrix.drawLine(1 + x, 0, 1 + x, 7, col);
	matrix.drawPixel(0 + x, 0, col);
}

void num8Big(int x, int y, uint16_t col)
{
	matrix.drawRect(0 + x, 0, 2, 3, col);
	matrix.drawRect(0 + x, 4, 2, 4, col);
}

void num9Big(int x, int y, uint16_t col)
{
	matrix.drawLine(0 + x, 0, 0 + x, 4, col);
	matrix.drawLine(1 + x, 0, 1 + x, 7, col);
}
#pragma endregion

#pragma endregion

int colSwitch1;
int colSwitch2;

int timer3 = 0;
bool timerSwitch = false;
void colourIcon(int startPos)
{
	//Serial.println(timer3);
	if (timerSwitch)
		timer3 += deltaTime3();

	if (timer3 > 1000)
	{
		srandom(millis());
		int rnd;
		rnd = random(1, 4);
		colSwitch1 += rnd;

		//Serial.println("Random: ");
		//Serial.println(rnd);

		rnd = random(1, 4);
		colSwitch2 += rnd;
		//Serial.println("Random: ");
		//Serial.println(rnd);

		//Serial.println();
		//Serial.print("ColSwitch1:");
		//Serial.println(colSwitch1);
		//Serial.println();
		//Serial.print("ColSwitch2:");
		//Serial.println(colSwitch2);

		timer3 = 0;
	}

	if (!timerSwitch)
	{
		colSwitch1 = displayCol1;
		colSwitch2 = displayCol2;

		timerSwitch = true;
	}

	if (colSwitch1 == colSwitch2)
		colSwitch2 += 5;

	if (colSwitch1 > 8)
	{
		colSwitch1 -= 8;
		//Serial.println("Ayy");
	}

	if (colSwitch2 > 8)
	{
		//Serial.println("Ayy");
		colSwitch2 -= 8;
	}

	int tempConveyor = conveyorBelt - startPos;

	matrix.fillRoundRect(tempConveyor + 1, 1, 6, 6, 1, colors[colSwitch1]);

	matrix.fillTriangle(tempConveyor + 2, 5, tempConveyor + 5, 2, tempConveyor + 2, 2, colors[colSwitch2]);

	matrix.drawLine(tempConveyor + 2, 1, tempConveyor + 5, 1, colors[colSwitch2]);
	matrix.drawLine(tempConveyor + 1, 2, tempConveyor + 1, 5, colors[colSwitch2]);
}

int timer2;
void speakerIcon(int startPos, bool play)
{
	if (play)
	{
		timer2 += deltaTime2();
		if (timer2 > 200)
		{
			moveNum--;
			timer2 = 0;
		}

		if (moveNum < -2)
		{
			moveNum = 3;
		}
	}

	int tempConveyor = conveyorBelt - startPos;

	if (moveNum > -1 || conveyorBelt != conveyorTarget)
	{
		matrix.drawLine(tempConveyor + moveNum, 2, tempConveyor + moveNum, 5, colors[displayCol1]);
	}

	if (moveNum > -2 || conveyorBelt != conveyorTarget)
	{
		matrix.drawPixel(tempConveyor + moveNum + 1, 1, colors[displayCol1]);
		matrix.drawPixel(tempConveyor + moveNum + 1, 6, colors[displayCol1]);
	}

	matrix.drawLine(tempConveyor + 3, 0, tempConveyor + 3, 7, colors[displayCol2]);
	matrix.drawLine(tempConveyor + 4, 1, tempConveyor + 4, 6, colors[displayCol2]);
	matrix.drawLine(tempConveyor + 5, 2, tempConveyor + 5, 5, colors[displayCol2]);
	matrix.drawRect(tempConveyor + 6, 3, tempConveyor + 2, 2, colors[displayCol2]);

	//Serial.println(moveNum);
}

void speakerIcon(bool play)
{
	if (play)
	{
		timer2 += deltaTime2();

		if (timer2 > 200)
		{
			moveNum--;
			timer2 = 0;
		}

		if (moveNum < -2)
		{
			moveNum = 3;
		}
	}

	if (moveNum > -1)
	{
		matrix.drawLine(moveNum, 2, moveNum, 5, colors[displayCol1]);
	}

	if (moveNum > -2)
	{
		matrix.drawPixel(moveNum + 1, 1, colors[displayCol1]);
		matrix.drawPixel(moveNum + 1, 6, colors[displayCol1]);
	}

	matrix.drawLine(3, 0, 3, 7, colors[displayCol2]);
	matrix.drawLine(4, 1, 4, 6, colors[displayCol2]);
	matrix.drawLine(5, 2, 5, 5, colors[displayCol2]);
	matrix.drawRect(6, 3, 2, 2, colors[displayCol2]);

	//Serial.println(moveNum);
}

void timeIcon(int startpos)
{
	//  int tempConveyor = conveyorBelt - 16;
	int tempConveyor = conveyorBelt - startpos;

	matrix.drawRoundRect(tempConveyor + 1, 1, 6, 6, 1, colors[displayCol1]);
	matrix.drawTriangle(tempConveyor + 3, 4, tempConveyor + 3, 3, tempConveyor + 4, 4, colors[displayCol1]);
}