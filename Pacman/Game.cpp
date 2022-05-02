#include "Game.h"
#include <time.h>

////////////////////////////////////////////////////////
// Game Play

// this function is the engine of the game
void Game::gamePlay( ifstream& infile, string& fileName)
{
	int tmp = 0;
	char key = 0, step = 0, randomStepFrt;
	bool game = true, meetPac = false;
	Position pos, currPos;

	int randomFrt = 50;
	char randVal = (rand() % 5 + 5) + '0';
	bool frtON = false;

	char randomStepGst;
	srand((int)time(NULL));
	randomStepGst = convertToDirection(0);

	bool flag = false;
	int countGhosts = (int)ghosts.size();

	if (mode == save)
		getOutFileName(fileName); // create steps file and result file
	if (mode == load || mode == silent)
	{
		getInFileName(fileName, game); // reading steps file and result file
		if (!game)
			return;
	}

	board.getBoard(infile);
	int foodLeft = board.countFood;
	if (mode != silent)
		printBoardGame(); // initialize the board
	initObj(randVal);

	if (mode == save || mode == simple)
		key = getKey();  // temporary variable fill the key until the player start the game
	while (game)
	{
		if (mode == save || mode == simple)
		{
			Sleep(100); // sleeps for a while before continuing to the next command
			if (_kbhit()) // if any key was hit
				key = getKey(); // temporary variable fill the key until the player start the game

			// only these keys changing the direction 
			if (trueKey(key))
				step = key; // the player enters a key

			if (key == ESC) // if the user pressed ESC
				pause(key, step);

			if (mode == save) // set output to file
				outfile_steps << step;
		}
		if (mode == load || mode == silent)
		{
			if (mode == load)
				Sleep(5); // sleeps for half a second before continuing to the next command
			step = Load();
		}

		pacPlay(pos, currPos, step, foodLeft);
		if (stepCounter > 0 && pos == pacman.prev)
			flag = true;
		pacman.prev = pos;

		pacMeetGst(meetPac);
		if (meetPac)
		{
			if (mode == save)
				outfile_res << stepCounter << endl;
			// update the LIVES
			lives--;
			if (lives != 0) // if the game is not over yet
				reinit(randomStepGst, randVal); // reinitialize the objects
			meetPac = false;

			//equalize between the step counter and the output from the file
			if (mode == silent || mode == load)
			{
				int res;
				infile_res >> res;
				if (res != stepCounter)
				{
					system("cls");
					if (mode == silent)
						cout << "test failed!";
					testPassed = false;
					choice = 9;
				}
				else
					testPassed = true;
			}

			if (frtON)
			{
				frtON = false;
				if (mode == 1)
					outfile_steps << "!";
			}
		}

		else if (stepCounter % 2 == 0) // every two steps of the pacman - the ghost is moving
			ghostPlay(randomStepGst, flag);
		pacMeetGst(meetPac);

		frtPlay(frtON, randomStepFrt, randomFrt, tmp);

		stepCounter++;

		if (gameOver(foodLeft)) // if the game is over
			gameOverReinit(game);

	}
}

// this function creating the game objects
void Game::initObj(char& randVal)
{
	pacman.setRow(board.pacPos.row);
	pacman.setCol(board.pacPos.col);
	if (mode != silent)
		pacman.draw(PACMAN, choice);

	for (unsigned int i = 0; i < board.gstPosVec.size(); i++)
	{
		Ghost gst(GHOST, board.gstPosVec[i].row, board.gstPosVec[i].col);
		ghosts.push_back(gst);
		if (mode != silent)
			ghosts[i].draw(GHOST, choice); // draw the ghosts on the board
	}

	frt.setCh(randVal);
	frt.setRow(board.frtPosVec[0].row);
	frt.setCol(board.frtPosVec[0].col);

	// initialize lives and score
	if (mode != silent)
	{
		gotoxy(board.livesPos.col + 7, board.livesPos.row);
		cout << lives;
		gotoxy(board.scorePos.col + 7, board.scorePos.row);
		cout << score;
		hideCursor();
	}
}

