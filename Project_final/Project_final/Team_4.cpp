#include "STcpClient.h"
#include <math.h>
#include <iostream>

using namespace std;

class piece {
public:
	int row;
	int col;
	int color;
};

class step {
public:
	int row1;
	int col1;
	int row2;
	int col2;
	step *next;
};

vector<vector<int>> Step;
vector<vector<int>> Pair;
vector<vector<vector<int>>> MoveSet;
vector<int> ScoreSet;
int store_score;

int Board[8][8] = {
{ 1,0,0,0,0,0,0,0 },
{ 0,1,0,0,0,0,0,2 },
{ 1,0,1,0,0,0,2,0 },
{ 0,1,0,0,0,2,0,2 },
{ 1,0,1,0,0,0,2,0 },
{ 0,1,0,0,0,2,0,2 },
{ 1,0,0,0,0,0,2,0 },
{ 0,0,0,0,0,0,0,2 } };

int Mine[8][8] = {
{ 0,0,0,0,0,0,0,0 },
{ 0,0,0,0,0,0,0,0 },
{ 0,0,0,0,0,0,0,0 },
{ 0,0,0,0,0,0,0,0 },
{ 0,0,0,0,0,0,0,0 },
{ 0,0,0,0,0,0,0,0 },
{ 0,0,0,0,0,0,0,0 },
{ 0,0,0,0,0,0,0,0 } };

//|L£¤X2{|baoaO¡èl
void PrintBoard()
{
	cout << "~ 0 1 2 3 4 5 6 7" << endl;
	for (int i = 0; i < 8; i++)
	{
		cout << i;
		for (int j = 0; j < 8; j++)
		{
			if (Board[i][j] == 1) cout << " X";
			else if (Board[i][j] == 2) cout << " 0";
			else cout << " -";
		}
		cout << endl;
	}
	cout << endl;
}

//¨¤??d¡ì¨²¡è¨¨|paG¡èU¡§¨¬"3o-¨®|¨¬?m"?O¡ì_¡¤|3Q1?¡è¨¨|Y
void DetectJump(int row, int col, bool is_black, int dir)
{
	if (row < 0 || row>7 || col < 0 || col>7) return;
	//UP
	else if (dir == 1)
	{
		if ((row - 2) >= 0 && Board[row - 2][col] == 0)
		{
			Mine[row - 1][col] = -1;
			if (Board[row - 1][col] != 0) DetectJump(row - 2, col, is_black, 1);
		}

		if ((col - 2) >= 0 && Board[row][col - 2] == 0)
		{
			Mine[row][col - 1] = -1;
			if (Board[row][col - 1] != 0) DetectJump(row, col - 2, is_black, 3);
		}

		if ((col + 2) <= 7 && Board[row][col + 2] == 0)
		{
			Mine[row][col + 1] = -1;
			if (Board[row][col + 1] != 0) DetectJump(row, col - 2, is_black, 4);
		}
	}
	//DOWN
	else if (dir == 2)
	{
		if ((row + 2) <= 7 && Board[row + 2][col] == 0)
		{
			Mine[row + 1][col] = -1;
			if (Board[row + 1][col] != 0) DetectJump(row + 2, col, is_black, 2);
		}

		if ((col - 2) >= 0 && Board[row][col - 2] == 0)
		{
			Mine[row][col - 1] = -1;
			if (Board[row][col - 1] != 0) DetectJump(row, col - 2, is_black, 3);
		}

		if ((col + 2) <= 7 && Board[row][col + 2] == 0)
		{
			Mine[row][col + 1] = -1;
			if (Board[row][col + 1] != 0) DetectJump(row, col - 2, is_black, 4);
		}
	}
	//LEFT
	else if (dir == 3)
	{
		if ((row - 2) >= 0 && Board[row - 2][col] == 0)
		{
			Mine[row - 1][col] = -1;
			if (Board[row - 1][col] != 0) DetectJump(row - 2, col, is_black, 1);
		}

		if ((row + 2) <= 7 && Board[row + 2][col] == 0)
		{
			Mine[row + 1][col] = -1;
			if (Board[row + 1][col] != 0) DetectJump(row + 2, col, is_black, 2);
		}

		if ((col - 2) >= 0 && Board[row][col - 2] == 0)
		{
			Mine[row][col - 1] = -1;
			if (Board[row][col - 1] != 0) DetectJump(row, col - 2, is_black, 3);
		}

	}
	//RIGHT
	else if (dir == 4)
	{
		if ((row - 2) >= 0 && Board[row - 2][col] == 0)
		{
			Mine[row - 1][col] = -1;
			if (Board[row - 1][col] != 0) DetectJump(row - 2, col, is_black, 1);
		}

		if ((row + 2) <= 7 && Board[row + 2][col] == 0)
		{
			Mine[row + 1][col] = -1;
			if (Board[row + 1][col] != 0) DetectJump(row + 2, col, is_black, 2);
		}

		if ((col + 2) <= 7 && Board[row][col + 2] == 0)
		{
			Mine[row][col + 1] = -1;
			if (Board[row][col + 1] != 0)DetectJump(row, col - 2, is_black, 4);
		}
	}
}

