#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// board information
#define BOARD_SIZE 12
#define EMPTY 0
#define BLACK 1
#define WHITE 2

// bool
typedef int BOOL;
#define TRUE 1
#define FALSE 0

// option
typedef int OPTION;
#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3
#define UP_LEFT 4
#define UP_RIGHT 5
#define DOWN_LEFT 6
#define DOWN_RIGHT 7

#define MAX_BYTE 10000

#define START "START"
#define PLACE "PLACE"
#define TURN  "TURN"
#define END   "END"

struct Command
{
	int x;
	int y;
	OPTION option;
};

char buffer[MAX_BYTE] = { 0 };
char board[BOARD_SIZE][BOARD_SIZE] = { 0 };
int me_flag;
int other_flag;

const int DIR[8][2] = { {-1, 0}, {1, 0}, {0, -1}, {0, 1}, {-1, -1}, {-1, 1}, {1, -1}, {1, 1} };

void debug(const char *str) {
	printf("DEBUG %s\n", str);
	fflush(stdout);
}

void printBoard() {
	char visual_board[BOARD_SIZE][BOARD_SIZE] = { 0 };
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			if (board[i][j] == EMPTY) {
				visual_board[i][j] = '.';
			}
			else if (board[i][j] == BLACK) {
				visual_board[i][j] = 'O';
			}
			else if (board[i][j] == WHITE) {
				visual_board[i][j] = 'X';
			}
		}
		printf("%s\n", visual_board[i]);
	}
}

BOOL isInBound(int x, int y) {
	return x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE;
}

/**
 * YOUR CODE BEGIN
 * 你的代码开始
 */

typedef struct fsearunit {
	char fsear[BOARD_SIZE][BOARD_SIZE];
	int fx;
	int fy;
	int foption;
	float maplue;
	float eatlue;
	float cohelue;
	float attacklue;
	float eatenlue;
	float value;
} Fsearunit;

Fsearunit *layernum[20];

#define AA 0
#define BB 1
#define CC 2
#define DD 3
#define EE -1

int MAPLUE[BOARD_SIZE][BOARD_SIZE] =
{
	 {EE,EE,EE,EE,EE,EE,AA,AA,AA,AA,AA,AA},
	 {EE,EE,EE,EE,EE,EE,AA,AA,AA,AA,AA,AA},
	 {EE,EE,EE,EE,EE,BB,BB,BB,BB,AA,AA,AA},
	 {EE,EE,BB,BB,BB,BB,CC,CC,BB,AA,AA,AA},
	 {AA,AA,BB,CC,CC,CC,DD,CC,BB,BB,AA,AA},
	 {AA,AA,BB,CC,DD,DD,DD,CC,CC,BB,AA,AA},
	 {AA,AA,BB,CC,DD,DD,DD,DD,CC,BB,AA,AA},
	 {AA,AA,BB,BB,CC,DD,CC,CC,CC,BB,AA,AA},
	 {AA,AA,AA,BB,CC,CC,BB,BB,BB,BB,EE,EE},
	 {AA,AA,AA,BB,BB,BB,BB,EE,EE,EE,EE,EE},
	 {AA,AA,AA,AA,AA,AA,EE,EE,EE,EE,EE,EE},
	 {AA,AA,AA,AA,AA,AA,EE,EE,EE,EE,EE,EE}
};


//主程序使用的函数一（初始化） 
void initAI(int me)
{
}

//判断能否移动 
BOOL canmove(const char board[BOARD_SIZE][BOARD_SIZE], int color, int x, int y, int option)
{
	switch (option)
	{
	case UP:
	{
		if (isInBound(x - 1, y) && board[x - 1][y] == EMPTY)
			return TRUE;
		else
			return FALSE;
	}
	case DOWN:
	{
		if (isInBound(x + 1, y) && board[x + 1][y] == EMPTY)
			return TRUE;
		else
			return FALSE;
	}
	case LEFT:
	{
		if (isInBound(x, y - 1) && board[x][y - 1] == EMPTY)
			return TRUE;
		else
			return FALSE;
	}
	case RIGHT:
	{
		if (isInBound(x, y + 1) && board[x][y + 1] == EMPTY)
			return TRUE;
		else
			return FALSE;
	}
	case UP_LEFT:
	{
		if (isInBound(x - 1, y - 1) && board[x - 1][y - 1] == EMPTY)
			return TRUE;
		else
			return FALSE;
	}
	case UP_RIGHT:
	{
		if (isInBound(x - 1, y + 1) && board[x - 1][y + 1] == EMPTY)
			return TRUE;
		else
			return FALSE;
	}
	case DOWN_LEFT:
	{
		if (isInBound(x + 1, y - 1) && board[x + 1][y - 1] == EMPTY)
			return TRUE;
		else
			return FALSE;
	}
	case DOWN_RIGHT:
	{
		if (isInBound(x + 1, y + 1) && board[x + 1][y + 1] == EMPTY)
			return TRUE;
		else
			return FALSE;
	}
	default: {
		return FALSE;
	}
	}
}

