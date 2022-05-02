#pragma once
#include "Pacman.h"
#include <fstream>

class Board
{
	char board[ROWS][COLS];
	Position ampPos, scorePos, livesPos, pacPos, gstPos, frtPos, topLeftPos;
	vector<Position> gstPosVec, frtPosVec;
	int countCols = 0;
	int countRows = 0;
	int Rows = 0;
	int countFood = 0;
	friend class Game;
	friend class Menu;
public:
	void getBoard(ifstream& infile);
};

