//Graphics functions like icons and numbers
#pragma region	numberzzz

void displayNum(int num, int x, int y, uint16_t col, efontSize size)
{
	switch (size)
	{
		case Big:
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
			break;

		case Med:
			switch (num)
			{
				case 1:
					num1Med(x, y, col);
					break;

				case 2:
					num2Med(x, y, col);
					break;

				case 3:
					num3Med(x, y, col);
					break;

				case 4:
					num4Med(x, y, col);
					break;

				case 5:
					num5Med(x, y, col);
					break;

				case 6:
					num6Med(x, y, col);
					break;

				case 7:
					num7Med(x, y, col);
					break;

				case 8:
					num8Med(x, y, col);
					break;

				case 9:
					num9Med(x, y, col);
					break;

				case 0:
					num0Med(x, y, col);
					break;
			}
			break;

		case Small:
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
			break;
	}
}

#pragma region	Small
void num0(int x, int y, uint16_t col)
{
	matrix.drawRect(0 + x, 0 + y, 2, 5, col);
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

#pragma region	Med
void num0Med(int x, int y, uint16_t col)
{
	matrix.drawRect(0 + x, 0 + y, 2, 7, col);
}

void num1Med(int x, int y, uint16_t col)
{
	if (x > 4)
		matrix.drawLine(1 + x, 0 + y, 1 + x, 6 + y, col);
	else
		matrix.drawLine(0 + x, 0 + y, 0 + x, 6 + y, col);
}

void num2Med(int x, int y, uint16_t col)
{
	matrix.drawLine(0 + x, 0 + y, 1 + x, 0 + y, col);
	matrix.drawLine(1 + x, 0 + y, 1 + x, 3 + y, col);
	matrix.drawLine(0 + x, 3 + y, 0 + x, 6 + y, col);
	matrix.drawPixel(1 + x, 6 + y, col);
}

void num3Med(int x, int y, uint16_t col)
{
	matrix.drawLine(0 + x, 0 + y, 1 + x, 0 + y, col);
	matrix.drawLine(1 + x, 0 + y, 1 + x, 6 + y, col);
	matrix.drawLine(0 + x, 3 + y, 0 + x, 3 + y, col);
	matrix.drawLine(0 + x, 6 + y, 1 + x, 6 + y, col);
}

void num4Med(int x, int y, uint16_t col)
{
	matrix.drawLine(0 + x, 0 + y, 0 + x, 3 + y, col);
	matrix.drawLine(1 + x, 3 + y, 1 + x, 6 + y, col);
}

void num5Med(int x, int y, uint16_t col)
{
	matrix.drawLine(0 + x, 0 + y, 1 + x, 0 + y, col);
	matrix.drawLine(0 + x, 0 + y, 0 + x, 3 + y, col);
	matrix.drawLine(1 + x, 3 + y, 1 + x, 6 + y, col);
	matrix.drawPixel(0 + x, 6 + y, col);
}

void num6Med(int x, int y, uint16_t col)
{
	matrix.drawLine(0 + x, 0 + y, 1 + x, 0 + y, col);
	matrix.drawLine(0 + x, 0 + y, 0 + x, 6 + y, col);
	matrix.drawLine(1 + x, 3 + y, 1 + x, 6 + y, col);
}

void num7Med(int x, int y, uint16_t col)
{
	matrix.drawLine(1 + x, 0 + y, 1 + x, 6 + y, col);
	matrix.drawPixel(0 + x, 0 + y, col);
}

void num8Med(int x, int y, uint16_t col)
{
	matrix.drawRect(0 + x, 0 + y, 2, 2, col);
	matrix.drawRect(0 + x, 3 + y, 2, 4, col);
}

void num9Med(int x, int y, uint16_t col)
{
	matrix.drawLine(0 + x, 0 + y, 0 + x, 4 + y, col);
	matrix.drawLine(1 + x, 0 + y, 1 + x, 6 + y, col);
}
#pragma endregion

#pragma region	Big
void num0Big(int x, int y, uint16_t col)
{
	matrix.drawLine(0 + x, 0 + y, 0 + x, 7 + y, col);
	matrix.drawLine(1 + x, 0 + y, 1 + x, 7 + y, col);

	//matrix.drawLine(0 + x, 0, 1 + x, 0, col);
	//matrix.drawLine(0 + x, 7, 1 + x, 7, col);
}

void num1Big(int x, int y, uint16_t col)
{
	if (x > 4)
		matrix.drawLine(1 + x, 0 + y, 1 + x, 7 + y, col);
	else
		matrix.drawLine(0 + x, 0 + y, 0 + x, 7 + y, col);
}

void num2Big(int x, int y, uint16_t col)
{
	matrix.drawLine(0 + x, 0 + y, 1 + x, 0 + y, col);
	matrix.drawLine(1 + x, 0 + y, 1 + x, 4 + y, col);
	matrix.drawLine(0 + x, 4 + y, 0 + x, 7 + y, col);
	matrix.drawPixel(1 + x, 7 + y, col);
}

void num3Big(int x, int y, uint16_t col)
{
	matrix.drawLine(0 + x, 0 + y, 1 + x, 0 + y, col);
	matrix.drawLine(1 + x, 0 + y, 1 + x, 7 + y, col);
	matrix.drawLine(0 + x, 4 + y, 0 + x, 4 + y, col);
	matrix.drawLine(0 + x, 7 + y, 1 + x, 7 + y, col);
}

void num4Big(int x, int y, uint16_t col)
{
	matrix.drawLine(0 + x, 0 + y, 0 + x, 4 + y, col);
	matrix.drawLine(1 + x, 4 + y, 1 + x, 7 + y, col);
}

void num5Big(int x, int y, uint16_t col)
{
	matrix.drawLine(0 + x, 0 + y, 1 + x, 0 + y, col);
	matrix.drawLine(0 + x, 0 + y, 0 + x, 4 + y, col);
	matrix.drawLine(1 + x, 4 + y, 1 + x, 7 + y, col);
	matrix.drawPixel(0 + x, 7 + y, col);
}

void num6Big(int x, int y, uint16_t col)
{
	matrix.drawLine(0 + x, 0 + y, 1 + x, 0 + y, col);
	matrix.drawLine(0 + x, 0 + y, 0 + x, 7 + y, col);
	matrix.drawLine(1 + x, 4 + y, 1 + x, 7 + y, col);
}

void num7Big(int x, int y, uint16_t col)
{
	matrix.drawLine(1 + x, 0 + y, 1 + x, 7 + y, col);
	matrix.drawPixel(0 + x, 0 + y, col);
}

void num8Big(int x, int y, uint16_t col)
{
	matrix.drawRect(0 + x, 0 + y, 2, 3, col);
	matrix.drawRect(0 + x, 4 + y, 2, 4, col);
}

void num9Big(int x, int y, uint16_t col)
{
	matrix.drawLine(0 + x, 0 + y, 0 + x, 4 + y, col);
	matrix.drawLine(1 + x, 0 + y, 1 + x, 7 + y, col);
}
#pragma endregion

#pragma endregion

#pragma region Icons
int colSwitch1;
int colSwitch2;

int timer3 = 0;
bool timerSwitch = false;
void colourIcon(int startPos, uint16_t col)
{
	if (timerSwitch)
		timer3 += deltaTime3();

	if (timer3 > 1000)
	{
		srandom(millis());
		int rnd;
		rnd = random(1, 4);
		colSwitch1 += rnd;

		rnd = random(1, 4);
		colSwitch2 += rnd;

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
void speakerIconMenu(int startPos, uint16_t col2, uint16_t col1)
{
	if (true)
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
		matrix.drawLine(tempConveyor + moveNum, 2, tempConveyor + moveNum, 5, col1);
	}

	if (moveNum > -2 || conveyorBelt != conveyorTarget)
	{
		matrix.drawPixel(tempConveyor + moveNum + 1, 1, col1);
		matrix.drawPixel(tempConveyor + moveNum + 1, 6, col1);
	}

	matrix.drawLine(tempConveyor + 3, 0, tempConveyor + 3, 7, col2);
	matrix.drawLine(tempConveyor + 4, 1, tempConveyor + 4, 6, col2);
	matrix.drawLine(tempConveyor + 5, 2, tempConveyor + 5, 5, col2);
	matrix.drawRect(tempConveyor + 6, 3, tempConveyor + 2, 2, col2);

	//Serial.println(moveNum);
}

void speakerIcon(bool play, bool invert)
{
	uint16_t col1, col2;

	if (!invert)
	{
		col1 = displayCol1;
		col2 = displayCol2;
	}
	else
	{
		col1 = displayCol2;
		col2 = displayCol1;
	}

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
		matrix.drawLine(moveNum, 2, moveNum, 5, colors[col1]);
	}

	if (moveNum > -2)
	{
		matrix.drawPixel(moveNum + 1, 1, colors[col1]);
		matrix.drawPixel(moveNum + 1, 6, colors[col1]);
	}

	matrix.drawLine(3, 0, 3, 7, colors[col2]);
	matrix.drawLine(4, 1, 4, 6, colors[col2]);
	matrix.drawLine(5, 2, 5, 5, colors[col2]);
	matrix.drawRect(6, 3, 2, 2, colors[col2]);

	//Serial.println(moveNum);
}

void timeIcon(int startpos, uint16_t col)
{
	//  int tempConveyor = conveyorBelt - 16;
	int tempConveyor = conveyorBelt - startpos;

	matrix.drawRoundRect(tempConveyor + 1, 1, 6, 6, 1, colors[displayCol1]);
	matrix.drawTriangle(tempConveyor + 3, 4, tempConveyor + 3, 3, tempConveyor + 4, 4, colors[displayCol1]);
}

void pirrana(int startpos, int convey, int timer)
{
	timerTemp += deltaTime4();

	if (timerTemp > timer)
	{
		if (mouth)
		{
			mouth = false;
		}
		else
		{
			mouth = true;
		}

		timerTemp = 0;
	}

	int tempConveyor = convey - startpos;

	if (mouth)
	{
		//Stem
		matrix.drawLine(tempConveyor + 2, 7, tempConveyor + 4, 7, colors[2]);
		matrix.drawLine(tempConveyor + 3, 5, tempConveyor + 3, 7, colors[2]);
		matrix.drawPixel(tempConveyor + 1, 6, colors[2]);
		matrix.drawPixel(tempConveyor + 5, 6, colors[2]);

		//Head
		matrix.drawLine(tempConveyor + 2, 4, tempConveyor + 4, 4, colors[displayCol1]);
		matrix.drawTriangle(tempConveyor + 1, 2, tempConveyor + 1, 3, tempConveyor + 2, 3, colors[displayCol1]);
		matrix.drawTriangle(tempConveyor + 5, 2, tempConveyor + 5, 3, tempConveyor + 4, 3, colors[displayCol1]);

		matrix.drawLine(tempConveyor + 1, 1, tempConveyor + 3, 3, colors[10]);
		matrix.drawLine(tempConveyor + 4, 2, tempConveyor + 5, 1, colors[10]);
	}
	else
	{
		//Stem
		matrix.drawLine(tempConveyor + 2, 7, tempConveyor + 4, 7, colors[2]);
		matrix.drawPixel(tempConveyor + 1, 6, colors[2]);
		matrix.drawPixel(tempConveyor + 5, 6, colors[2]);
		matrix.drawLine(tempConveyor + 3, 5, tempConveyor + 3, 7, colors[2]);

		//Head
		matrix.drawLine(tempConveyor + 2, 4, tempConveyor + 4, 4, colors[displayCol1]);
		matrix.drawLine(tempConveyor + 1, 2, tempConveyor + 1, 3, colors[displayCol1]);
		matrix.drawLine(tempConveyor + 2, 1, tempConveyor + 2, 4, colors[displayCol1]);

		matrix.drawLine(tempConveyor + 4, 1, tempConveyor + 4, 4, colors[displayCol1]);
		matrix.drawLine(tempConveyor + 5, 2, tempConveyor + 5, 3, colors[displayCol1]);

		matrix.drawLine(tempConveyor + 3, 1, tempConveyor + 3, 3, colors[10]);
	}
}

void pirranaMenu(int startpos, uint16_t col)
{
	timerTemp += deltaTime4();

	if (timerTemp > 500)
	{
		if (mouth)
			mouth = false;
		else
			mouth = true;
		timerTemp = 0;
	}

	int tempConveyor = conveyorBelt - startpos;

	if (mouth)
	{
		//Stem
		matrix.drawLine(tempConveyor + 2, 7, tempConveyor + 4, 7, colors[2]);
		matrix.drawLine(tempConveyor + 3, 5, tempConveyor + 3, 7, colors[2]);
		matrix.drawPixel(tempConveyor + 1, 6, colors[2]);
		matrix.drawPixel(tempConveyor + 5, 6, colors[2]);

		//Head
		matrix.drawLine(tempConveyor + 2, 4, tempConveyor + 4, 4, col);
		matrix.drawTriangle(tempConveyor + 1, 2, tempConveyor + 1, 3, tempConveyor + 2, 3, col);
		matrix.drawTriangle(tempConveyor + 5, 2, tempConveyor + 5, 3, tempConveyor + 4, 3, col);

		matrix.drawLine(tempConveyor + 1, 1, tempConveyor + 3, 3, colors[10]);
		matrix.drawLine(tempConveyor + 4, 2, tempConveyor + 5, 1, colors[10]);
	}
	else
	{
		//Stem
		matrix.drawLine(tempConveyor + 2, 7, tempConveyor + 4, 7, colors[2]);
		matrix.drawPixel(tempConveyor + 1, 6, colors[2]);
		matrix.drawPixel(tempConveyor + 5, 6, colors[2]);
		matrix.drawLine(tempConveyor + 3, 5, tempConveyor + 3, 7, colors[2]);

		//Head
		matrix.drawLine(tempConveyor + 2, 4, tempConveyor + 4, 4, col);
		matrix.drawLine(tempConveyor + 1, 2, tempConveyor + 1, 3, col);
		matrix.drawLine(tempConveyor + 2, 1, tempConveyor + 2, 4, col);

		matrix.drawLine(tempConveyor + 4, 1, tempConveyor + 4, 4, col);
		matrix.drawLine(tempConveyor + 5, 2, tempConveyor + 5, 3, col);

		matrix.drawLine(tempConveyor + 3, 1, tempConveyor + 3, 3, colors[10]);
	}
}

int backMove = 0;
int timer50;
void backIcon(int startpos, uint16_t col)
{
	timer50 += 51;

	if (timer50 > 500)
	{
		timer50 = 0;
		backMove--;
	}

	if (backMove < -1)
		backMove = 0;

	int tempConveyor = conveyorBelt - startpos;

	matrix.drawTriangle(tempConveyor + backMove + 2, 2, tempConveyor + backMove + 3, 1, tempConveyor + backMove + 3, 2, col);
	matrix.drawRect(tempConveyor + backMove + 1, 3, 6, 2, col);
	matrix.drawTriangle(tempConveyor + backMove + 2, 5, tempConveyor + backMove + 3, 5, tempConveyor + backMove + 3, 6, col);
}

void backIcon(int x, int y, uint16_t col, bool animated)
{
	if (animated)
		timer3 += deltaTime3();

	if (timer3 > 500)
	{
		timer3 = 0;
		backMove--;
	}

	if (backMove < -1)
		backMove = 0;

	matrix.drawTriangle(x + backMove + 2, y + 2, x + backMove + 3, y + 1, x + backMove + 3, y + 2, col);
	matrix.drawRect(x + backMove + 1, y + 3, 6, 2, col);
	matrix.drawTriangle(x + backMove + 2, y + 5, x + backMove + 3, y + 5, x + backMove + 3, y + 6, col);
}

void sunIcon(int startPos, uint16_t col)
{
	int tempConveyor = conveyorBelt - startPos;

	matrix.drawRect(tempConveyor + 1, 1, 6, 6, col);
	matrix.fillRect(tempConveyor + 3, 2, 3, 3, col);
}

void bedIcon(int startPos, uint16_t col1, uint16_t col2)
{
	int tempConveyor = conveyorBelt - startPos;

	matrix.drawLine(tempConveyor + 1, 5, tempConveyor + 6, 5, col1);
	matrix.drawPixel(tempConveyor + 1, 6, col1);
	matrix.drawPixel(tempConveyor + 6, 6, col1);
	matrix.drawPixel(tempConveyor + 6, 4, col1);

	matrix.drawLine(tempConveyor + 1, tempConveyor + 0, tempConveyor + 1, tempConveyor + 1, col2);
	matrix.drawLine(tempConveyor + 2, tempConveyor + 1, tempConveyor + 2, tempConveyor + 2, col2);
}
#pragma endregion

#pragma region Days Of The Week

void sunday(int x, int y, uint16_t col1, uint16_t col2)
{
	//S
	matrix.drawLine(x + 1, y + 0, x + 1, y + 2, col1);
	matrix.drawPixel(x + 2, y + 0, col1);
	matrix.drawPixel(x + 0, y + 2, col1);

	//U
	matrix.drawLine(x + 3, y + 0, x + 3, y + 2, col2);
	matrix.drawLine(x + 5, y + 0, x + 5, y + 2, col2);
	matrix.drawPixel(x + 4, y + 2, col2);
}

void monday(int x, int y, uint16_t col1, uint16_t col2)
{
	//M
	matrix.drawLine(x + 0, y + 0, x + 0, y + 2, col1);
	matrix.drawLine(x + 2, y + 0, x + 2, y + 2, col1);
	matrix.drawPixel(x + 1, y + 0, col1);

	//O
	matrix.drawRect(x + 3, y + 0, 3, 3, col2);
}

void tuesday(int x, int y, uint16_t col1, uint16_t col2)
{
	//T
	matrix.drawLine(x + 0, y + 0, x + 2, y + 0, col1);
	matrix.drawLine(x + 1, y + 1, x + 1, y + 2, col1);

	//U
	matrix.drawLine(x + 3, y + 0, x + 3, y + 2, col2);
	matrix.drawLine(x + 5, y + 0, x + 5, y + 2, col2);
	matrix.drawPixel(x + 4, y + 2, col2);
}

void wednesday(int x, int y, uint16_t col1, uint16_t col2)
{
	//"W"
	matrix.drawLine(x + 0, y + 0, x + 0, y + 2, col1);
	matrix.drawLine(x + 2, y + 0, x + 2, y + 2, col1);
	matrix.drawPixel(x + 1, y + 2, col1);

	//E
	matrix.drawRect(x + 3, y + 0, 2, 3, col2);
	matrix.drawPixel(x + 5, y + 0, col2);
	matrix.drawPixel(x + 5, y + 2, col2);
}

void thursday(int x, int y, uint16_t col1, uint16_t col2)
{
	//T
	matrix.drawLine(x + 0, y + 0, x + 2, y + 0, col1);
	matrix.drawLine(x + 1, y + 1, x + 1, y + 2, col1);

	//U
	matrix.drawLine(x + 3, y + 0, x + 3, y + 2, col2);
	matrix.drawLine(x + 5, y + 0, x + 5, y + 2, col2);
	matrix.drawPixel(x + 4, y + 1, col2);
}

void friday(int x, int y, uint16_t col1, uint16_t col2)
{
	//F
	matrix.drawTriangle(x + 0, y + 0, x + 0, y + 2, x + 2, y + 0, col1);

	//R
	matrix.drawTriangle(x + 3, y + 0, x + 3, y + 2, x + 5, y + 0, col2);
	matrix.drawPixel(x + 5, y + 2, col2);
}

void saturday(int x, int y, uint16_t col1, uint16_t col2)
{
	//S
	matrix.drawLine(x + 1, y + 0, x + 1, y + 2, col1);
	matrix.drawPixel(x + 2, y + 0, col1);
	matrix.drawPixel(x + 0, y + 2, col1);

	//A
	matrix.drawRect(x + 3, y + 0, 3, 2, col2);
	matrix.drawPixel(x + 3, y + 2, col2);
	matrix.drawPixel(x + 5, y + 2, col2);
}

#pragma endregion

#pragma region Months

//Together

void jan(int x, int y, uint16_t col1, uint16_t col2)
{
	j(x, y, col1);
	a(x + 3, y, col2);
}
void feb(int x, int y, uint16_t col1, uint16_t col2)
{
	f(x, y, col1);
	e(x + 3, y, col2);
}
void march(int x, int y, uint16_t col1, uint16_t col2)
{
	m(x, y, col1);
	a(x + 3, y, col2);
}
void april(int x, int y, uint16_t col1, uint16_t col2)
{
	a(x, y, col1);
	p(x + 3, y, col2);
}
void may(int x, int y, uint16_t col1, uint16_t col2)
{
	m(x, y, col1);
	a(x + 3, y, col2);
}
void june(int x, int y, uint16_t col1, uint16_t col2)
{
	j(x, y, col1);
	u(x + 3, y, col2);
}
void july(int x, int y, uint16_t col1, uint16_t col2) //Well fuck
{
	j(x, y, col1);
	u(x + 3, y, col2);
}
void august(int x, int y, uint16_t col1, uint16_t col2)
{
	a(x, y, col1);
	u(x + 3, y, col2);
}
void sep(int x, int y, uint16_t col1, uint16_t col2)
{
	s(x, y, col1);
	e(x + 3, y, col2);
}
void oct(int x, int y, uint16_t col1, uint16_t col2)
{
	o(x, y, col1);
	c(x + 3, y, col2);
}
void nov(int x, int y, uint16_t col1, uint16_t col2)
{
	n(x, y, col1);
	o(x + 3, y, col2);
}
void dec(int x, int y, uint16_t col1, uint16_t col2)
{
	d(x, y, col1);
	e(x + 3, y, col2);
}

//Apart
void jan(int x, int y, uint16_t col)
{
	j(x, y, col);
	a(x + 4, y, col);
}
void feb(int x, int y, uint16_t col)
{
	f(x, y, col);
	e(x + 4, y, col);
}
void march(int x, int y, uint16_t col)
{
	m(x, y, col);
	a(x + 4, y, col);
}
void april(int x, int y, uint16_t col)
{
	a(x, y, col);
	p(x + 4, y, col);
}
void may(int x, int y, uint16_t col)
{
	m(x, y, col);
	a(x + 4, y, col);
}
void june(int x, int y, uint16_t col)
{
	j(x, y, col);
	u(x + 4, y, col);
}
void july(int x, int y, uint16_t col) //Well fuck
{
	j(x, y, col);
	u(x + 4, y, col);
}
void august(int x, int y, uint16_t col)
{
	a(x, y, col);
	u(x + 4, y, col);
}
void sep(int x, int y, uint16_t col)
{
	s(x, y, col);
	e(x + 4, y, col);
}
void oct(int x, int y, uint16_t col)
{
	o(x, y, col);
	c(x + 4, y, col);
}
void nov(int x, int y, uint16_t col)
{
	n(x, y, col);
	o(x + 4, y, col);
}
void dec(int x, int y, uint16_t col)
{
	d(x, y, col);
	e(x + 4, y, col);
}

void displayMonth(int month)
{
	switch (month)
	{
		case 0:
			jan(0, 0, colors[displayCol1]);
			break;
		case 1:
			feb(0, 0, colors[displayCol1]);
			break;
		case 2:
			march(0, 0, colors[displayCol1]);
			break;
		case 3:
			april(0, 0, colors[displayCol1]);
			break;
		case 4:
			may(0, 0, colors[displayCol1]);
			break;
		case 5:
			june(0, 0, colors[displayCol1]);
			break;
		case 6:
			july(0, 0, colors[displayCol1]);
			break;
		case 7:
			august(0, 0, colors[displayCol1]);
			break;
		case 8:
			sep(0, 0, colors[displayCol1]);
			break;
		case 9:
			oct(0, 0, colors[displayCol1]);
			break;
		case 10:
			nov(0, 0, colors[displayCol1]);
			break;
		case 11:
			dec(0, 0, colors[displayCol1]);
			break;
	}
}

#pragma endregion

#pragma region Letters

void a(int x, int y, uint16_t col)
{
	matrix.drawRect(x + 0, y + 0, 3, 2, col);
	matrix.drawPixel(x + 0, y + 2, col);
	matrix.drawPixel(x + 2, y + 2, col);
}
//void b(int x, int y, uint16_t col);
void c(int x, int y, uint16_t col)
{
	drawRect(0, 0, 3, 3, x, y, col);
	drawPixel(2, 1, x, y, 0);
}
void d(int x, int y, uint16_t col)
{
	drawRect(0, 0, 3, 3, x, y, col);
	drawPixel(2, 0, x, y, 0);
}
void e(int x, int y, uint16_t col)
{
	matrix.drawRect(x + 0, y + 0, 2, 3, col);
	matrix.drawPixel(x + 2, y + 0, col);
	matrix.drawPixel(x + 2, y + 2, col);
}
void f(int x, int y, uint16_t col)
{
	matrix.drawTriangle(x + 0, y + 0, x + 0, y + 2, x + 2, y + 0, col);
}
//void g(int x, int y, uint16_t col);
//void i(int x, int y, uint16_t col);
void j(int x, int y, uint16_t col)
{
	drawRect(0, 0, 3, 3, x, y, col);
	drawLine(0, 0, 1, 0, x, y, 0);
}
//void k(int x, int y, uint16_t col);
//void l(int x, int y, uint16_t col);
void m(int x, int y, uint16_t col)
{
	matrix.drawLine(x + 0, y + 0, x + 0, y + 2, col);
	matrix.drawLine(x + 2, y + 0, x + 2, y + 2, col);
	matrix.drawPixel(x + 1, y + 0, col);
}
void n(int x, int y, uint16_t col)
{
	drawRect(0, 0, 3, 3, x, y, col);
	drawPixel(1, 2, x, y, 0);
}
void o(int x, int y, uint16_t col)
{
	matrix.drawRect(x + 0, y + 0, 3, 3, col);
}
void p(int x, int y, uint16_t col)
{
	drawRect(0, 0, 3, 2, x, y, col);
	drawPixel(0, 2, x, y, col);
}
//void q(int x, int y, uint16_t col);
void r(int x, int y, uint16_t col)
{
	matrix.drawTriangle(x + 0, y + 0, x + 0, y + 2, x + 2, y + 0, col);
	matrix.drawPixel(x + 2, y + 2, col);
}
void s(int x, int y, uint16_t col)
{
	matrix.drawLine(x + 1, y + 0, x + 1, y + 2, col);
	matrix.drawPixel(x + 2, y + 0, col);
	matrix.drawPixel(x + 0, y + 2, col);
}
void t(int x, int y, uint16_t col)
{
	matrix.drawLine(x + 0, y + 0, x + 2, y + 0, col);
	matrix.drawLine(x + 1, y + 1, x + 1, y + 2, col);
}
void u(int x, int y, uint16_t col)
{
	matrix.drawLine(x + 0, y + 0, x + 0, y + 2, col);
	matrix.drawLine(x + 2, y + 0, x + 2, y + 2, col);
	matrix.drawPixel(x + 1, y + 2, col);
}
//void v(int x, int y, uint16_t col);
//void w(int x, int y, uint16_t col);
//void x(int x, int y, uint16_t col);
//void y(int x, int y, uint16_t col);
//void z(int x, int y, uint16_t col);

#pragma endregion

#pragma region templates
void drawLine(int x1, int y1, int x2, int y2, int xmod, int ymod, uint16_t col)
{
	matrix.drawLine(xmod + x1, ymod + y1, xmod + x2, ymod + y2, col);
}

void drawPixel(int x1, int y1, int xmod, int ymod, uint16_t col)
{
	matrix.drawPixel(xmod + x1, ymod + y1, col);
}

void drawTriangle(int x0, int y0, int x1, int y1, int x2, int y2, int xmod, int ymod, uint16_t col)
{
	matrix.drawTriangle(xmod + x0, ymod + y0, xmod + x1, ymod + y1, xmod + x2, ymod + y2, col);
}

void drawLine(int x0, int y0, int x1, int y1, int x2, int y2, int xmod, int ymod, uint16_t col)
{
	matrix.drawTriangle(xmod + x0, ymod + y0, xmod + x1, ymod + y1, xmod + x2, ymod + y2, col);
}

void drawRect(int x0, int y0, int w, int h, int xmod, int ymod, uint16_t col)
{
	matrix.drawRect(xmod + x0, ymod + y0, w, h, col);
}
#pragma endregion