//移动更改棋盘
void changeboard(char board[BOARD_SIZE][BOARD_SIZE], int color, int x, int y, int option)
{
	switch (option)
	{
	case UP:
	{
		board[x - 1][y] = color;
		board[x][y] = EMPTY;
		break;
	}
	case DOWN:
	{
		board[x + 1][y] = color;
		board[x][y] = EMPTY;
		break;
	}
	case LEFT:
	{
		board[x][y - 1] = color;
		board[x][y] = EMPTY;
		break;
	}
	case RIGHT:
	{
		board[x][y + 1] = color;
		board[x][y] = EMPTY;
		break;
	}
	case UP_LEFT:
	{
		board[x - 1][y - 1] = color;
		board[x][y] = EMPTY;
		break;
	}
	case UP_RIGHT:
	{
		board[x - 1][y + 1] = color;
		board[x][y] = EMPTY;
		break;
	}
	case DOWN_LEFT:
	{
		board[x + 1][y - 1] = color;
		board[x][y] = EMPTY;
		break;
	}
	case DOWN_RIGHT:
	{
		board[x + 1][y + 1] = color;
		board[x][y] = EMPTY;
		break;
	}
	}
}

//复制棋盘
void copyboard(char copyboard[BOARD_SIZE][BOARD_SIZE], const char board[BOARD_SIZE][BOARD_SIZE])
{
	int i, j;
	for (i = 0; i < BOARD_SIZE; i++)
		for (j = 0; j < BOARD_SIZE; j++)
		{
			copyboard[i][j] = board[i][j];
		}
}

//返回color走一步棋后的夹吃数并更新棋盘
int nip(char board[BOARD_SIZE][BOARD_SIZE], int x, int y, int option, int color)
{
	int nipnum = 0;
	switch (option)
	{
	case UP:
	{
		x = x - 1; break;
	}
	case DOWN:
	{
		x = x + 1; break;
	}
	case LEFT:
	{
		y = y - 1; break;
	}
	case RIGHT:
	{
		y = y + 1; break;
	}
	case UP_LEFT:
	{
		x = x - 1;
		y = y - 1;
		break;
	}
	case UP_RIGHT:
	{
		x = x - 1;
		y = y + 1;
		break;
	}
	case DOWN_LEFT:
	{
		x = x + 1;
		y = y - 1;
		break;
	}
	case DOWN_RIGHT:
	{
		x = x + 1;
		y = y + 1;
		break;
	}
	}
	if (board[x - 2][y] == color && board[x - 1][y] == 3 - color && isInBound(x - 2, y) && isInBound(x - 1, y)) {
		nipnum++;
		board[x - 1][y] = color;
	}
	if (board[x][y - 2] == color && board[x][y - 1] == 3 - color && isInBound(x, y - 2) && isInBound(x, y - 1)) {
		nipnum++;
		board[x][y - 1] = color;
	}
	if (board[x + 2][y] == color && board[x + 1][y] == 3 - color && isInBound(x + 2, y) && isInBound(x + 1, y)) {
		nipnum++;
		board[x + 1][y] = color;
	}
	if (board[x][y + 2] == color && board[x][y + 1] == 3 - color && isInBound(x, y + 2) && isInBound(x, y + 1)) {
		nipnum++;
		board[x][y + 1] = color;
	}
	if (board[x - 2][y - 2] == color && board[x - 1][y - 1] == 3 - color && isInBound(x - 2, y - 2) && isInBound(x - 1, y - 1)) {
		nipnum++;
		board[x - 1][y - 1] = color;
	}
	if (board[x + 2][y - 2] == color && board[x + 1][y - 1] == 3 - color && isInBound(x + 2, y - 2) && isInBound(x + 1, y - 1)) {
		nipnum++;
		board[x + 1][y - 1] = color;
	}
	if (board[x + 2][y + 2] == color && board[x + 1][y + 1] == 3 - color && isInBound(x + 2, y + 2) && isInBound(x + 1, y + 1)) {
		nipnum++;
		board[x + 1][y + 1] = color;
	}
	if (board[x - 2][y + 2] == color && board[x - 1][y + 1] == 3 - color && isInBound(x - 2, y + 2) && isInBound(x - 1, y + 1)) {
		nipnum++;
		board[x - 1][y + 1] = color;
	}
	return nipnum;
}