// this function prints the borders of the board
void Game::printBoardGame()
{
	int rows, cols;
	for (rows = 0; rows < board.countRows; rows++)
	{
		for (cols = 0; cols < board.countCols ; cols++)
		{
			if (choice == 2)
				setTextColor(LIGHTCYAN);
			cout << board.board[rows][cols];
			if (choice == 2)
				setTextColor(WHITE);
		}
		cout << endl;
	}
	cout << endl;
	gotoxy(board.livesPos.col, board.livesPos.row);
	cout << "Lives: ";
	gotoxy(board.scorePos.col, board.scorePos.row);
	cout << "Score: " << endl;
	hideCursor();
}

////////////////////////////////////////////////////////
// Pacman

// this function checks the moves of the pacman
void Game::pacPlay(Position& pos, Position& currPos, char step, int& foodLeft)
{
	pos.row = pacman.getRow();
	pos.col = pacman.getCol();
	if (checkMove(step, pacman.getRow(), pacman.getCol())) // if the pacman's move is valid
	{
		currPos.row = pos.row;
		currPos.col = pos.col;

		if (mode != silent)
			pacman.draw(' ', choice); // clear the previous step
		pacman.movement(step, board.countRows, board.countCols); // update the pacman's place
		eatFood(currPos, foodLeft); // update food and score
		if (mode != silent)
		{
			pacman.draw(PACMAN, choice); // draw the pacman on the new place
			hideCursor();
		}
	}
}

// this function checks if there's food in the next step of the pacman
void Game::eatFood(Position currPos, int& foodLeft)
{
	if (board.board[currPos.row][currPos.col] == FOOD)
	{
		board.board[currPos.row][currPos.col] = ' ';
		foodLeft--;
		score += 10;
		if (mode != silent)
		{
			gotoxy(board.scorePos.col + 7, board.scorePos.row);
			cout << score;
		}
	}
}

// this function checks if one of the ghosts meet the pacman
void Game::pacMeetGst(bool& meetPac)
{
	for (unsigned int i = 0; i < ghosts.size(); i++)
	{
		if (eatPacman(ghosts[i])) // if the pacman and one of the ghosts have met
		{
			meetPac = true;
			break;
		}
	}
}

////////////////////////////////////////////////////////
// Checks

// this function checks if the move is valid
bool Game::checkMove(char step, int row, int col)
{
	if (step == UP1 || step == UP2)
	{
		if (checkBorders(row - 1, col))
			return false;
	}
	else if (step == DOWN1 || step == DOWN2)
	{
		if (checkBorders(row + 1, col))
			return false;
	}
	else if (step == RIGHT1 || step == RIGHT2)
	{
		if (checkBorders(row, col + 1))
			return false;
	}
	else if (step == LEFT1 || step == LEFT2)
	{
		if (checkBorders(row, col - 1))
			return false;
	}
	return true;
}

// this function checks if the objects meet the walls
bool Game::checkBorders(int row, int col)
{
	if (col == -1 || col == board.countCols /*- 1*/)
		return false;
	if (row == -1 || row == board.countRows /*- 1*/)
		return false;
	if (board.board[row][col] == FRAME)
		return true;
	return false;
}

// this function checks the tunnels validity
bool Game::checkTunnels(int row, int col, char randomStep)
{
	if (row - 1 == 0 && randomStep == UP1)
		return false;
	else if (row + 1 == board.countRows && randomStep == DOWN1)
		return false;
	else if (col + 1 == board.countCols && randomStep == RIGHT1)
		return false;
	else if (col - 1 == 0 && randomStep == LEFT1)
		return false;
	else
		return true;
}

////////////////////////////////////////////////////////
// Ghosts

// this function checks if the ghost eats pacman
bool Game::eatPacman(Ghost& gst)
{
	if (gst.getRow() == pacman.getRow() && gst.getCol() == pacman.getCol())
		return true;
	return false;
}

// this function picks the level of the ghosts
void Game::ghostPlay(char& randomStepGst, bool flag)
{
	// NOVICE
	if (level == 1)
		levelNOVICE(randomStepGst);
	// GOOD
	else if (level == 2)
		levelGOOD(randomStepGst, flag);
	// BEST
	else if (level == 3)
		levelBEST(flag);
}