void DetectMines(int row, int col, bool is_black)
{
	if (is_black)
	{
		if ((row - 1) >= 0 && (row + 1) < 8 && Board[row - 1][col] == 2 && Board[row + 1][col] == 0) Mine[row][col] = -1;
		if ((row - 1) >= 0 && (row + 1) < 8 && Board[row + 1][col] == 2 && Board[row - 1][col] == 0) Mine[row][col] = -1;
		if ((col - 1) >= 0 && (col + 1) < 8 && Board[row][col - 1] == 2 && Board[row][col + 1] == 0) Mine[row][col] = -1;
		if ((col - 1) >= 0 && (col + 1) < 8 && Board[row][col + 1] == 2 && Board[row][col - 1] == 0) Mine[row][col] = -1;
		if ((row - 2) >= 0 && Board[row - 2][col] == 0 && Board[row - 1][col] == 2 && Board[row][col] == 2) DetectJump(row - 2, col, is_black, 1);
		if ((row + 2) <= 7 && Board[row + 2][col] == 0 && Board[row + 1][col] == 2 && Board[row][col] == 2) DetectJump(row + 2, col, is_black, 2);
		if ((col - 2) >= 0 && Board[row][col - 2] == 0 && Board[row][col - 1] == 2 && Board[row][col] == 2) DetectJump(row, col - 2, is_black, 3);
		if ((col + 2) >= 0 && Board[row][col + 2] == 0 && Board[row][col + 1] == 2 && Board[row][col] == 2) DetectJump(row, col + 2, is_black, 4);
	}

	else
	{
		if ((row - 1) >= 0 && (row + 1) < 8 && Board[row - 1][col] == 1 && Board[row + 1][col] == 0) Mine[row][col] = -1;
		if ((row - 1) >= 0 && (row + 1) < 8 && Board[row + 1][col] == 1 && Board[row - 1][col] == 0) Mine[row][col] = -1;
		if ((col - 1) >= 0 && (col + 1) < 8 && Board[row][col - 1] == 1 && Board[row][col + 1] == 0) Mine[row][col] = -1;
		if ((col - 1) >= 0 && (col + 1) < 8 && Board[row][col + 1] == 1 && Board[row][col - 1] == 0) Mine[row][col] = -1;
		if ((row - 2) >= 0 && Board[row - 2][col] == 0 && Board[row - 1][col] == 1 && Board[row][col] == 1) DetectJump(row - 2, col, is_black, 1);
		if ((row + 2) <= 7 && Board[row + 2][col] == 0 && Board[row + 1][col] == 1 && Board[row][col] == 1) DetectJump(row + 2, col, is_black, 2);
		if ((col - 2) >= 0 && Board[row][col - 2] == 0 && Board[row][col - 1] == 1 && Board[row][col] == 1) DetectJump(row, col - 2, is_black, 3);
		if ((col + 2) >= 0 && Board[row][col + 2] == 0 && Board[row][col + 1] == 1 && Board[row][col] == 1) DetectJump(row, col + 2, is_black, 4);
	}

}

void MapMines(bool is_black)
{
	//Initialize
	for (int row = 0; row < 8; row++)
	{
		for (int col = 0; col < 8; col++)
		{
			Mine[row][col] = 0;
		}
	}

	//Calculate Mine
	for (int row = 0; row < 8; row++)
	{
		for (int col = 0; col < 8; col++)
		{
			DetectMines(row, col, is_black);
		}
	}

}