//返回color走一步棋后的挑吃数并更新棋盘
int pick(char board[BOARD_SIZE][BOARD_SIZE], int x, int y, int option, int color)
{
	int picknum = 0;
	switch (option)
	{
	case UP:
	{
		x = x - 1; break;
	}
	case DOWN:
	{
		x = x + 1; break;
	}
	case LEFT:
	{
		y = y - 1; break;
	}
	case RIGHT:
	{
		y = y + 1; break;
	}
	case UP_LEFT:
	{
		x = x - 1;
		y = y - 1;
		break;
	}
	case UP_RIGHT:
	{
		x = x - 1;
		y = y + 1;
		break;
	}
	case DOWN_LEFT:
	{
		x = x + 1;
		y = y - 1;
		break;
	}
	case DOWN_RIGHT:
	{
		x = x + 1;
		y = y + 1;
		break;
	}
	}
	if (board[x - 1][y] == 3 - color && board[x + 1][y] == 3 - color && isInBound(x - 1, y) && isInBound(x + 1, y)) {
		picknum += 2;
		board[x - 1][y] = color;
		board[x + 1][y] = color;
	}
	if (board[x][y - 1] == 3 - color && board[x][y + 1] == 3 - color && isInBound(x, y - 1) && isInBound(x, y + 1)) {
		picknum += 2;
		board[x][y - 1] = color;
		board[x][y + 1] = color;
	}
	if (board[x - 1][y - 1] == 3 - color && board[x + 1][y + 1] == 3 - color && isInBound(x - 1, y - 1) && isInBound(x + 1, y + 1)) {
		picknum += 2;
		board[x - 1][y - 1] = color;
		board[x + 1][y + 1] = color;
	}
	if (board[x - 1][y + 1] == 3 - color && board[x + 1][y - 1] == 3 - color && isInBound(x - 1, y + 1) && isInBound(x + 1, y - 1)) {
		picknum += 2;
		board[x - 1][y + 1] = color;
		board[x + 1][y - 1] = color;
	}
	return picknum;
}

//color的抱团能力(返回一个整数反映当前局面的凝聚力） 
int cohere(const char board[BOARD_SIZE][BOARD_SIZE], int color)    /*const */
{
	int i, j;
	int coherence = 0;
	char copy[BOARD_SIZE][BOARD_SIZE];
	copyboard(copy, board);
	for (i = 0; i < BOARD_SIZE; i++) {
		for (j = 0; j < BOARD_SIZE; j++) {
			if (copy[i][j] == color) {
				if (copy[i + 1][j] == color && isInBound(i + 1, j)) coherence++;
				if (copy[i - 1][j] == color && isInBound(i - 1, j)) coherence++;
				if (copy[i][j + 1] == color && isInBound(i, j + 1)) coherence++;
				if (copy[i][j - 1] == color && isInBound(i, j - 1)) coherence++;
				if (copy[i + 1][j + 1] == color && isInBound(i + 1, j + 1)) coherence++;
				if (copy[i + 1][j - 1] == color && isInBound(i + 1, j - 1)) coherence++;
				if (copy[i - 1][j + 1] == color && isInBound(i - 1, j + 1)) coherence++;
				if (copy[i - 1][j - 1] == color && isInBound(i - 1, j - 1)) coherence++;
				copy[i][j] = EMPTY;
			}
		}
	}
	return coherence;
}

//返回当前棋盘color的个数
int countn(const char board[BOARD_SIZE][BOARD_SIZE], int color)      /*const */
{
	int x, y;
	int n = 0;
	for (x = 0; x < BOARD_SIZE; x++) {
		for (y = 0; y < BOARD_SIZE; y++) {
			if (board[x][y] == color)
				n++;
		}
	}
	return n;
}

//评估当前棋盘棋子的地图值(返回一个float)
float judgemap(const char board[BOARD_SIZE][BOARD_SIZE], int color)
{
	int x, y;
	int plue = 0;
	float maplue1 = 0;

	for (x = 0; x < BOARD_SIZE; x++) {
		for (y = 0; y < BOARD_SIZE; y++) {
			if (board[x][y] == color)
				plue += MAPLUE[x][y];
		}
	}
	maplue1 = (float)plue / 144;
	return maplue1;
}

