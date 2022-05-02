#pragma once
#include "Board.h"
#include "GameObjects.h"
#include "Pacman.h"
#include "Ghost.h"
#include "Fruit.h"
#include <iostream>
#include <math.h>

struct disLoc // distance and location
{
	Position point;
	double distance;
};

class Game
{
private:
	Pacman pacman;
	Board board;
	vector<Ghost> ghosts;
	Fruit frt;
	int lives = 3, score = 0;
	int livesNow = 3;
	int level = -1;
	int mode = simple;
	int choice = 1; /////////
	int order = 2;
	bool lose = false;
	ofstream outfile_steps;
	ofstream outfile_res;
	ifstream infile_steps;
	ifstream infile_res;
	Position frtPlace;
	char randVal = 0;
	friend class Menu;
	int stepCounter = 0;
	bool testPassed = 0;

public:
	// Game Play
	void gamePlay( ifstream& infile, string& fileName);
	void initObj( char& randVal);
	void printBoardGame();

	// Pacman
	void pacPlay(Position& pos, Position& currPos, char step, int& foodLeft);
	void eatFood(Position currPos, int& foodLeft);
	void pacMeetGst(bool& meetPac);
	bool eatPacman(Ghost& gst);

	// Checks
	bool checkMove(char step, int row, int col);
	bool checkBorders(int row, int col);
	bool checkTunnels(int row, int col, char randomStep);
	
	// Ghosts
	void ghostPlay(char& randomStepGst, bool flag);
	void levelNOVICE(char& randomStepGst);
	void levelGOOD(char& randomStepGst, bool flag);
	void levelBEST(bool flag);
	void gstMove(Ghost& gst, char randomStep);
	bool checkGstMeetGst(int currGst, Position nextPos);
	bool checkGstMeetGst(int currGst, char& randomStepGst);
	double calcDis(Position pos);

	// Fruit
	void frtPlay(bool& frtON, char& randomStepFrt, int& randomFrt, int& tmp);
	void frtMove(char randomStep, char randVal);
	void placeRandFrt();
	bool checkObj(int row, int col);
	bool frtDisappear(int tmp);
	bool meetFrt();
	bool meetFrt(Ghost& gst);

	// Middle-end game
	void pause(char& key, char& step);
	bool gameOver( int foodLeft);
	void gameOverReinit(bool& game);
	void reinit(char& randomStepGst, char& randVal);

	// Other
	char convertToDirection(int num);
	bool trueKey(char key);
	int getKey();
	void winPhase();
	void winMessage();
	void LoseMessage();

	// Files
	void getOutFileName(string& fileName);
	void getInFileName(string& fileName, bool& game);
	char Load();
	void checkFileExist(bool& game);
};