//-poa"3o¡è@¡§B|paG¡§?¡èU£¤h?¨¢"aoevalution function
int Evalfunct(int row, int col, bool is_black) {
	int score = 0, eat_score = 0;
	int color;

	if (is_black) color = 1;										//¨¤??d?C|a
	else color = 2;

	if (Mine[row][col] == -1) score -= 100;							//¡¤|3Q|Y

	if (row > 0 && Board[row - 1][col] == color) score += 10;		//?P3¨°|3¡§S|3¡è¨ª-x
	if (row < 7 && Board[row + 1][col] == color) score += 10;
	if (col > 0 && Board[row][col - 1] == color) score += 30;
	if (col < 7 && Board[row][col + 1] == color) score += 30;

	if (row == 0 || row == 7) score += 10;							//?O¡ì_?a¨¤eor?a¡§¡è?¡§
	if (color == 1)
		if ((row == 0 || row == 7) && col == 7) score += 100;
		else if (col == 7) score += 20;
	if (color == 2)
		if ((row == 0 || row == 7) && col == 0) score += 100;
		else if (col == 0) score += 20;

	if (color == 1) score = score + 10 * col;							//?¡Â¡Ào¡è¨¤¡ã?ao?Z?¡Â
	else score = score + 10 * (7 - col);

	return score;
}