//评估棋盘棋子抱团值(返回一个float) 
float judgecohe(int cohelity)
{
	float colue = 0;
	colue = cohelity / 4.0;
	return colue;
}

//color的下一步全棋盘吃子值(夹)
int nipnext(const char board[BOARD_SIZE][BOARD_SIZE], int color)
{
	int i, j;
	int m, n;
	int nipnum2 = 0;
	for (i = 0; i < BOARD_SIZE; i++)
	{
		for (j = 0; j < BOARD_SIZE; j++)
		{
			if (board[i][j] == 3 - color)
			{
				int flag1 = 0;
				if (board[i - 1][j] == color && isInBound(i + 1, j) && board[i + 1][j] == EMPTY)
				{
					for (m = i; m <= i + 2; m++)
					{
						for (n = j - 1; n <= j + 1; n++)
						{
							if (isInBound(m, n))
							{
								if (board[m][n] == color) flag1++;
							}
						}
					}
				}
				if (board[i + 1][j] == color && isInBound(i - 1, j) && board[i - 1][j] == EMPTY)
				{
					for (m = i - 2; m <= i; m++)
					{
						for (n = j - 1; n <= j + 1; n++)
						{
							if (isInBound(m, n))
							{
								if (board[m][n] == color) flag1++;
							}
						}
					}
				}
				if (board[i][j - 1] == color && isInBound(i, j + 1) && board[i][j + 1] == EMPTY)
				{
					for (m = i - 1; m <= i + 1; m++)
					{
						for (n = j; n <= j + 2; n++)
						{
							if (isInBound(m, n))
							{
								if (board[m][n] == color) flag1++;
							}
						}
					}
				}
				if (board[i][j + 1] == color && isInBound(i, j - 1) && board[i][j - 1] == EMPTY)
				{
					for (m = i - 1; m <= i + 1; m++)
					{
						for (n = j - 2; n <= j; n++)
						{
							if (isInBound(m, n))
							{
								if (board[m][n] == color) flag1++;
							}
						}
					}
				}
				if (board[i - 1][j + 1] == color && isInBound(i + 1, j - 1) && board[i + 1][j - 1] == EMPTY)
				{
					for (m = i; m <= i + 2; m++)
					{
						for (n = j - 2; n <= j; n++)
						{
							if (isInBound(m, n))
							{
								if (board[m][n] == color) flag1++;
							}
						}
					}
				}
				if (board[i + 1][j - 1] == color && isInBound(i - 1, j + 1) && board[i - 1][j + 1] == EMPTY)
				{
					for (m = i - 2; m <= i; m++)
					{
						for (n = j; n <= j + 2; n++)
						{
							if (isInBound(m, n))
							{
								if (board[m][n] == color) flag1++;
							}
						}
					}
				}
				if (board[i - 1][j - 1] == color && isInBound(i + 1, j + 1) && board[i + 1][j + 1] == EMPTY)
				{
					for (m = i; m <= i + 2; m++)
					{
						for (n = j; n <= j + 2; n++)
						{
							if (isInBound(m, n))
							{
								if (board[m][n] == color) flag1++;
							}
						}
					}
				}
				if (board[i + 1][j + 1] == color && isInBound(i - 1, j - 1) && board[i - 1][j - 1] == EMPTY)
				{
					for (m = i - 2; m <= i; m++)
					{
						for (n = j - 2; n <= j; n++)
						{
							if (isInBound(m, n))
							{
								if (board[m][n] == color) flag1++;
							}
						}
					}
				}
				if (flag1 != 0) nipnum2++;
			}
		}
	}
	return nipnum2;
}

