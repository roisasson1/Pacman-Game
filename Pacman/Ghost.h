#pragma once
#include "GameObjects.h"
using namespace std;

const char GHOST = (char)234;

class Ghost:public GameObjects
{
public:
	Position prev;
	Ghost(char gst, int row, int col);
	Position getMovement(char step);
	void draw(char ch, int choice);
};