vector<vector<int>> Movement(int row, int col, int dir, bool is_jump, bool is_black, vector<vector<int>> rec) {
	vector<vector<int>> temp, max_rec;
	rec.push_back({ row,col });
	int max_score;
	//BLACK
	if (is_black)
	{
		//Only for consecutive jump 
		if (is_jump && (((row - 1) >= 0 && Board[row - 1][col] != 0) || ((row + 1 < 8) && Board[row + 1][col] != 0) || ((col + 1) < 8 && Board[row][col + 1] != 0)))
		{
			//UP
			if (dir == 0) {
				//GO UP if available
				if ((row - 2) >= 0 && Board[row - 1][col] == 1 && Board[row - 2][col] == 0) temp = Movement(row - 2, col, 0, 1, is_black, rec);
				else if ((row - 2) >= 0 && Board[row - 1][col] == 2 && Board[row - 2][col] == 0) { temp = Movement(row - 2, col, 0, 1, is_black, rec); store_score += 100; }
				max_score = store_score; store_score = 0; max_rec = temp;
				//GO RIGHT if available
				if ((col + 2) < 8 && Board[row][col + 1] == 1 && Board[row][col + 2] == 0) temp = Movement(row, col + 2, 3, 1, is_black, rec);
				else if ((col + 2) < 8 && Board[row][col + 1] == 2 && Board[row][col + 2] == 0) { temp = Movement(row, col + 2, 3, 1, is_black, rec); store_score += 100; }
				if (max_score < store_score) { max_score = store_score;  max_rec = temp; }
				store_score = max_score;
			}
			//DOWN
			else if (dir == 1) {
				//GO DOWN if available
				if ((row + 2) < 8 && Board[row + 1][col] == 1 && Board[row + 2][col] == 0) temp = Movement(row + 2, col, 1, 1, is_black, rec);
				else if ((row + 2) < 8 && Board[row + 1][col] == 2 && Board[row + 2][col] == 0) { temp = Movement(row + 2, col, 1, 1, is_black, rec); store_score += 100; }
				max_score = store_score; store_score = 0; max_rec = temp;
				//GO RIGHT if available
				if ((col + 2) < 8 && Board[row][col + 1] == 1 && Board[row][col + 2] == 0) temp = Movement(row, col + 2, 3, 1, is_black, rec);
				else if ((col + 2) < 8 && Board[row][col + 1] == 2 && Board[row][col + 2] == 0) { temp = Movement(row, col + 2, 3, 1, is_black, rec); store_score += 100; }
				if (max_score < store_score) { max_score = store_score;  max_rec = temp; }
				store_score = max_score;
			}
			//RIGHT
			else if (dir == 3) {
				//GO UP if available
				if ((row - 2) >= 0 && Board[row - 1][col] == 1 && Board[row - 2][col] == 0) temp = Movement(row - 2, col, 0, 1, is_black, rec);
				else if ((row - 2) >= 0 && Board[row - 1][col] == 2 && Board[row - 2][col] == 0) { temp = Movement(row - 2, col, 0, 1, is_black, rec); store_score += 100; }
				max_score = store_score; store_score = 0; max_rec = temp;
				//GO DOWN if available
				if ((row + 2) < 8 && Board[row + 1][col] == 1 && Board[row + 2][col] == 0) temp = Movement(row + 2, col, 1, 1, is_black, rec);
				else if ((row + 2) < 8 && Board[row + 1][col] == 2 && Board[row + 2][col] == 0) { temp = Movement(row + 2, col, 1, 1, is_black, rec); store_score += 100; }
				if (max_score < store_score) { max_score = store_score;  max_rec = temp; }
				store_score = 0;
				//GO RIGHT if available
				if ((col + 2) < 8 && Board[row][col + 1] == 1 && Board[row][col + 2] == 0) temp = Movement(row, col + 2, 3, 1, is_black, rec);
				else if ((col + 2) < 8 && Board[row][col + 1] == 2 && Board[row][col + 2] == 0) { temp = Movement(row, col + 2, 3, 1, is_black, rec); store_score += 100; }
				if (max_score < store_score) { max_score = store_score;  max_rec = temp; }
				store_score = max_score;
			}
			//Return best route
			return max_rec;
		}
		//Cannot Move or Jump anymore
		else
		{
			store_score += Evalfunct(row, col, is_black);
			return rec;
		}
	}
	//WHITE
	else
	{
		//Only for consecutive jump 
		if (is_jump && (((row - 1) >= 0 && Board[row - 1][col] != 0) || ((row + 1 < 8) && Board[row + 1][col] != 0) || ((col - 1) >= 0 && Board[row][col - 1] != 0)))
		{
			//UP
			if (dir == 0) {
				//GO UP if available
				if ((row - 2) >= 0 && Board[row - 1][col] == 2 && Board[row - 2][col] == 0) temp = Movement(row - 2, col, 0, 1, is_black, rec);
				else if ((row - 2) >= 0 && Board[row - 1][col] == 1 && Board[row - 2][col] == 0) { temp = Movement(row - 2, col, 0, 1, is_black, rec); store_score += 100; }
				max_score = store_score; store_score = 0; max_rec = temp;
				//GO LEFT if available
				if ((col - 2) >= 0 && Board[row][col - 1] == 2 && Board[row][col - 2] == 0) temp = Movement(row, col - 2, 2, 1, is_black, rec);
				else if ((col - 2) >= 0 && Board[row][col - 1] == 1 && Board[row][col - 2] == 0) { temp = Movement(row, col - 2, 2, 1, is_black, rec); store_score += 100; }
				if (max_score < store_score) { max_score = store_score;  max_rec = temp; }
				store_score = max_score;
			}
			//DOWN
			else if (dir == 1) {
				//GO DOWN if available
				if ((row + 2) < 8 && Board[row + 1][col] == 2 && Board[row + 2][col] == 0) temp = Movement(row + 2, col, 1, 1, is_black, rec);
				else if ((row + 2) < 8 && Board[row + 1][col] == 1 && Board[row + 2][col] == 0) { temp = Movement(row + 2, col, 1, 1, is_black, rec); store_score += 100; }
				max_score = store_score; store_score = 0; max_rec = temp;
				//GO LEFT if available
				if ((col - 2) >= 0 && Board[row][col - 1] == 2 && Board[row][col - 2] == 0) temp = Movement(row, col - 2, 2, 1, is_black, rec);
				else if ((col - 2) >= 0 && Board[row][col - 1] == 1 && Board[row][col - 2] == 0) { temp = Movement(row, col - 2, 2, 1, is_black, rec); store_score += 100; }
				if (max_score < store_score) { max_score = store_score;  max_rec = temp; }
				store_score = max_score;
			}
			//LEFT
			else if (dir == 2) {
				//GO UP if available
				if ((row - 2) >= 0 && Board[row - 1][col] == 2 && Board[row - 2][col] == 0) temp = Movement(row - 2, col, 0, 1, is_black, rec);
				else if ((row - 2) >= 0 && Board[row - 1][col] == 1 && Board[row - 2][col] == 0) { temp = Movement(row - 2, col, 0, 1, is_black, rec); store_score += 100; }
				max_score = store_score; store_score = 0; max_rec = temp;
				//GO DOWN if available
				if ((row + 2) < 8 && Board[row + 1][col] == 2 && Board[row + 2][col] == 0) temp = Movement(row + 2, col, 1, 1, is_black, rec);
				else if ((row + 2) < 8 && Board[row + 1][col] == 1 && Board[row + 2][col] == 0) { temp = Movement(row + 2, col, 1, 1, is_black, rec); store_score += 100; }
				if (max_score < store_score) { max_score = store_score;  max_rec = temp; }
				store_score = 0;
				//GO LEFT if available
				if ((col - 2) >= 0 && Board[row][col - 1] == 2 && Board[row][col - 2] == 0) temp = Movement(row, col - 2, 2, 1, is_black, rec);
				else if ((col - 2) >= 0 && Board[row][col - 1] == 1 && Board[row][col - 2] == 0) { temp = Movement(row, col - 2, 2, 1, is_black, rec); store_score += 100; }
				if (max_score < store_score) { max_score = store_score;  max_rec = temp; }
				store_score = max_score;
			}
			//Return best route
			return max_rec;
		}
		//Cannot Move or Jump anymore
		else
		{
			store_score += Evalfunct(row, col, is_black);
			return rec;
		}
	}
}