//color的下一步全棋盘吃子值(挑)
int picknext(const char board[BOARD_SIZE][BOARD_SIZE], int color)
{
	int i, j;
	int m, n;
	char copy[BOARD_SIZE][BOARD_SIZE];
	copyboard(copy, board);
	int picknum2 = 0;
	for (i = 0; i < BOARD_SIZE; i++)
	{
		for (j = 0; j < BOARD_SIZE; j++)
		{
			if (copy[i][j] == 3 - color)
			{
				int flag = 0;
				if (isInBound(i, j + 2) && copy[i][j + 2] == 3 - color && copy[i][j + 1] == EMPTY)
				{
					int flag2 = 0;
					for (m = i - 1; m <= i + 1; m++)
					{
						for (n = j; n <= j + 2; n++)
						{
							if (isInBound(m, n))
							{
								if (copy[m][n] == color) flag2++;
							}
						}
					}
					if (flag2 != 0) flag++;
					if (flag2 != 0 && flag == 1) picknum2 = picknum2 + 2;
					if (flag2 != 0 && flag != 1) picknum2++;
					if (flag2 != 0) copy[i][j + 2] = EMPTY;
				}
				if (isInBound(i + 2, j + 2) && copy[i + 2][j + 2] == 3 - color && copy[i + 1][j + 1] == EMPTY)
				{
					int flag2 = 0;
					int flag3 = 0;
					for (m = i; m <= i + 2; m++)
					{
						for (n = j; n <= j + 2; n++)
						{
							if (isInBound(m, n))
							{
								if (copy[m][n] == color) flag2++;
							}
						}
					}
					if (flag2 != 0) flag++;
					if (flag2 != 0 && flag == 1) picknum2 = picknum2 + 2;
					if (flag2 != 0 && flag != 1) picknum2++;
					if (flag2 != 0) copy[i + 2][j + 2] = EMPTY;
				}
				if (isInBound(i + 2, j) && copy[i + 2][j] == 3 - color && copy[i + 1][j] == EMPTY)
				{
					int flag2 = 0;
					for (m = i; m <= i + 2; m++)
					{
						for (n = j - 1; n <= j + 1; n++)
						{
							if (isInBound(m, n))
							{
								if (copy[m][n] == color) flag2++;
							}
						}
					}
					if (flag2 != 0) flag++;
					if (flag2 != 0 && flag == 1) picknum2 = picknum2 + 2;
					if (flag2 != 0 && flag != 1) picknum2++;
					if (flag2 != 0) copy[i + 2][j] = EMPTY;
				}
				if (isInBound(i + 2, j - 2) && copy[i + 2][j - 2] == 3 - color && copy[i + 1][j - 1] == EMPTY)
				{
					int flag2 = 0;
					for (m = i; m <= i + 2; m++)
					{
						for (n = j - 2; n <= j; n++)
						{
							if (isInBound(m, n))
							{
								if (copy[m][n] == color) flag2++;
							}
						}
					}
					if (flag2 != 0) flag++;
					if (flag2 != 0 && flag == 1) picknum2 = picknum2 + 2;
					if (flag2 != 0 && flag != 1) picknum2++;
					if (flag2 != 0) copy[i + 2][j - 2] = EMPTY;
				}
				if (isInBound(i, j - 2) && copy[i][j - 2] == 3 - color && copy[i][j - 1] == EMPTY)
				{
					int flag2 = 0;
					for (m = i - 1; m <= i + 1; m++)
					{
						for (n = j - 2; n <= j; n++)
						{
							if (isInBound(m, n))
							{
								if (copy[m][n] == color) flag2++;
							}
						}
					}
					if (flag2 != 0) flag++;
					if (flag2 != 0 && flag == 1) picknum2 = picknum2 + 2;
					if (flag2 != 0 && flag != 1) picknum2++;
					if (flag2 != 0) copy[i][j - 2] = EMPTY;
				}
				if (isInBound(i - 2, j - 2) && copy[i - 2][j - 2] == 3 - color && copy[i - 1][j - 1] == EMPTY)
				{
					int flag2 = 0;
					for (m = i - 2; m <= i; m++)
					{
						for (n = j - 2; n <= j; n++)
						{
							if (isInBound(m, n))
							{
								if (copy[m][n] == color) flag2++;
							}
						}
					}
					if (flag2 != 0) flag++;
					if (flag2 != 0 && flag == 1) picknum2 = picknum2 + 2;
					if (flag2 != 0 && flag != 1) picknum2++;
					if (flag2 != 0) copy[i - 2][j - 2] = EMPTY;
				}
				if (isInBound(i - 2, j) && copy[i - 2][j] == 3 - color && copy[i - 1][j] == EMPTY)
				{
					int flag2 = 0;
					for (m = i - 2; m <= i; m++)
					{
						for (n = j - 1; n <= j + 1; n++)
						{
							if (isInBound(m, n))
							{
								if (copy[m][n] == color) flag2++;
							}
						}
					}
					if (flag2 != 0) flag++;
					if (flag2 != 0 && flag == 1) picknum2 = picknum2 + 2;
					if (flag2 != 0 && flag != 1) picknum2++;
					if (flag2 != 0) copy[i - 2][j] = EMPTY;
				}
				if (isInBound(i - 2, j + 2) && copy[i - 2][j + 2] == 3 - color && copy[i - 1][j + 1] == EMPTY)
				{
					int flag2 = 0;
					for (m = i - 2; m <= i; m++)
					{
						for (n = j; n <= j + 2; n++)
						{
							if (isInBound(m, n))
							{
								if (copy[m][n] == color) flag2++;
							}
						}
					}
					if (flag2 != 0) flag++;
					if (flag2 != 0 && flag == 1) picknum2 = picknum2 + 2;
					if (flag2 != 0 && flag != 1) picknum2++;
					if (flag2 != 0)copy[i - 2][j + 2] = EMPTY;
				}
				copy[i][j] = EMPTY;
			}
		}
	}
	return picknum2;
}