// this function checks if the next move of the ghosts is valid
void Game::gstMove(Ghost& gst, char randomStep)
{
	if (checkMove(randomStep, gst.getRow(), gst.getCol()) && checkTunnels(gst.getRow(), gst.getCol(), randomStep))
	{
		// if the ghost is meeting food, it skips that
		if (board.board[gst.getRow()][gst.getCol()] == FOOD && mode != silent)
			gst.draw(FOOD, choice);
		else if (mode != silent)
			gst.draw(' ', choice);
		gst.movement(randomStep);
		if (mode != silent)
			gst.draw(GHOST, choice);
	}
}

void Game::levelNOVICE(char& randomStepGst)
{
	for (unsigned int i = 0; i < ghosts.size(); i++)
	{
		// starting move after 4 moves and every 20 steps
		if (stepCounter % 20 == 4)
		{
			if (mode == save || mode == simple)
			{
				randomStepGst = convertToDirection((rand() % 4));
				if (mode == save)
					outfile_steps << randomStepGst;
			}
			if (mode == load)
				randomStepGst = Load();
		}
		if (!checkGstMeetGst(i, randomStepGst))
			gstMove(ghosts[i], randomStepGst);
	}
}

void Game::levelGOOD(char& randomStepGst, bool flag)
{
	if (stepCounter % 20 >= 14 && stepCounter % 20 <= 19)
		levelNOVICE(randomStepGst);
	else
		levelBEST( flag);
}

void Game::levelBEST(bool flag)
{
	disLoc d[4];
	double min;
	int minInd;
	char step = 0;
	bool found = false;
	for (unsigned int i = 0; i < ghosts.size(); i++)
	{
		if (mode == save || mode == simple)
		{
			for (int j = 0; j < 4; j++)
			{
				// checks if the intended move is valid
				d[j].point = ghosts[i].getMovement(convertToDirection(j));
				if (checkBorders(d[j].point.row, d[j].point.col) || checkGstMeetGst(i, d[j].point))
					d[j].distance = ERRORMOVE;
				else if ((flag == true && d[j].point == ghosts[i].prev))
					d[j].distance = ERRORMOVE;
				else if (d[j].point.row >= board.ampPos.row && d[j].point.row <= board.ampPos.row + 2 &&
					d[j].point.col >= board.ampPos.col && d[j].point.col <= board.ampPos.col + 20)
					d[j].distance = ERRORMOVE;
				else // saves the distance
					d[j].distance = calcDis(d[j].point);
				if (j == 0)
				{
					min = d[j].distance;
					minInd = 0;
				}

				if (d[j].distance < min) // update the minimal distance
				{
					min = d[j].distance;
					minInd = j;
				}
			}
			step = convertToDirection(minInd);
			if (mode == save)
				outfile_steps << step;
		}
		if (mode == load || mode == silent)
			step = Load();
		// only from the second iteration
		ghosts[i].prev.row = ghosts[i].getRow();
		ghosts[i].prev.col = ghosts[i].getCol();
		gstMove(ghosts[i], step);
	}
}

// this function checks if the ghosts meet themselves
bool Game::checkGstMeetGst(int currGst, Position nextPos)
{
	Position pos;

	for (unsigned int i = 0; i < ghosts.size(); i++)
	{
		pos.row = ghosts[i].getRow();
		pos.col = ghosts[i].getCol();
		if (i == currGst)
			continue;
		if (pos == nextPos)
			return true;
	}
	return false;
}

bool Game::checkGstMeetGst(int currGst, char& randomStepGst)
{
	Position nextRandPos = ghosts[currGst].getMovement(randomStepGst);
	return checkGstMeetGst(currGst, nextRandPos);
}

double Game::calcDis(Position pos)
{
	double disX = (double)pacman.getRow() - (double)pos.row;
	double disY = (double)pacman.getCol() - (double)pos.col;
	double res = pow(disX, 2) + pow(disY, 2);
	return sqrt(res);
}

////////////////////////////////////////////////////////
// Fruit