//¨¤??d2?¡ã¨º¡§¨¬-t¡è@|¨¬?m¡¤|3¨¬|n?A¡§?-¡Á¡ì?Step vector
vector<vector<int>> Move_Piece(int row, int col, bool is_jump, bool is_black) {
	//cout << "[" << row << "," << col << "] , Done Jump:" << is_jump << endl;

	//Initialize
	store_score = 0;
	Step.clear();
	int max_score = 0;
	vector<vector<int>> rec, temp, max_rec;
	rec.push_back({ row,col });

	//Black
	if (is_black)
	{
		//GO UP if available
		if ((row - 1) >= 0 && Board[row - 1][col] == 0) temp = Movement(row - 1, col, 0, 0, is_black, rec);
		else if ((row - 2) >= 0 && Board[row - 1][col] == 1 && Board[row - 2][col] == 0) temp = Movement(row - 2, col, 0, 1, is_black, rec);
		else if ((row - 2) >= 0 && Board[row - 1][col] == 2 && Board[row - 2][col] == 0) { temp = Movement(row - 2, col, 0, 1, is_black, rec); store_score += 100; }
		max_score = store_score; store_score = 0; max_rec = temp;
		//GO DOWN if available
		if ((row + 1) < 8 && Board[row + 1][col] == 0) temp = Movement(row + 1, col, 1, 0, is_black, rec);
		else if ((row + 2) < 8 && Board[row + 1][col] == 1 && Board[row + 2][col] == 0) temp = Movement(row + 2, col, 1, 1, is_black, rec);
		else if ((row + 2) < 8 && Board[row + 1][col] == 2 && Board[row + 2][col] == 0) { temp = Movement(row + 2, col, 1, 1, is_black, rec); store_score += 100; }
		if (max_score < store_score) { max_score = store_score;  max_rec = temp; }
		store_score = 0;
		//GO RIGHT if available
		if ((col + 1) < 8 && Board[row][col + 1] == 0) temp = Movement(row, col + 1, 2, 0, is_black, rec);
		else if ((col + 2) < 8 && Board[row][col + 1] == 1 && Board[row][col + 2] == 0) temp = Movement(row, col + 2, 2, 1, is_black, rec);
		else if ((col + 2) < 8 && Board[row][col + 1] == 2 && Board[row][col + 2] == 0) { temp = Movement(row, col + 2, 2, 1, is_black, rec); store_score += 100; }
		if (max_score < store_score) { max_score = store_score;  max_rec = temp; }
		store_score = max_score;
		return max_rec;
	}
	//White
	else {
		//GO UP if available
		if ((row - 1) >= 0 && Board[row - 1][col] == 0) temp = Movement(row - 1, col, 0, 0, is_black, rec);
		else if ((row - 2) >= 0 && Board[row - 1][col] == 2 && Board[row - 2][col] == 0) temp = Movement(row - 2, col, 0, 1, is_black, rec);
		else if ((row - 2) >= 0 && Board[row - 1][col] == 1 && Board[row - 2][col] == 0) { temp = Movement(row - 2, col, 0, 1, is_black, rec); store_score += 100; }
		max_score = store_score; store_score = 0; max_rec = temp;
		//GO DOWN if available
		if ((row + 1) < 8 && Board[row + 1][col] == 0) temp = Movement(row + 1, col, 1, 0, is_black, rec);
		else if ((row + 2) < 8 && Board[row + 1][col] == 2 && Board[row + 2][col] == 0) temp = Movement(row + 2, col, 1, 1, is_black, rec);
		else if ((row + 2) < 8 && Board[row + 1][col] == 1 && Board[row + 2][col] == 0) { temp = Movement(row + 2, col, 1, 1, is_black, rec); store_score += 100; }
		if (max_score < store_score) { max_score = store_score;  max_rec = temp; }
		store_score = 0;
		//GO LEFT if available
		if ((col - 1) >= 0 && Board[row][col - 1] == 0) temp = Movement(row, col - 1, 3, 0, is_black, rec);
		else if ((col - 2) >= 0 && Board[row][col - 1] == 2 && Board[row][col - 2] == 0) temp = Movement(row, col - 2, 3, 1, is_black, rec);
		else if ((col - 2) >= 0 && Board[row][col - 1] == 1 && Board[row][col - 2] == 0) { temp = Movement(row, col - 2, 3, 1, is_black, rec); store_score += 100; }
		if (max_score < store_score) { max_score = store_score;  max_rec = temp; }
		store_score = max_score;
		return max_rec;
	}

}

