#include "GameObjects.h"

GameObjects::GameObjects()
{
	setCh(' ');
	setRow(0);
	setCol(0);
}

GameObjects::GameObjects(char ch, int row, int col)
{
	setCh(ch);
	setRow(row);
	setCol(col);
}

char GameObjects::getCh() const
{
	return ch;
}

int GameObjects::getRow() const
{
	return pos.row;
}

int GameObjects::getCol() const
{
	return pos.col;
}

void GameObjects::setCh(char ch)
{
	this->ch = ch;
}

void GameObjects::setRow(int row)
{
	pos.row = row;
}

void GameObjects::setCol(int col)
{
	pos.col = col;
}

void GameObjects::movement(char step)
{
	if (step == UP1)
		setRow(pos.row - 1);
	else if (step == DOWN1)
		setRow(pos.row + 1);
	else if (step == RIGHT1)
		setCol(pos.col + 1);
	else if (step == LEFT1)
		setCol(pos.col - 1);
}

/// ///////////////////////////////////////////////////////////////

bool Position::operator==(const Position& pos) const
{
	if (this->row == pos.row && this->col == pos.col)
		return true;
	return false;
}

void gotoxy(int x, int y)
{
	HANDLE hConsoleOutput;
	COORD dwCursorPosition;
	cout.flush();
	dwCursorPosition.X = x;
	dwCursorPosition.Y = y;
	hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hConsoleOutput, dwCursorPosition);
}

void hideCursor()
{
	HANDLE myconsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO CURSOR;
	CURSOR.dwSize = 1;
	CURSOR.bVisible = FALSE;
	SetConsoleCursorInfo(myconsole, &CURSOR);//second argument need pointer
}

void setTextColor(Color colorToSet) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (int)colorToSet);
}