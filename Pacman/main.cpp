/***************** PACMAN ******************
 * Name: Roi Sasson                       *
 * ID Number: 208633677                   *
 *                                        *
 * Name: Stav Shay                        *
 * ID Number: 316062520                   *
 ******************************************/

#include "Menu.h"
#include <iostream>
using namespace std;

void wrongInput();
void noFile();

int main(int argc, char** argv)
{
	Menu menu;
	int mode = simple;
	bool loadON = false;
	bool saveON = false;
	
	if (!argv[1])
		mode = simple;
	else if (strcmp(argv[1], "-load") == 0)
	{
		mode = load;
		loadON = true;
	}
	else if (strcmp(argv[1], "-save") == 0)
	{
		mode = save;
		saveON = true;
	}
	else
	{
		wrongInput();
		return 0;
	}

	if (loadON)
	{
		if (argv[2])
		{
			if (strcmp(argv[2], "[-silent]") == 0)
				mode = silent;
			else
			{
				wrongInput();
				return 0;
			}
		}
	}
	if (saveON)
	{
		if (argv[2])
		{
			wrongInput();
			return 0;
		}
	}

	menu.menu(mode);
	return 0;
}

void wrongInput()
{
	cout << "Wrong input!" << endl << "Please write: -load | -save | -load [-silent]";
}

void noFile()
{
	cout << "There are no loaded files!";
}