void Game::frtPlay(bool& frtON, char& randomStepFrt, int& randomFrt, int& tmp)
{
	if (frtON) // if the fruit appears on the screen
	{
		if (mode == save || mode == simple)
		{
			randomStepFrt = convertToDirection((rand() % 4)); // random direction
			if (mode == save)
				outfile_steps << randomStepFrt;
		}
		if (mode == load || mode == silent)
			randomStepFrt = Load();
		if (stepCounter % 4 == 0)
		{
			frtMove(randomStepFrt, randVal);
		}
		if (frtDisappear(tmp))
		{
			frtON = false;
			if (mode == save)
				outfile_steps << "!";
		}
	}
	else if (!frtON) // if the fruit doesn't exist
	{
		// sets the random value and places it on the screen
		if (stepCounter % 100 + 5 == randomFrt)
		{
			frtON = true;
			if (mode == save || mode == simple)
				randVal = (rand() % 5 + 5) + '0';
			if (mode == load || mode == silent)
				randVal = Load();

			frt.setCh(randVal);
			placeRandFrt();
			if (mode != silent)
				frt.draw(randVal, choice);
		}
		tmp = stepCounter; // saves the temporary counter
	}
}

void Game::frtMove(char randomStep, char randVal)
{
	if (checkMove(randomStep, frt.getRow(), frt.getCol()) && checkTunnels(frt.getRow(), frt.getCol(), randomStep))
	{
		// if the ghost is meeting food, it skips that
		if ((board.board[frt.getRow()][frt.getCol()] == FOOD) && (mode != silent))
			frt.draw(FOOD, choice);
		else if (mode != silent)
			frt.draw(' ', choice);
		frt.movement(randomStep);
		if (mode != silent)
			frt.draw(randVal, choice);
	}
}

// this function places randomly the fruit on the screen
void Game::placeRandFrt()
{
	char randStep = 0;
	int row = 0, col = 0;
	bool OK = false;

	if (mode == save || mode == simple)
	{
		int size = (int)board.frtPosVec.size();
		int randInd = rand() % size;
		row = board.frtPosVec[randInd].row;
		col = board.frtPosVec[randInd].col;
		if (mode == save)
			outfile_steps << "?" << row << "," << col << "," << randVal;
	}
	if (mode == load || mode == silent)
	{
		row = frtPlace.row;
		col = frtPlace.col;
	}
	while (!OK)
	{
		if (!checkBorders(row, col) && !checkObj(row, col) && checkTunnels(row, col, randStep))
		{
			OK = true;
			frt.setRow(row);
			frt.setCol(col);
			break;
		}
		row = rand() % ROWS;
		col = rand() % COLS;
	}
}

// checks if the movement is valid
bool Game::checkObj(int row, int col)
{
	if (pacman.getRow() == row && pacman.getCol() == col)
		return true;
	else if (ghosts[0].getRow() == row && ghosts[0].getCol() == col)
		return true;
	else if (ghosts[1].getRow() == row && ghosts[1].getCol() == col)
		return true;
	return false;
}

bool Game::frtDisappear(int tmp)
{
	if (tmp == stepCounter - 100)
	{
		if ((board.board[frt.getRow()][frt.getCol()] == FOOD) && (mode != silent))
			frt.draw(FOOD, choice);
		else if (mode != silent)
			frt.draw(' ', choice);
		return true;
	}

	// if the pacman meet the fruit
	if (meetFrt())
	{
		int bonus = frt.getCh() - '0';
		score += bonus;
		if (mode != silent)
		{
			gotoxy(board.scorePos.col + 7, board.scorePos.row);
			cout << score;
		}
		return true;
	}

	// if the ghosts meet the fruit
	for (unsigned int i = 0; i < ghosts.size(); i++)
	{
		if (meetFrt(ghosts[i]))
		{
			if ((board.board[frt.getRow()][frt.getCol()] == FOOD) && (mode != silent))
			{
				frt.draw(FOOD, choice);
				if (choice == 2)
					setTextColor(LIGHTCYAN);
			}
			else if (mode != silent)
				frt.draw(' ', choice);
			return true;
		}
	}
	return false;
}

