#pragma once
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <vector>
#include <time.h>
using namespace std;

const char PACMAN = (char)002;
const char FOOD = (char)250;

const int RIGHT1 = 68; // D
const int LEFT1 = 65; // A
const int UP1 = 87; // W
const int STAY1 = 83; // S
const int DOWN1 = 88; // X

const int RIGHT2 = 100; // d
const int LEFT2 = 97; // a
const int UP2 = 119; // w
const int STAY2 = 115; // s
const int DOWN2 = 120; // x

const int ESC = 27;
const char FRAME = (char)178;
const int ROWS = 25;
const int COLS = 80;
const int ERRORMOVE = 100;

enum Mode {load, save, silent, simple};

enum  direction { up, down, left, right };

enum Color
{
	BLACK = 0,
	BLUE = 1,
	GREEN = 2,
	CYAN = 3,
	RED = 4,
	MAGENTA = 5,
	BROWN = 6,
	LIGHTGREY = 7,
	DARKGREY = 8,
	LIGHTBLUE = 9,
	LIGHTGREEN = 10,
	LIGHTCYAN = 11,
	LIGHTRED = 12,
	LIGHTMAGENTA = 13,
	YELLOW = 14,
	WHITE = 15
};

struct Position {
	int row;
	int col;
public:
	bool operator==(const Position& pos) const;
};


class GameObjects
{
protected:
	char ch;
	Position pos;
public:
	GameObjects();
	GameObjects(char ch, int row, int col);
	char getCh() const;
	int getRow() const;
	int getCol() const;
	void setCh(char ch);
	void setRow(int row);
	void setCol(int col);
	void movement(char step);
	virtual void draw(char ch, int choice) = 0;
};

void gotoxy(int x, int y);
void hideCursor();
void setTextColor(Color colorToSet);