//2?¡ã¨º¡ä?¡èl
void ChangeBoard(int row, int col, int new_row, int new_col)
{
	int temp;
	temp = Board[row][col];
	Board[row][col] = 0;
	Board[new_row][new_col] = temp;
}

void JumpForward(int row, int col, bool is_black)
{
	if (is_black)
	{
		if (Board[row][col + 1] != 0)
		{
			while (Board[row][col + 1] != 0)
			{
				int new_row = row;
				int new_col = col + 2;
				ChangeBoard(row, col, new_row, new_col);
			}
		}

		else
		{
			int new_row = row;
			int new_col = col + 1;
			ChangeBoard(row, col, new_row, new_col);
		}
	}

	else
	{
		if (Board[row][col - 1] != 0)
		{
			while (Board[row][col - 1] != 0)
			{
				int new_row = row;
				int new_col = col - 2;
				ChangeBoard(row, col, new_row, new_col);
			}
		}

		else
		{
			int new_row = row;
			int new_col = col - 1;
			ChangeBoard(row, col, new_row, new_col);
		}
	}
}

//?}¡ì?
vector<vector<int>> opening(vector<vector<int>>& step, bool is_black, int round) {
	if (is_black) {
		if (round == 0) {
			step.push_back({ 1,1 });
			step.push_back({ 0,1 });
		}
		if (round == 1) {
			step.push_back({ 2,0 });
			step.push_back({ 2,1 });
		}
		if (round == 2) {
			step.push_back({ 4,0 });
			step.push_back({ 4,1 });
		}
		if (round == 3) {
			step.push_back({ 5,1 });
			step.push_back({ 6,1 });
		}
	}
	else {
		if (round == 0) {
			step.push_back({ 2,6 });
			step.push_back({ 1,6 });
		}
		if (round == 1) {
			step.push_back({ 3,7 });
			step.push_back({ 3,6 });
		}
		if (round == 2) {
			step.push_back({ 5,7 });
			step.push_back({ 5,6 });
		}
		if (round == 3) {
			step.push_back({ 6,6 });
			step.push_back({ 7,6 });
		}
	}
	return step;
}

//-poa¡§a-¨®¡ä?¡èlao?Z?¡Â
int piece_distance(int row1, int row2, int col1, int col2) {
	int distance = 0;
	if (col1 == col2) distance += 2;

	distance = distance + 2 * abs(row1 - row2) + abs(col1 - col2);
	return distance;
}

