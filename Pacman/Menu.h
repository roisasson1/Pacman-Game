#pragma once
#include <filesystem>
#include <fstream>
#include "Game.h"
using std::filesystem::directory_iterator;

class Menu
{
private:
	int countPacFiles = 0;
	vector<string> files;
public:
	void menu(int mode);
	void order1(string& fileName, ifstream& infile);
	void checkValidOrder(Game& game);
	void checkValidLevel(Game& game);
	void gamePlayMenu(Game& game, int& curFile, bool& lexOn, ifstream& infile, string& fileName);
	void printMenu();
	void printLevel();
	void printFilesOrder();
	void printInstructions();
	void exitMessage();
	void wrongInput();
	void lexOrder();
};