//评估棋盘的下一步吃子价值
float judgeattack(const char board[BOARD_SIZE][BOARD_SIZE], const char copyboard[BOARD_SIZE][BOARD_SIZE],int color)
{
	float niplue = 0;
	float picklue = 0;
	float attlue = 0;
	int minip;
	minip = nipnext(copyboard, color) - nipnext(board, color);
	switch (minip) {
	case 0: {
		niplue = 0;
		break;
	}
	case 1: {
		niplue = 0.14;
		break;
	}
	case 2: {
		niplue = 1.0;
		break;
	}
	case 3: {
		niplue = 1.0;
		break;
	}
	case 4: {
		niplue = 2.0;
		break;
	}
	case 5: {
		niplue = 2.0;
		break;
	}
	case 6: {
		niplue = 3.0;
		break;
	}
	case 7: {
		niplue = 3.0;
		break;
	}
	case 8: {
		niplue = 4.0;
		break;
	}
	case 9: {
		niplue = 4.0;
		break;
	}
	case 10: {
		niplue = 5.0;
		break;
	}
	case 11: {
		niplue = 5.0;
		break;
	}
	case 12: {
		niplue = 6.0;
		break;
	}
	case 13: {
		niplue = 6.0;
		break;
	}
	case 14: {
		niplue = 7.0;
		break;
	}
	case 15: {
		niplue = 7.0;
		break;
	}
	case 16: {
		niplue = 8.0;
		break;
	}
	}
	int mipick;
	mipick = picknext(copyboard, color) - picknext(board, color);
	switch (mipick) {
	case 0: {
		picklue = 0;
		break;
	}
	case 1: {
		picklue = 0.14;
		break;
	}
	case 2: {
		picklue = 0.14;
		break;
	}
	case 3: {
		picklue = 0.14;
		break;
	}
	case 4: {
		picklue = 2.0;
		break;
	}
	case 5: {
		picklue = 2.0;
		break;
	}
	case 6: {
		picklue = 3.0;
		break;
	}
	case 7: {
		picklue = 3.0;
		break;
	}
	case 8: {
		picklue = 4.0;
		break;
	}
	case 9: {
		picklue = 4.0;
		break;
	}
	case 10: {
		picklue = 5.0;
		break;
	}
	case 11: {
		picklue = 5.0;
		break;
	}
	case 12: {
		picklue = 6.0;
		break;
	}
	case 13: {
		picklue = 6.0;
		break;
	}
	case 14: {
		picklue = 7.0;
		break;
	}
	case 15: {
		picklue = 7.0;
		break;
	}
	case 16: {
		picklue = 8.0;
		break;
	}
	}
	attlue = niplue + picklue;
	return attlue;
}

//评估棋盘整体价值 
float judgetotal(float eatlue, float cohelue, float attacklue,float maplue,float eatenlue, int mn, int yn)
{
	float value = 0;
	if (mn > yn) value = eatlue + cohelue + 0.1*attacklue + maplue-eatenlue;
	if (mn <= yn) value = eatlue + cohelue + attacklue + maplue-eatenlue;
	return value;
}

