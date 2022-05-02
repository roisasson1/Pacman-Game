#include "Ghost.h"

// c'tor of the ghost
Ghost::Ghost(char gst, int row, int col): GameObjects(gst,row,col)
{
	// Default position of the ghost
	prev.row = row;
	prev.col = col;
}

Position Ghost::getMovement(char step)
{
	Position nextPos;
	int row, col;

	row = pos.row;
	col = pos.col;

	if (step == UP1)
		row -= 1;
	else if (step == DOWN1)
		row += 1;
	else if (step == RIGHT1)
		col += 1;
	else if (step == LEFT1)
		col -= 1;

	nextPos.row = row;
	nextPos.col = col;
	return nextPos;
}

// this function draws the ghost on the screen
void Ghost::draw(char ch, int choice)
{
	gotoxy(this->getCol(), this->getRow());
	if (choice == 2)
	{
		if (ch == FOOD)
			setTextColor(LIGHTCYAN);
		else
			setTextColor(RED);
	}
	cout << ch;
	if (choice == 2)
		setTextColor(WHITE);
}