bool Game::meetFrt()
{
	if (frt.getRow() == pacman.getRow() && frt.getCol() == pacman.getCol())
		return true;
	return false;
}

bool Game::meetFrt(Ghost& gst)
{
	if (frt.getRow() == gst.getRow() && frt.getCol() == gst.getCol())
		return true;
	return false;
}

////////////////////////////////////////////////////////
// Middle-end game

// this function pauses the game until the user presses ESC again
void Game::pause(char& key, char& step)
{
	bool hitESC = false;
	// initialize lives and score
	gotoxy(board.ampPos.col, board.ampPos.row);
	cout << "****GAME PAUSED!****";
	hideCursor();

	while (hitESC == false)
	{
		key = 0;
		if (_kbhit()) // if any key was hit
			key = getKey();  // temporary variable fill the key until the player start the game
		if (key == ESC)
		{
			hitESC = true;
			break;
		}
	}

	if (hitESC == true)
	{
		gotoxy(board.ampPos.col, board.ampPos.row);
		cout << "                    " << endl;
		key = 0;
		if (_kbhit()) // if any key was hit
			key = getKey();  // temporary variable fill the key until the player start the game

		// only these keys changing the direction 
		if (trueKey(key))
			step = key; // the player enters a key
	}
}

bool Game::gameOver(int foodLeft)
{
	// WINNING CASE
	if (foodLeft == 0)
	{
		// saves the step counter and the lives
		if (mode == save)
		{
			outfile_res << stepCounter << endl;
			outfile_res << lives << endl;
			outfile_steps.close();
			outfile_res.close();
		}
		if (mode == load || mode == silent)
		{
			int res;
			infile_res >> res;
			infile_res >> livesNow;

			// equalize between the step counter and the value from the file
			if ((mode == load || mode == silent) && res != stepCounter)
			{
				system("cls");
				if (mode == silent)
				{
					cout << "test failed!";
				}
				if (mode == load)
				{
					cout << "The files are unmatch!";
					Sleep(600);
				}
				choice = 9;
			}

			// equalize between the lives and the value from the file
			if ((mode == load || mode == silent) && livesNow != lives)
			{
				system("cls");
				if (mode == silent)
				{
					cout << "test failed!";
				}
				if (mode == load)
				{
					cout << "The files are unmatch!";
					Sleep(600);
				}
				choice = 9;
			}
			else if ((mode == load || mode == silent) && res == stepCounter)
				testPassed = true;

			infile_steps.close();
			infile_res.close();
		}
		system("cls");
		if (order == 1)
		{
			if (mode != silent)
				winMessage();
			lives = 3;
			score = 0;
		}
		if (order == 2)
		{
			if (mode != silent)
				winPhase();
		}
		return true;
	}
	// LOSING CASE
	else if (lives == 0)
	{
		system("cls");
		if (mode == save)
		{
			outfile_steps.close();
			outfile_res.close();
		}
		if (mode == load || mode == silent)
		{
			infile_steps.close();
			infile_res.close();
		}
		LoseMessage();
		lose = true;
		lives = 3;
		score = 0;
		return true;
	}
	return false;
}

// this function reinit the board for the next map
void Game::gameOverReinit(bool& game)
{
	game = false;
	for (int i = 0; i < board.countRows; i++)
	{
		for (int j = 0; j < board.countCols; j++)
		{
			board.board[i][j] = ' ';
		}
	}
	board.countCols = 0;
	board.countRows = 0;
	board.countFood = 0;
	ghosts.clear();
	if (mode == save)
	{
		cout << "Please enter any key!" << endl;
		_getch();
	}
}

// this function reinit the objects to their default positions
void Game::reinit(char& randomStepGst, char& randVal)
{
	if (mode != silent)
	{
		system("cls");
		printBoardGame();
	}
	pacman.setRow(board.pacPos.row);
	pacman.setCol(board.pacPos.col);
	if (mode != silent)
		pacman.draw(PACMAN, choice);

	for (unsigned int i = 0; i < board.gstPosVec.size(); i++)
	{
		ghosts[i].setRow(board.gstPosVec[i].row);
		ghosts[i].setCol(board.gstPosVec[i].col);
		if (mode != silent)
			ghosts[i].draw(GHOST, choice); // draw the ghosts on the board
	}

	frt.setCh(randVal);
	frt.setRow(board.frtPosVec[0].row);
	frt.setCol(board.frtPosVec[0].col);

	randomStepGst = convertToDirection(0);
	if (mode != silent)
	{
		gotoxy(board.livesPos.col + 7, board.livesPos.row);
		cout << lives;
		gotoxy(board.scorePos.col + 7, board.scorePos.row);
		cout << score;
	}
}