//¨¤??d|3¦ÌL£¤H¡ãt1?ao¡ä?¡èl
int Check_Piece_Pair(int row, int col, bool is_black) {
	if (is_black) {
		if (Board[row][col - 1] == 1 || Board[row][col + 1] == 1) {
			return 0;
		}
	}
	else {
		if (Board[row][col - 1] == 2 || Board[row][col + 1] == 2) {
			return 0;
		}
	}
	return 1;
}

//???¨¹¡Ày¡ãt1?ao¡ä?¡èl?A¡§?-¡Á¡ì?Pair vector
int Choose_Match_Piecese(bool is_black) {
	int distance = 9999, distance_tmp;                                     //distance between piecese
	piece piece1, piece2;												   //?¨¨?¨¦¡ãO?y¡Ày-poa?Z?¡Â¡è¡ì¡ä?¡èlao?y?D												   //3¨¬2¡Á¡Ày2?¡ã¨º¡è¡ì¡ä?¡èlao?y?D
	int count = 0, tmp_count, color, init_col;

	if (is_black == 0) {												   //¡ìP?_¡ä?¡èl?C|a
		color = 2;
		init_col = 7;
	}
	else {
		color = 1;
		init_col = 0;
	}

	while (1) {
		piece1.col = -1;
		piece2.col = -1;
		tmp_count = count;
		for (int row = 0; row < 8; row++) {								//¡äM¡ì?3¨¬¡èp?Z?¡Âao¡§a-¨®¡ä?¡èl
			for (int col = init_col; ;) {
				if (Board[row][col] == color && piece1.col < 0) {
					if (tmp_count == 0 && Check_Piece_Pair(row, col, is_black)) {
						piece1.row = row;
						piece1.col = col;
					}
					else if (tmp_count > 0) {
						tmp_count--;
					}
				}
				else if (Board[row][col] == color) {
					distance_tmp = piece_distance(piece1.row, row, piece1.col, col);
					if (distance_tmp < distance && distance_tmp > 1 && Check_Piece_Pair(row, col, is_black)) {
						distance = distance_tmp;
						piece2.row = row;
						piece2.col = col;
					}
				}
				if (color == 1) {												//?¨²?¨²¡ä?¡èl?C|a?A¡§M?w2¡Á¡è?¡À?£¤¨®&col?W¡ä?
					col++;
					if (col > 7) break;
				}
				else {
					col--;
					if (col < 0) break;
				}
			}
		}

		if (piece1.col < 0) break;									//£¤Na¨ª?¨°|3¡ä?¡èl3¡ê¡À?1L¡è@1M¡èF

		if (piece2.col >= 0) {										//???y?Z?¡Â3¨¬a?aoao¡§a-¨®¡ä?¡èl
			Pair.clear();
			Pair.push_back({ piece1.row, piece1.col });
			Pair.push_back({ piece2.row, piece2.col });
		}
		count++;
	}

	if (piece1.col >= 0) return 1;									//return 1 if |3¡ì?¡§¨¬¡è@1?¡ä?¡èl
	else return 0;
}

//Detect Horizontal Bridge
int DetectHBridge(vector<vector<int>>& Board, bool is_black) {
	int Wbridge[4] = { 0,2,0,2 };
	int Bbridge[4] = { 1,0,1,0 };

	for (int row = 0; row < Board.size(); row++)
	{
		if (is_black)
		{
			for (int col = 1; col < 5; col++)
			{
				int chk = 0;
				for (int idx = 0; idx < 4; idx++)
				{
					if (Board.at(row).at(col) == Wbridge[chk]) chk++;
				}
				if (chk == 4) return(row);
			}
		}

		else
		{
			for (int col = 1; col < 5; col++)
			{
				int chk = 0;
				for (int idx = 0; idx < 4; idx++)
				{
					if (Board.at(row).at(col) == Bbridge[chk]) chk++;
				}
				if (chk == 4) return(row);
			}
		}
	}
	return -1;
}

