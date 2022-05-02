#include "Fruit.h"

// this function draws the ghost on the screen
void Fruit::draw(char ch, int choice)
{
	gotoxy(this->getCol(), this->getRow());
	if (choice == 2)
	{
		if (ch == FOOD)
			setTextColor(LIGHTCYAN);
		else
		    setTextColor(LIGHTGREY);
	}
	cout << ch;
	if (choice == 2)
		setTextColor(WHITE);
}