//第二,三层搜索函数(返回一个棋盘的最大价值)
void searchboard2(const char board[BOARD_SIZE][BOARD_SIZE], int color, int recnum)
{
	recnum++;
	if (recnum >= 3) return;
	Fsearunit boardo[128];
	int x, y, option;
	int i = 0;
	int j;
	int mn, yn;
	Fsearunit maxvalue;
	for (x = 0; x < BOARD_SIZE; x++) {
		for (y = 0; y < BOARD_SIZE; y++) {
			if (board[x][y] == color) {
				for (option = 0; option < 8; option++) {
					if (canmove(board, color, x, y, option)) {
						copyboard(boardo[i].fsear, board);
						changeboard(boardo[i].fsear, color, x, y, option);
						boardo[i].fx = x;
						boardo[i].fy = y;
						boardo[i].foption = option;
						i++;
					}
				}
			}
		}
	}
	for (j = 0; j < i - 1; j++) {
		boardo[j].eatlue = pick(boardo[j].fsear, boardo[j].fx, boardo[j].fy, boardo[j].foption, color) + nip(boardo[j].fsear, boardo[j].fx, boardo[j].fy, boardo[j].foption, color);
		mn = countn((const char(*)[BOARD_SIZE])boardo[j].fsear, color);
		yn = countn((const char(*)[BOARD_SIZE])boardo[j].fsear, 3 - color);
		boardo[j].cohelue = judgecohe(cohere((const char(*)[BOARD_SIZE])boardo[j].fsear, color));
		boardo[j].attacklue = judgeattack(board, (const char(*)[BOARD_SIZE])boardo[j].fsear, color);
		boardo[j].maplue = judgemap((const char(*)[BOARD_SIZE])boardo[j].fsear, color);
		boardo[j].eatenlue = judgeattack(board, (const char(*)[BOARD_SIZE])boardo[j].fsear, 3 - color);
		boardo[j].value = judgetotal(boardo[j].eatlue, boardo[j].cohelue, boardo[j].attacklue, boardo[j].maplue, boardo[j].eatenlue, mn, yn);
	}
	maxvalue = boardo[0];
	for (j = 1; j < i - 1; j++) {
		if (boardo[j].value > maxvalue.value) maxvalue = boardo[j];
	}
	layernum[recnum - 1] = &maxvalue;
	for (j = 0; j < i - 1; j++) {
		int recnum2 = recnum;
		searchboard2((const char(*)[BOARD_SIZE])boardo[j].fsear, 3 - color, recnum2);
	}
}

//第一步搜索：遍历color棋子的所有可能走法 (同时用一个结构体复制走子后的棋盘方便二次搜索)
Fsearunit searchboard(const char board[BOARD_SIZE][BOARD_SIZE], int color)
{
	Fsearunit boardo[128];
	int x, y, option;
	int i = 0;
	int j;
	int mn, yn;
	Fsearunit maxvalue;
	for (x = 0; x < BOARD_SIZE; x++) {
		for (y = 0; y < BOARD_SIZE; y++) {
			if (board[x][y] == color) {
				for (option = 0; option < 8; option++) {
					if (canmove(board, color, x, y, option)) {
						copyboard(boardo[i].fsear, board);
						changeboard(boardo[i].fsear, color, x, y, option);
						boardo[i].fx = x;
						boardo[i].fy = y;
						boardo[i].foption = option;
						i++;
					}
				}
			}
		}
	}
	for (j = 0; j < i - 1; j++) {
		int recnum = 0;
		boardo[j].eatlue = pick(boardo[j].fsear, boardo[j].fx, boardo[j].fy, boardo[j].foption, color) + nip(boardo[j].fsear, boardo[j].fx, boardo[j].fy, boardo[j].foption, color);
		mn = countn((const char(*)[BOARD_SIZE])boardo[j].fsear, color);
		yn = countn((const char(*)[BOARD_SIZE])boardo[j].fsear, 3 - color);
		boardo[j].cohelue = judgecohe(cohere((const char(*)[BOARD_SIZE])boardo[j].fsear, color));
		boardo[j].attacklue = judgeattack(board, (const char(*)[BOARD_SIZE])boardo[j].fsear, color);
		boardo[j].maplue = judgemap((const char(*)[BOARD_SIZE])boardo[j].fsear, color);
		boardo[j].eatenlue = judgeattack(board, (const char(*)[BOARD_SIZE])boardo[j].fsear, 3 - color);
		boardo[j].value = judgetotal(boardo[j].eatlue, boardo[j].cohelue, boardo[j].attacklue, boardo[j].maplue,boardo[j].eatenlue, mn, yn);
		searchboard2((const char(*)[BOARD_SIZE])boardo[j].fsear, 3 - color, recnum);
		boardo[j].value = boardo[j].value -2*(layernum[0]->value) + 0.4*(layernum[1]->value);
	}
	maxvalue = boardo[0];
	for (j = 1; j < i - 1; j++) {
		if (boardo[j].value > maxvalue.value) maxvalue = boardo[j];
	}
	if (board[9][7] == color && board[9][8] == color && board[9][9] == color && board[5][3] == color && board[5][4] == color && board[5][5] == color && board[2][9] == color && board[3][9] == color)
	{
		maxvalue.fx = 9; maxvalue.fy = 7; maxvalue.foption = 4;
	}

	return maxvalue;
}


