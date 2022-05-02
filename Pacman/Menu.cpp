#include "Menu.h"

void Menu::menu(int mode)
{
	Game game;
	int curFile = 0;
	char ch, ch2;
	string fileName;
	ifstream infile;
	bool lexOn = false;
	bool gameOver = false;
	game.mode = mode;
	
	if (game.mode == save || game.mode == simple)
	{
		printMenu();
		ch = _getch();
		game.choice = ch - '0';
	}

	while (game.choice != 9)
	{
		gameOver = false;
		system("cls");
		if (game.choice == 1 || game.choice == 2)
		{
			if (!lexOn) 
			{
				if (game.mode == save || game.mode == simple)
				{
					printFilesOrder();
					ch = _getch();
					game.order = ch - '0';
					while (game.order != 1 && game.order != 2)
						checkValidOrder(game);
				}
				if (game.order == 1)
					order1(fileName, infile);
				else if (game.order == 2)
				{
					lexOrder();
					lexOn = true;
				}

				if (mode == save || mode == simple)
				{
					printLevel();
					ch2 = _getch();
					game.level = ch2 - '0';
					while (game.level != 1 && game.level != 2 && game.level != 3)
						checkValidLevel(game);
				}
			}
			if (lexOn)
			{
				fileName = files[curFile];
				infile.open(fileName);
				curFile++;
			}
			if (!gameOver)
			{
				gamePlayMenu(game, curFile, lexOn, infile, fileName);
				game.stepCounter = 0;
			}
		}
		else if (game.choice == 8)
			printInstructions();
		else if (game.choice < 0 || game.choice >9)
		{
			wrongInput();
		}
		else
			wrongInput();
		if (!lexOn)
		{
			if (mode == save || mode == simple)
			{
				printMenu();
				ch = _getch();
				game.choice = ch - '0';
			}
			else
				break;
		}
	}
	exitMessage();
	return;
}

void Menu::lexOrder()
{
	vector<string> files(0);
	string path = ".";
	for (const auto& file : directory_iterator(path))
	{
		if (file.path().string().ends_with(".screen"))
		{
			files.push_back(file.path().string());
			countPacFiles++;
		}
	}
	this->files = files;
}

void Menu::printMenu()
{
	system("cls");
	cout << "(1) Start a new game" << endl;
	cout << "(2) Start a new game with colors" << endl;
	cout << endl << "(8) Present instructions and keys" << endl;
	cout << "(9) EXIT" << endl;
}

void Menu::printLevel()
{
	system("cls");
	cout << "Please choose your level:" << endl;
	cout << "1) NOVICE\n2) GOOD\n3) BEST" << endl;
}

void Menu::printFilesOrder()
{
	system("cls");
	cout << "Please choose:" << endl;
	cout << "1) Upload specific screen\n2) Upload lexicographic order" << endl;
}

void Menu::printInstructions()
{
	cout << "  You have to eat the all breadcrumbs without caught by the ghosts for winning the game." << endl;
	cout << "  If the ghosts caught you 3 times, the game is over!" << endl;
	cout << endl << "  The keys for playing: " << endl;
	cout << "   UP - W or w" << endl << "   DOWN - X or x" << endl << "   LEFT - A or a" << endl
		<< "   RIGHT - D or d" << endl << "   Staying the same place - S or s" << endl;
	cout << endl << "  Let's start playing!" << endl;
	_getch();
}

void Menu::exitMessage()
{
	system("cls");
	cout << endl << "Goodbye!" << endl;
}

void Menu::wrongInput()
{
	cout << "Wrong input! Please press a key to return" << endl;
	_getch();
}

void Menu::order1(string& fileName, ifstream& infile)
{
	system("cls");
	cout << "Please enter name of file:" << endl;
	cin >> fileName;
	fileName += ".screen";
	infile.open(fileName);
	while (!infile)
	{
		system("cls");
		cout << "File is not found! Please enter a valid name: " << endl;
		cin >> fileName;
		fileName += ".screen";
		infile.open(fileName);
	}
}

void Menu::checkValidOrder(Game& game)
{
	char ch;
	system("cls");
	wrongInput();
	printFilesOrder();
	ch = _getch();
	game.order = ch - '0';
}

void Menu::checkValidLevel(Game& game)
{
	char ch;
	system("cls");
	wrongInput();
	printLevel();
	ch = _getch();
	game.level = ch - '0';
}

void Menu::gamePlayMenu(Game& game, int& curFile, bool& lexOn, ifstream& infile, string& fileName)
{
	system("cls");

	game.gamePlay(infile, fileName);
	if (game.lose)
	{
		if (game.mode == silent && game.testPassed)
		{
			system("cls");
			cout << "Test passed!";
			Sleep(2000);
			game.choice = 9;
		}
		countPacFiles = 0;
		files.clear();
		game.lose = false;
		lexOn = false;
		curFile = 0;
	}
	else
	{
		game.livesNow = game.lives;
		if (game.mode == silent && game.testPassed && game.order == 1)
		{
			system("cls");
			cout << "Test passed!";
			game.choice = 9;
		}
		if (curFile == countPacFiles && game.order == 2)
		{
			countPacFiles = 0;
			files.clear();
			lexOn = false;
			curFile = 0;
			system("cls");
			game.winMessage();
			if (game.mode == silent && game.testPassed)
			{
				system("cls");
				cout << "Test passed!";
				Sleep(2000);
			}
			if (game.mode == simple || game.mode == save)
				_getch();
		}
	}
}