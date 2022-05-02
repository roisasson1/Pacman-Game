#pragma once
#include "GameObjects.h"

class Pacman:public GameObjects
{
public:
	Position prev;
	void movement(char step, int rows, int cols);
	void draw(char ch, int choice);
};


