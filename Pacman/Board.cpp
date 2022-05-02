#include "Board.h"

void Board::getBoard(ifstream& infile)
{
	int i = 0, j = 0;
	bool topleft = false, topRight = false;
	bool pacExist = false;
	bool ampExist = false;
	vector<Position> tmpGst(0);
	vector<Position> tmpFrt(0);

	while (!infile.eof())
	{
		char tmp;
		tmp = infile.get();
		if (tmp == '#')
		{
			if (!topleft)
			{
				topLeftPos.row = i;
				topLeftPos.col = j;
				topleft = true;
				topRight = true;
			}
			board[i][j] = FRAME;
		}
		else if (tmp == ' ')
		{
			countFood++;
			board[i][j] = FOOD;
			frtPos.row = i;
			frtPos.col = j;
			if (topleft)
			{
				if (!(i >= ampPos.row && i <= ampPos.row + 2 &&
					j >= ampPos.col && j <= ampPos.col + 20))
					tmpFrt.push_back(frtPos);
			}
		}
		else if (tmp == '%')
		{
			board[i][j] = ' ';
			frtPos.row = i;
			frtPos.col = j;
			//tmpFrt.push_back(frtPos);
		}
		else if (tmp == '&')
		{
			if (!ampExist)
			{
			ampExist = true;
			ampPos.row = i;
			ampPos.col = j;
			scorePos.row = i + 1;
			scorePos.col = j;
			livesPos.row = i + 2;
			livesPos.col = j;
			}
			board[i][j] = ' ';
		}
		else if (tmp == '$')
		{
			board[i][j] = ' ';
			gstPos.row = i;
			gstPos.col = j;
			if (tmpGst.size() < 4)
			    tmpGst.push_back(gstPos);
		}
		else if (tmp == '@')
		{
			if (!pacExist)
			{
				pacExist = true;
				pacPos.row = i;
				pacPos.col = j;
			}
			board[i][j] = ' ';
		}
		else if (tmp == '\n')
		{
			countRows++;
			if (topleft && topRight)
			{
			    countCols = j;
				topRight = false;
			}
			i++;
			j = -1;
		}
		j++;
	}
	countRows++;
	for (int n = ampPos.row; n < ampPos.row + 3; n++)
	{
		for (int k = ampPos.col; k < ampPos.col + 20; k++)
		{
			if (n == ampPos.row && k == ampPos.col + 20)
				continue;
			else if (board[n][k] == FOOD)
			{
				board[n][k] = ' ';
				countFood--;
			}
			else
				board[n][k] = ' ';
				
		}
	}
	if (ampPos.row == 0 && !(board[ampPos.row][countCols - 1] == ' '))
		countRows += 2;
	if (ampPos.row == countRows - 1)
		countRows += 2;
	else if (countRows - ampPos.row == 2)
		countRows -= 2;
	frtPosVec = tmpFrt;
	gstPosVec = tmpGst;
	infile.close();
}

