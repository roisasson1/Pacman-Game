#include "Pacman.h"

// this function updates the pacman's move
void Pacman::movement(char step, int rows, int cols)
{
	if (step == UP1 || step == UP2)
	{
		if (pos.row == 0)
			setRow(rows - 2);
		else
		    setRow(pos.row - 1);
	}
	else if (step == DOWN1 || step == DOWN2)
	{
		if (pos.row == rows - 1)
			setRow(0);
		else
			setRow(pos.row + 1);
	}
	else if (step == RIGHT1 || step == RIGHT2)
	{
		if (pos.col == cols - 1)
			setCol(0);
		else
			setCol(pos.col + 1);
	}
	else if (step == LEFT1 || step == LEFT2)
	{
		if (pos.col == 0)
			setCol(cols - 1);
		else
			setCol(pos.col - 1);
	}
}

// this function draws the pacman on the screen
void Pacman::draw(char ch, int choice)
{
	gotoxy(this->getCol(), this->getRow());
	if (choice == 2)
	   setTextColor(YELLOW);
	cout << ch;
	if (choice == 2)
		setTextColor(LIGHTGREY);
}