//主程序使用的函数二，传入的参数有：1.棋盘当前的状态 2.我方棋种类（黑棋还是白棋） 
//要求return一个Command类型的值 
struct Command aiTurn(const char board[BOARD_SIZE][BOARD_SIZE], int me) {
	struct Command preferedPos;
	Fsearunit thisone;
	thisone = searchboard(board, me);
	preferedPos.x = thisone.fx;
	preferedPos.y = thisone.fy;
	preferedPos.option = thisone.foption;
	return preferedPos;
}
/**
 * 你的代码结束
 */


BOOL place(int x, int y, OPTION option, int cur_flag) {
	// 移动之前的位置没有我方棋子
	if (board[x][y] != cur_flag) {
		return FALSE;
	}

	int new_x = x + DIR[option][0];
	int new_y = y + DIR[option][1];
	// 移动之后的位置超出边界, 或者不是空地
	if (!isInBound(new_x, new_y) || board[new_x][new_y] != EMPTY) {
		return FALSE;
	}

	board[x][y] = EMPTY;
	board[new_x][new_y] = cur_flag;

	int cur_other_flag = 3 - cur_flag;

	// 挑
	int intervention_dir[4][2] = { {1, 0}, {0, 1}, {1, 1}, {1, -1} };
	for (int i = 0; i < 4; i++) {
		int x1 = new_x + intervention_dir[i][0];
		int y1 = new_y + intervention_dir[i][1];
		int x2 = new_x - intervention_dir[i][0];
		int y2 = new_y - intervention_dir[i][1];
		if (isInBound(x1, y1) && isInBound(x2, y2) && board[x1][y1] == cur_other_flag && board[x2][y2] == cur_other_flag) {
			board[x1][y1] = cur_flag;
			board[x2][y2] = cur_flag;
		}
	}

	// 夹
	int custodian_dir[8][2] = { {1, 0}, {-1, 0}, {0, 1}, {0, -1}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1} };
	for (int i = 0; i < 8; i++) {
		int x1 = new_x + custodian_dir[i][0];
		int y1 = new_y + custodian_dir[i][1];
		int x2 = new_x + custodian_dir[i][0] * 2;
		int y2 = new_y + custodian_dir[i][1] * 2;
		if (isInBound(x1, y1) && isInBound(x2, y2) && board[x2][y2] == cur_flag && board[x1][y1] == cur_other_flag) {
			board[x1][y1] = cur_flag;
		}
	}

	return TRUE;
}


//........
//........
//..XXXX..
//XXXX....
//....OOOO
//..OOOO..
//........
//........
void start(int flag) {
	memset(board, 0, sizeof(board));

	for (int i = 0; i < 3; i++) {
		board[2][2 + i] = WHITE;
		board[6][6 + i] = WHITE;
		board[5][3 + i] = BLACK;
		board[9][7 + i] = BLACK;
	}

	for (int i = 0; i < 2; i++) {
		board[8 + i][2] = WHITE;
		board[2 + i][9] = BLACK;
	}

	initAI(flag);
}

void turn() {
	// AI
	struct Command command = aiTurn((const char(*)[BOARD_SIZE])board, me_flag);
	place(command.x, command.y, command.option, me_flag);
	printf("%d %d %d\n", command.x, command.y, command.option);
	fflush(stdout);
}

void end(int x) {

}

void loop() {
	//  freopen("../input", "r", stdin);
	while (TRUE)
	{
		memset(buffer, 0, sizeof(buffer));
		gets(buffer);

		if (strstr(buffer, START))
		{
			char tmp[MAX_BYTE] = { 0 };
			sscanf(buffer, "%s %d", tmp, &me_flag);
			other_flag = 3 - me_flag;
			start(me_flag);
			printf("OK\n");
			fflush(stdout);
		}
		else if (strstr(buffer, PLACE))
		{
			char tmp[MAX_BYTE] = { 0 };
			int x, y;
			OPTION option;
			sscanf(buffer, "%s %d %d %d", tmp, &x, &y, &option);
			place(x, y, option, other_flag);
		}
		else if (strstr(buffer, TURN))
		{
			turn();
		}
		else if (strstr(buffer, END))
		{
			char tmp[MAX_BYTE] = { 0 };
			int x;
			sscanf(buffer, "%s %d", tmp, &x);
			end(x);
		}
		//    printBoard();
	}
}


int main(int argc, char *argv[]) {
	loop();
	return 0;
}