////////////////////////////////////////////////////////
// Other

char Game::convertToDirection(int num)
{
	if (num == 0)
		return UP1;
	else if (num == 1)
		return RIGHT1;
	else if (num == 2)
		return DOWN1;
	else if (num == 3)
		return LEFT1;
	else
		return STAY1;
}

void Game::winMessage()
{
	int i;
	int size = 34;

	for (i = 0; i < size; i++)
		cout << "#";
	cout << endl;

	cout << "            ";
	cout << "YOU WIN" << endl;

	for (i = 0; i < size; i++)
		cout << "#";
	cout << endl << endl;
}

void Game::winPhase()
{
	int i;
	int size = 35;

	for (i = 0; i < size; i++)
		cout << "#";
	cout << endl;

	cout << "           WELL DONE!!!" << endl;

	for (i = 0; i < size; i++)
		cout << "#";
	cout << endl << endl;
}

void Game::LoseMessage()
{
	int i;
	int size = 34;

	for (i = 0; i < size; i++)
		cout << "#";
	cout << endl;

	cout << "            YOU LOSE!!!" << endl;
	cout << "            GAME OVER!" << endl;

	for (i = 0; i < size; i++)
		cout << "#";
	cout << endl << endl;
}

// this function checks if the key is valid
bool Game::trueKey(char key)
{
	if (key == UP1 || key == UP2 || key == RIGHT1 || key == RIGHT2 || key == LEFT1
		|| key == LEFT2 || key == DOWN1 || key == DOWN2 || key == STAY1 || key == STAY2)
		return true;
	return false;
}

// This function gets keyCodes for special keys such as arrows , escape and others
int Game::getKey()
{
	char KeyStroke = _getch();
	if (KeyStroke == 0 || KeyStroke == -32)
		KeyStroke = _getch();
	return (KeyStroke);
}

/////////////////////////////////////////////////////////////////
// FILES

// this function creates a new files to the folder
void Game::getOutFileName(string& fileName)
{
	string name;
	string result;
	name = fileName;
	result = fileName;

	// deleting the ".screen"
	for (int i = 0; i < 7; i++)
	{
		name.pop_back();
		result.pop_back();
	}

	name += ".steps";
	result += ".result";

	outfile_steps.open(name);
	outfile_steps << level;
	outfile_res.open(result);
}

// this function get the files from the folder
void Game::getInFileName(string& fileName, bool& game)
{
	string name;
	string result;
	name = fileName;
	result = fileName;

	// deleting the ".screen"
	for (int i = 0; i < 7; i++)
	{
		name.pop_back();
		result.pop_back();
	}

	name += ".steps";
	result += ".result";

	infile_steps.open(name);

	// checks if the file's existance
	if (!infile_steps)
	{
		checkFileExist(game);
		return;
	}
	infile_steps >> level;
	infile_res.open(result);
	if (!infile_res)
	{
		checkFileExist(game);
		return;
	}
}

// this function loading the next move from the file
char Game::Load()
{
	char s;
	infile_steps >> s;
	if (s == '?')
	{
		infile_steps >> frtPlace.row;
		infile_steps >> s; // ','
		infile_steps >> frtPlace.col;
	}
	if (s == '!')
		infile_steps >> s;
	if (s == ',')
	{
		infile_steps >> s;
		infile_steps >> s;
	}
	return s;
}

void Game::checkFileExist(bool& game)
{
	if (mode != silent)
	{
		cout << "There is no saved file!";
		Sleep(2000);
	}
	else
	{
		cout << "Test failed!";
		Sleep(2000);
	}
	game = false;
	choice = 9;
}