//Detect Diagonal Bridge
int DetectDBridge(vector<vector<int>>& Board, bool is_black) {
	for (int row = 0; row < 6; row++)
	{
		if (is_black)
		{
			for (int col = 1; col < 5; col++)
			{
				if (Board.at(row).at(col) == 2)
					if (Board.at(row + 1).at(col + 1) == 2)
						if (Board.at(row + 2).at(col + 2) == 2)
							return (row);

				if (Board.at(row).at(col + 2) == 2)
					if (Board.at(row + 1).at(col + 1) == 2)
						if (Board.at(row + 2).at(col) == 2)
							return (row + 2);
			}
		}

		else
		{
			for (int col = 1; col < 5; col++)
			{
				if (Board.at(row).at(col) == 1)
					if (Board.at(row + 1).at(col + 1) == 1)
						if (Board.at(row + 2).at(col + 2) == 1)
							return (row);

				if (Board.at(row).at(col + 2) == 1)
					if (Board.at(row + 1).at(col + 1) == 1)
						if (Board.at(row + 2).at(col) == 1)
							return (row + 2);
			}
		}
	}
	return -1;
}

void CopyBoard(vector<vector<int>> board)
{
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			Board[i][j] = board.at(i).at(j);
		}
	}

}

vector<vector<int>> CopyBoard2()
{
	vector<vector<int>> temp;
	for (int i = 0; i < 8; i++)
	{
		temp.push_back(vector<int>());
		for (int j = 0; j < 8; j++)
		{
			temp.at(i).push_back(Board[i][j]);
		}
	}
	return temp;
}

// Choose Match Piece only return bool , pair will get the shortest distance 2 chess coordinates
vector<vector<int>> GetStep(vector<vector<int>>& board, bool is_black, int n) {
	vector<vector<int>> step, final_step;
	int row, col, score = 0, tmp = 0;
	if (n < 4)
	{
		return opening(step, is_black, n);
	}

	else
	{
		MoveSet.clear();
		ScoreSet.clear();
		CopyBoard(board);
		MapMines(is_black);
		for (row = 0; row < 8; row++) {
			for (col = 0; col < 8; col++) {
				if (Board[row][col] == 1 && is_black == 1)
				{
					step = Move_Piece(row, col, 0, 1);
					MoveSet.push_back(step);
					ScoreSet.push_back(store_score);
				}
				else if (Board[row][col] == 2 && is_black == 0)
				{
					step = Move_Piece(row, col, 0, 0);
					MoveSet.push_back(step);
					ScoreSet.push_back(store_score);
				}
			}
		}

		int highest_score = 0, chk=0;
		for (int idx = 0; idx < ScoreSet.size(); idx++)
		{
			if (ScoreSet.at(idx) > highest_score) highest_score = ScoreSet.at(idx);
			cout << "Score: " << ScoreSet.at(idx) << ",";
			for (int setn = 0; setn < MoveSet.at(idx).size(); setn++)
			{
				cout << "(" << MoveSet.at(idx).at(setn).at(0) << "," << MoveSet.at(idx).at(setn).at(1) << ")";
				if (setn != MoveSet.at(idx).size() - 1) cout << " -> ";
			}
			cout << endl;
		}

		for (int idx = 0; idx < ScoreSet.size(); idx++)
		{
			for (int setn = 0; setn < MoveSet.at(idx).size(); setn++)
			{
				if (ScoreSet.at(idx) == highest_score) {
					final_step.push_back({ MoveSet.at(idx).at(setn).at(0),MoveSet.at(idx).at(setn).at(1) });
					chk = 1;
				}
			}
			if (chk) break;
		}
	}
	return final_step;
}

int main(void) {
	int n = 0;
	/*vector<vector<int>> board, step;
	bool is_black;
	PrintBoard();
	while (true) {
		step.clear();
		board = CopyBoard2();
		step = GetStep(board, 1, n); n++;
		for (int i = 0; i < step.size(); i++)
		{
			cout << "(" << step.at(i).at(0) << "," << step.at(i).at(1) << ")";
			if (i != step.size() - 1) cout << " ->";
		}
		cout << endl;
		ChangeBoard(step.at(0).at(0), step.at(0).at(1), step.at(1).at(0), step.at(1).at(1));
		PrintBoard();
		system("pause");
	}*/
	
	int id_package;
	vector<vector<int>> board, step;
	bool is_black;
	while (true) {
		step.clear();
		if (GetBoard(id_package, board, is_black))
			break;

		step = GetStep(board, is_black,n);
		n++;
		SendStep(id_package, step);
	}
}

