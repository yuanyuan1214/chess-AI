#include "ai.h"
#include<string.h>
                                                    
// bool
typedef int BOOL;
#define TRUE 1
#define FALSE 0


//判断棋是否在棋盘内 
BOOL isInBound(int x, int y) {
  return x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE;
}

const int MIR[8][2] = { {-1, 0}, {1, 0}, {0, -1}, {0, 1}, {-1, -1}, {1, 1}, {1, -1}, {-1, 1} };

typedef struct fsearunit {
	char fsear[BOARD_SIZE][BOARD_SIZE];
	int fx;
	int fy;
	int foption;
	float value;
} Fsearunit;

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

typedef struct chesspos {
	int x;
	int y;
	float value;
} Chesspos;

//记录当前棋盘的color子和对方的位置(返回color个数)
int recorpos(const char board[BOARD_SIZE][BOARD_SIZE], int color, Chesspos mpos[16], Chesspos ypos[16])
{
	int x, y;
	int i = 0;
	int j = 0;
	for (x = 0; x < BOARD_SIZE; x++) {
		for (y = 0; y < BOARD_SIZE; y++) {
			if (board[x][y] == color)
			{
				mpos[i].x = x;
				mpos[i].y = y;
				i++;
			}
			if (board[x][y] == 3 - color)
			{
				ypos[j].x = x;
				ypos[j].y = y;
				j++;
			}
		}
	}
	return i;
}

//计算两点之前距离
int distance(Chesspos a, Chesspos b)
{
	int di;
	int c, d;
	c = a.x - b.x;
	d = a.y - b.y;
	di = c * c + d * d;
	return di;
}

//初始化空位置
void initpos(Chesspos empos[128])
{
	int j;
	for (j = 0; j < 128; j++)
	{
		empos[j].x = 100;
		empos[j].y = 100;
		empos[j].value = 0;
	}
}

//搜索空位数组，若有相同坐标，返回下标值，若无，返回128
int searchempos(Chesspos empos[], int en, int x, int y)
{
	int j;
	for (j = 0; j < en; j++)
	{
		if (empos[j].x == x && empos[j].y == y)
			return j;
	}
	return 128;
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

//评估棋盘棋子抱团值(返回一个float) 
float judgecohe(int cohelity, int mn)
{
	float colue = 0;
	switch (mn) {
	case 0: {
		colue = 0;
		break;
	}
	case 1: {
		colue = 0;
		break;
	}
	case 2: {
		colue = cohelity;
		break;
	}
	case 3: {
		colue = cohelity / 3.0;
		break;
	}
	case 4: {
		colue = cohelity / 6.0;
		break;
	}
	case 5: {
		colue = cohelity / 8.0;
		break;
	}
	case 6: {
		colue = cohelity / 11.0;
		break;
	}
	case 7: {
		colue = cohelity / 14.0;
		break;
	}
	case 8: {
		colue = cohelity / 17.0;
		break;
	}
	case 9: {
		colue = cohelity / 20.0;
		break;
	}
	case 10: {
		colue = cohelity / 23.0;
		break;
	}
	case 11: {
		colue = cohelity / 26.0;
		break;
	}
	case 12: {
		colue = cohelity / 28.0;
		break;
	}
	case 13: {
		colue = cohelity / 31.0;
		break;
	}
	case 14: {
		colue = cohelity / 34.0;
		break;
	}
	case 15: {
		colue = cohelity / 37.0;
		break;
	}
	case 16: {
		colue = cohelity / 40.0;
		break;
	}
	}
	return colue;
}

//color的下一步吃子位价值(float)及聚合度与地图值 
float recorempos(const char board[BOARD_SIZE][BOARD_SIZE], int color, Chesspos mpos[],Chesspos empos[128], int mn,int *cohere,float *maplue)
{
	int p, option;
	int en=0;
	int j;
	float attacklue=0;
	for (p = 0; p < mn; p++)
	{
		int x = mpos[p].x;
		int y = mpos[p].y;
		for (option = 0; option < 8; option++)
		{
			int i = x + MIR[option][0];
			int j = y + MIR[option][1];
			if (isInBound(i, j) && board[i][j] == EMPTY)
			{
				int k = searchempos(empos, en, i, j);
				if (k != 128) continue;
				int option2;
				for (option2 = 0; option2 < 8; option2 += 2)
				{
					int moption = option2 + 1;
					int m1 = i + MIR[option2][0];
					int n1 = j + MIR[option2][1];
					int m2 = i + MIR[moption][0];
					int n2 = j + MIR[moption][1];
					if (board[m1][n1] == 3 - color && board[m2][n2] == 3 - color)
					{
							empos[en].x = i;
							empos[en].y = j;
							if(empos[en].value<1.9) empos[en].value = 1.9;
						    break;
					}
				}
				for (option2 = 0; option2 < 8; option2++)
				{
					int m1 = i + MIR[option2][0];
					int n1 = j + MIR[option2][1];
					int m2 = m1 + MIR[option2][0];
					int n2 = n1 + MIR[option2][1];
					if (board[m1][n1] == 3 - color && board[m2][n2] == color)
					{
							empos[en].x = i;
							empos[en].y = j;
							if(empos[en].value<1) empos[en].value = 1;
							break;
					}
				}
			    if(empos[en].value!=0) en++;
			}
			else if (board[i][j] == color)
			{
				(*cohere)++;
			}
		}
		(*maplue) += MAPLUE[x][y];
	}
	*maplue = (*maplue) / 144.0;
	for (j = 0; j < en; j++)
	{
		attacklue += empos[j].value;
	}
	attacklue = attacklue / 16.0;
	return attacklue;
}

//对手的下一步吃子位价值(float)及最大吃子数
float trecorempos(const char board[BOARD_SIZE][BOARD_SIZE], int color, Chesspos ypos[],Chesspos tempos[128], int yn,int *eatn)
{
	int p, option;
	int en = 0;
	int j;
	float attacklue=0;
	int eat[128];
	for (p = 0; p<128; p++)
	{
		eat[p] = 0;
	}
	for (p = 0; p < yn; p++)
	{
		int x = ypos[p].x;
		int y = ypos[p].y;
		for (option = 0; option < 8; option++)
		{
			int i = x + MIR[option][0];
			int j = y + MIR[option][1];
			if (isInBound(i, j) && board[i][j] == EMPTY)
			{
				int k = searchempos(tempos, en, i, j);
				if (k != 128) continue;
				int option2;
				for (option2 = 0; option2 < 8; option2 += 2)
				{
					int moption = option2 + 1;
					int m1 = i + MIR[option2][0];
					int n1 = j + MIR[option2][1];
					int m2 = i + MIR[moption][0];
					int n2 = j + MIR[moption][1];
					if (board[m1][n1] == 3 - color && board[m2][n2] == 3 - color)
					{
							tempos[en].x = i;
							tempos[en].y = j;
							if(tempos[en].value<1.9) tempos[en].value = 1.9;
							eat[en] += 2.02;
					}
				}
				for (option2 = 0; option2 < 8; option2++)
				{
					int m1 = i + MIR[option2][0];
					int n1 = j + MIR[option2][1];
					int m2 = m1 + MIR[option2][0];
					int n2 = n1 + MIR[option2][1];
					if (board[m1][n1] == 3 - color && board[m2][n2] == color)
					{
							tempos[en].x = i;
							tempos[en].y = j;
							if(tempos[en].value<1) tempos[en].value = 1;
							eat[en]+=1.01;
					}
				}
				if(tempos[en].value!=0) en++;
			}
		}
	}
	for (j = 0; j < en; j++)
	{
		attacklue += tempos[j].value;
		if (eat[j] > (*eatn)) (*eatn) = eat[j];
	}
	attacklue = attacklue / 16.0;
	return attacklue;
}

//color的下两步全棋盘吃子值(夹)
float nipnext2(const char board[BOARD_SIZE][BOARD_SIZE], int color, Chesspos mpos[], Chesspos ypos[], int mn)
{
	int p, q;
	float tnipnum1 = 0;
	float tnipnum2 = 0;
	for (p = 0; p < 16 - mn; p++)
	{
		float nipnum1 = 0;
		float nipnum2 = 0;
		int i, j;
		i = ypos[p].x;
		j = ypos[p].y;
		if (board[i - 1][j] == color && isInBound(i + 1, j) && board[i + 1][j] == EMPTY)
		{
			Chesspos pos;
			pos.x = i + 1;
			pos.y = j;
			for (q = 0; q < mn; q++) {
				if ((mpos[q].x == i - 1) && (mpos[q].y == j)) continue;
				int di;
				di = distance(pos, mpos[q]);
				if (di <= 2)
				{
					nipnum1++;
					tnipnum1 += nipnum1;
					break;
				}
				if (di > 2 && di <= 8)
				{
					nipnum2 += 0.1;
					tnipnum2 += nipnum2;
					break;
				}
			}
		}
		if (nipnum1 == 1) continue;
		if (board[i + 1][j] == color && isInBound(i - 1, j) && board[i - 1][j] == EMPTY)
		{
			Chesspos pos;
			pos.x = i - 1;
			pos.y = j;
			for (q = 0; q < mn; q++) {
				if ((mpos[q].x == i + 1) && (mpos[q].y == j)) continue;
				int di;
				di = distance(pos, mpos[q]);
				if (di <= 2)
				{
					nipnum1++;
					tnipnum1 += nipnum1;
					break;
				}
				if (nipnum2 == 0 && di > 2 && di <= 8)
				{
					nipnum2 += 0.1;
					tnipnum2 += nipnum2;
					break;
				}
			}
		}
		if (nipnum1 == 1) continue;
		if (board[i][j - 1] == color && isInBound(i, j + 1) && board[i][j + 1] == EMPTY)
		{
			Chesspos pos;
			pos.x = i;
			pos.y = j + 1;
			for (q = 0; q < mn; q++) {
				if ((mpos[q].x == i) && (mpos[q].y == j - 1)) continue;
				int di;
				di = distance(pos, mpos[q]);
				if (di <= 2)
				{
					nipnum1++;
					tnipnum1 += nipnum1;
					break;
				}
				if (nipnum2 == 0 && di > 2 && di <= 8)
				{
					nipnum2 += 0.1;
					tnipnum2 += nipnum2;
					break;
				}
			}
		}
		if (nipnum1 == 1) continue;
		if (board[i][j + 1] == color && isInBound(i, j - 1) && board[i][j - 1] == EMPTY)
		{
			Chesspos pos;
			pos.x = i;
			pos.y = j - 1;
			for (q = 0; q < mn; q++) {
				if ((mpos[q].x == i - 1) && (mpos[q].y == j + 1)) continue;
				int di;
				di = distance(pos, mpos[q]);
				if (di <= 2)
				{
					nipnum1++;
					tnipnum1 += nipnum1;
					break;
				}
				if (nipnum2 == 0 && di > 2 && di <= 8)
				{
					nipnum2 += 0.1;
					tnipnum2 += nipnum2;
					break;
				}
			}
		}
		if (nipnum1 == 1) continue;
		if (board[i - 1][j + 1] == color && isInBound(i + 1, j - 1) && board[i + 1][j - 1] == EMPTY)
		{
			Chesspos pos;
			pos.x = i + 1;
			pos.y = j - 1;
			for (q = 0; q < mn; q++) {
				if ((mpos[q].x == i - 1) && (mpos[q].y == j + 1)) continue;
				int di;
				di = distance(pos, mpos[q]);
				if (di <= 2)
				{
					nipnum1++;
					tnipnum1 += nipnum1;
					break;
				}
				if (nipnum2 == 0 && di > 2 && di <= 8)
				{
					nipnum2 += 0.1;
					tnipnum2 += nipnum2;
					break;
				}
			}
		}
		if (nipnum1 == 1) continue;
		if (board[i + 1][j - 1] == color && isInBound(i - 1, j + 1) && board[i - 1][j + 1] == EMPTY)
		{
			Chesspos pos;
			pos.x = i - 1;
			pos.y = j + 1;
			for (q = 0; q < mn; q++) {
				if ((mpos[q].x == i + 1) && (mpos[q].y == j - 1)) continue;
				int di;
				di = distance(pos, mpos[q]);
				if (di <= 2)
				{
					nipnum1++;
					tnipnum1 += nipnum1;
					break;
				}
				if (nipnum2 == 0 && di > 2 && di <= 8)
				{
					nipnum2 += 0.1;
					tnipnum2 += nipnum2;
					break;
				}
			}
		}
		if (nipnum1 == 1) continue;
		if (board[i - 1][j - 1] == color && isInBound(i + 1, j + 1) && board[i + 1][j + 1] == EMPTY)
		{
			Chesspos pos;
			pos.x = i + 1;
			pos.y = j + 1;
			for (q = 0; q < mn; q++) {
				if ((mpos[q].x == i - 1) && (mpos[q].y == j - 1)) continue;
				int di;
				di = distance(pos, mpos[q]);
				if (di <= 2)
				{
					nipnum1++;
					tnipnum1 += nipnum1;
					break;
				}
				if (nipnum2 == 0 && di > 2 && di <= 8)
				{
					nipnum2 += 0.1;
					tnipnum2 += nipnum2;
					break;
				}
			}
		}
		if (nipnum1 == 1) continue;
		if (board[i + 1][j + 1] == color && isInBound(i - 1, j - 1) && board[i - 1][j - 1] == EMPTY)
		{
			Chesspos pos;
			pos.x = i - 1;
			pos.y = j - 1;
			for (q = 0; q < mn; q++) {
				if ((mpos[q].x == i + 1) && (mpos[q].y == j + 1)) continue;
				int di;
				di = distance(pos, mpos[q]);
				if (di <= 2)
				{
					nipnum1++;
					tnipnum1 += nipnum1;
					break;
				}
				if (nipnum2 == 0 && di > 2 && di <= 8)
				{
					nipnum2 += 0.1;
					tnipnum2 += nipnum2;
					break;
				}
			}
		}
	}
	for (p = 0; p < 16 - mn; p++)
	{
		float nipnum2 = 0;
		int i, j;
		i = ypos[p].x;
		j = ypos[p].y;
		if (isInBound(i - 1, j) && isInBound(i + 1, j) && board[i - 1][j] == EMPTY && board[i + 1][j] == EMPTY)
		{
			int flag1 = 0;
			int flag2 = 0;
			Chesspos pos1;
			pos1.x = i - 1;
			pos1.y = j;
			Chesspos pos2;
			pos2.x = i + 1;
			pos2.y = j;
			Chesspos pflag1;
			Chesspos pflag2;
			for (q = 0; q < mn; q++)
			{
				int di;
				di = distance(pos1, mpos[q]);
				if (di <= 2)
				{
					flag1++;
					pflag1.x = mpos[q].x;
					pflag1.y = mpos[q].y;
					break;
				}
			}
			for (q = 0; q < mn; q++)
			{
				int di;
				di = distance(pos2, mpos[q]);
				if (di <= 2)
				{
					flag2++;
					pflag2.x = mpos[q].x;
					pflag2.y = mpos[q].y;
					break;
				}
			}
			if ((flag1 != 0) && (flag2 != 0) && ((pflag1.x != pflag2.x) || (pflag1.y != pflag2.y)))
			{
				nipnum2 += 0.1;
				tnipnum2 += nipnum2;
			}
		}
		if (nipnum2 != 0) continue;
		if (isInBound(i, j - 1) && isInBound(i, j + 1) && board[i][j - 1] == EMPTY && board[i][j + 1] == EMPTY)
		{
			int flag1 = 0;
			int flag2 = 0;
			Chesspos pos1;
			pos1.x = i;
			pos1.y = j - 1;
			Chesspos pos2;
			pos2.x = i;
			pos2.y = j + 1;
			Chesspos pflag1;
			Chesspos pflag2;
			for (q = 0; q < mn; q++)
			{
				int di;
				di = distance(pos1, mpos[q]);
				if (di <= 2)
				{
					flag1++;
					pflag1.x = mpos[q].x;
					pflag1.y = mpos[q].y;
					break;
				}
			}
			for (q = 0; q < mn; q++)
			{
				int di;
				di = distance(pos2, mpos[q]);
				if (di <= 2)
				{
					flag2++;
					pflag2.x = mpos[q].x;
					pflag2.y = mpos[q].y;
					break;
				}
			}
			if ((flag1 != 0) && (flag2 != 0) && ((pflag1.x != pflag2.x) || (pflag1.y != pflag2.y)))
			{
				nipnum2 += 0.1;
				tnipnum2 += nipnum2;
			}
		}
		if (nipnum2 != 0) continue;
		if (isInBound(i - 1, j - 1) && isInBound(i + 1, j + 1) && board[i - 1][j - 1] == EMPTY && board[i + 1][j + 1] == EMPTY)
		{
			int flag1 = 0;
			int flag2 = 0;
			Chesspos pos1;
			pos1.x = i - 1;
			pos1.y = j - 1;
			Chesspos pos2;
			pos2.x = i + 1;
			pos2.y = j + 1;
			Chesspos pflag1;
			Chesspos pflag2;
			for (q = 0; q < mn; q++)
			{
				int di;
				di = distance(pos1, mpos[q]);
				if (di <= 2)
				{
					flag1++;
					pflag1.x = mpos[q].x;
					pflag1.y = mpos[q].y;
					break;
				}
			}
			for (q = 0; q < mn; q++)
			{
				int di;
				di = distance(pos2, mpos[q]);
				if (di <= 2)
				{
					flag2++;
					pflag2.x = mpos[q].x;
					pflag2.y = mpos[q].y;
					break;
				}
			}
			if ((flag1 != 0) && (flag2 != 0) && ((pflag1.x != pflag2.x) || (pflag1.y != pflag2.y)))
			{
				nipnum2 += 0.1;
				tnipnum2 += nipnum2;
			}
		}
		if (nipnum2 != 0) continue;
		if (isInBound(i - 1, j + 1) && isInBound(i + 1, j - 1) && board[i - 1][j + 1] == EMPTY && board[i + 1][j - 1] == EMPTY)
		{
			int flag1 = 0;
			int flag2 = 0;
			Chesspos pos1;
			pos1.x = i - 1;
			pos1.y = j + 1;
			Chesspos pos2;
			pos2.x = i + 1;
			pos2.y = j - 1;
			Chesspos pflag1;
			Chesspos pflag2;
			for (q = 0; q < mn; q++)
			{
				int di;
				di = distance(pos1, mpos[q]);
				if (di <= 2)
				{
					flag1++;
					pflag1.x = mpos[q].x;
					pflag1.y = mpos[q].y;
					break;
				}
			}
			for (q = 0; q < mn; q++)
			{
				int di;
				di = distance(pos2, mpos[q]);
				if (di <= 2)
				{
					flag2++;
					pflag2.x = mpos[q].x;
					pflag2.y = mpos[q].y;
					break;
				}
			}
			if ((flag1 != 0) && (flag2 != 0) && ((pflag1.x != pflag2.x) || (pflag1.y != pflag2.y)))
			{
				nipnum2 += 0.1;
				tnipnum2 += nipnum2;
			}
		}
	}
	return (tnipnum2/16.0);
}

//color的下两步全棋盘伪吃子值(挑)
float picknext2(const char board[BOARD_SIZE][BOARD_SIZE], int color, Chesspos mpos[], Chesspos ypos[], int mn)
{
	int p, q;
	char copy[BOARD_SIZE][BOARD_SIZE];
	copyboard(copy, board);
	float picknum2 = 0;
	for (p = 0; p < 16 - mn; p++)
	{
		int i, j;
		i = ypos[p].x;
		j = ypos[p].y;
		if (isInBound(i, j + 2) && copy[i][j + 2] == 3 - color && copy[i][j + 1] == EMPTY)
		{
			Chesspos pos;
			pos.x = i;
			pos.y = j + 1;
			for (q = 0; q < mn; q++) {
				int di;
				di = distance(pos, mpos[q]);
				if (di <= 2)
				{
					break;
				}
				if (di > 2 && di <= 8)
				{
					picknum2 += 0.19;
					break;
				}
			}
		}
		if (isInBound(i + 2, j + 2) && copy[i + 2][j + 2] == 3 - color && copy[i + 1][j + 1] == EMPTY)
		{
			Chesspos pos;
			pos.x = i + 1;
			pos.y = j + 1;
			for (q = 0; q < mn; q++) {
				int di;
				di = distance(pos, mpos[q]);
				if (di <= 2)
				{
					break;
				}
				if (di > 2 && di <= 8)
				{
					picknum2 += 0.19;
					break;
				}
			}
		}
		if (isInBound(i + 2, j) && copy[i + 2][j] == 3 - color && copy[i + 1][j] == EMPTY)
		{
			Chesspos pos;
			pos.x = i + 1;
			pos.y = j;
			for (q = 0; q < mn; q++) {
				int di;
				di = distance(pos, mpos[q]);
				if (di <= 2)
				{
					break;
				}
				if (di > 2 && di <= 8)
				{
					picknum2 += 0.19;
					break;
				}
			}
		}
		if (isInBound(i + 2, j - 2) && copy[i + 2][j - 2] == 3 - color && copy[i + 1][j - 1] == EMPTY)
		{
			Chesspos pos;
			pos.x = i + 1;
			pos.y = j - 1;
			for (q = 0; q < mn; q++) {
				int di;
				di = distance(pos, mpos[q]);
				if (di <= 2)
				{
					break;
				}
				if (di > 2 && di <= 8)
				{
					picknum2 += 0.19;
					break;
				}
			}
		}
		if (isInBound(i, j - 2) && copy[i][j - 2] == 3 - color && copy[i][j - 1] == EMPTY)
		{
			Chesspos pos;
			pos.x = i;
			pos.y = j - 1;
			for (q = 0; q < mn; q++) {
				int di;
				di = distance(pos, mpos[q]);
				if (di <= 2)
				{
					break;
				}
				if (di > 2 && di <= 8)
				{
					picknum2 += 0.19;
					break;
				}
			}
		}
		if (isInBound(i - 2, j - 2) && copy[i - 2][j - 2] == 3 - color && copy[i - 1][j - 1] == EMPTY)
		{
			Chesspos pos;
			pos.x = i - 1;
			pos.y = j - 1;
			for (q = 0; q < mn; q++) {
				int di;
				di = distance(pos, mpos[q]);
				if (di <= 2)
				{
					break;
				}
				if (di > 2 && di <= 8)
				{
					picknum2 += 0.19;
					break;
				}
			}
		}
		if (isInBound(i - 2, j) && copy[i - 2][j] == 3 - color && copy[i - 1][j] == EMPTY)
		{
			Chesspos pos;
			pos.x = i - 1;
			pos.y = j;
			for (q = 0; q < mn; q++) {
				int di;
				di = distance(pos, mpos[q]);
				if (di <= 2)
				{
					break;
				}
				if (di > 2 && di <= 8)
				{
					picknum2 += 0.19;
					break;
				}
			}
		}
		if (isInBound(i - 2, j + 2) && copy[i - 2][j + 2] == 3 - color && copy[i - 1][j + 1] == EMPTY)
		{
			Chesspos pos;
			pos.x = i - 1;
			pos.y = j + 1;
			for (q = 0; q < mn; q++) {
				int di;
				di = distance(pos, mpos[q]);
				if (di <= 2)
				{
					break;
				}
				if (di > 2 && di <= 8)
				{
					picknum2 += 0.19;
					break;
				}
			}
		}
		copy[i][j] = EMPTY;
	}
	return (picknum2/16.0);
}

//评估棋盘整体价值 
float judgetotal(float numlue, float cohelue, float attacklue, float maplue, float eatenlue, int mn, int color)
{
	float value = 0;
	switch (color) {
	case 1: {
		if (mn >= 7 && mn <= 9) value = numlue + attacklue + 0* cohelue + 0* maplue - 1* eatenlue;
		if (mn > 9) value = numlue + attacklue + 0* cohelue + 0* maplue - 1 * eatenlue;
		if (mn < 7) value = numlue + attacklue + 0 * cohelue + 0 * maplue - 1 * eatenlue;
		break;
	}
	case 2: {
		if (mn >= 8 && mn <= 10) value = numlue + attacklue + 0* cohelue + 0* maplue - 0.5 * eatenlue;
		if (mn > 10) value = numlue + attacklue + 0.01* cohelue + 0* maplue - 0.5* eatenlue;
		if (mn < 8) value = numlue + attacklue + 0 * cohelue + 0 * maplue - 0.5 * eatenlue;
		break;
	}
	}
	return value;
}

//对可行位置进行由小到大排序
void sort1(Fsearunit avapos[], int n)
{
	int i, j;
	Fsearunit temp;
	int m = 0, maxchan = 0;
	int sortborder = n - 1;
	for (i = 0; i < n - 1; i++)
	{
		BOOL isort = TRUE;
		for (j = 0; j < sortborder; j++)
		{
			if (avapos[j].value > avapos[j + 1].value)
			{
				temp = avapos[j];
				avapos[j] = avapos[j + 1];
				avapos[j + 1] = temp;
				isort = FALSE;
				maxchan = j;
			}
			m++;
		}
		sortborder = maxchan;
		if (isort) break;
	}
}

//对可行位置进行由大到小排序
void sort2(Fsearunit avapos[], int n)
{
	int i, j;
	Fsearunit temp;
	int m = 0, maxchan = 0;
	int sortborder = n - 1;
	for (i = 0; i < n - 1; i++)
	{
		BOOL isort = TRUE;
		for (j = 0; j < sortborder; j++)
		{
			if (avapos[j].value < avapos[j + 1].value)
			{
				temp = avapos[j];
				avapos[j] = avapos[j + 1];
				avapos[j + 1] = temp;
				isort = FALSE;
				maxchan = j;
			}
			m++;
		}
		sortborder = maxchan;
		if (isort) break;
	}
}

//剪枝搜索算法
float absearch(const char board[BOARD_SIZE][BOARD_SIZE], int color, float a, float b, int h, int player)
{
	if (h >= 3)
	{
		int mn, yn;
		int eatn = 0;
		int coherence=0;
		float maplue = 0;
		float numlue, attacklue1,cohelue,attacklue2,attacklue;
		float eatenlue, eatenlue1, eatenlue2;
		float value;
		Chesspos mpos[16];
		Chesspos ypos[16];
		Chesspos empos[128];
		Chesspos tempos[128];
		mn = recorpos(board, color, mpos, ypos);
		yn = 16 - mn;
		initpos(empos);
		initpos(tempos);
		attacklue1 = recorempos(board, color, mpos,empos, mn, &coherence, &maplue);
		attacklue2 =nipnext2(board, color, mpos, ypos, mn)+picknext2(board, color, mpos, ypos, mn);
		attacklue = attacklue1 + attacklue2;
		eatenlue1 = trecorempos(board, 3 - color, ypos, tempos, yn, &eatn);
		eatenlue2= nipnext2(board,3- color, ypos, mpos, yn) + picknext2(board, 3-color, ypos, mpos, yn);
		eatenlue = eatenlue1 + eatenlue2;
		cohelue = judgecohe(coherence, mn);
		numlue = mn - eatn;
		value = judgetotal(numlue, cohelue, attacklue, maplue, eatenlue, mn, color);
		return value;
	}
	Fsearunit avapos[128];
	int x, y, option;
	int i = 0;
	int j;
	if (player == 1)
	{
		for (x = 0; x < BOARD_SIZE; x++) {
			for (y = 0; y < BOARD_SIZE; y++) {
				if (board[x][y] == color) {
					for (option = 0; option < 8; option++) {
						if (canmove(board, color, x, y, option)) {
							copyboard(avapos[i].fsear, board);
							changeboard(avapos[i].fsear, color, x, y, option);
							avapos[i].fx = x;
							avapos[i].fy = y;
							avapos[i].foption = option;
							avapos[i].value = pick(avapos[i].fsear, avapos[i].fx, avapos[i].fy, avapos[i].foption, color) + nip(avapos[i].fsear, avapos[i].fx, avapos[i].fy, avapos[i].foption, color);
							i++;
						}
					}
				}
			}
		}
		//sort2(avapos, i);
		for (j = 0; j < i; j++)
		{
			float value;
			value = absearch((const char(*)[BOARD_SIZE])avapos[j].fsear, color, a, b, h + 1, 1 - player);
			if (value > a) a = value;
			if (a >= b) return a;
		}
		return a;
	}
	if (player == 0)
	{
		for (x = 0; x < BOARD_SIZE; x++) {
			for (y = 0; y < BOARD_SIZE; y++) {
				if (board[x][y] == 3 - color) {
					for (option = 0; option < 8; option++) {
						if (canmove(board, 3 - color, x, y, option)) {
							copyboard(avapos[i].fsear, board);
							changeboard(avapos[i].fsear, 3 - color, x, y, option);
							avapos[i].fx = x;
							avapos[i].fy = y;
							avapos[i].foption = option;
							avapos[i].value = 0 - pick(avapos[i].fsear, avapos[i].fx, avapos[i].fy, avapos[i].foption, 3 - color) - nip(avapos[i].fsear, avapos[i].fx, avapos[i].fy, avapos[i].foption, 3 - color);
							i++;
						}
					}
				}
			}
		}
		//sort1(avapos, i);
		for (j = 0; j < i; j++)
		{
			float value;
			value = absearch((const char(*)[BOARD_SIZE])avapos[j].fsear, color, a, b, h + 1, 1 - player);
			if (value < b) b = value;
			if (a >= b) return b;
		}
		return b;
	}
}

//第一步搜索
Fsearunit searchboard(const char board[BOARD_SIZE][BOARD_SIZE], int color)
{
	Fsearunit boardo[128];
	Fsearunit avamax[128]; 
	int x, y, option;
	int i = 0;
	int j;
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
						boardo[i].value = pick(boardo[i].fsear, boardo[i].fx, boardo[i].fy, boardo[i].foption, color) + nip(boardo[i].fsear, boardo[i].fx, boardo[i].fy, boardo[i].foption, color);
						i++;
					}
				}
			}
		}
	}
	//sort2(boardo, i);
	maxvalue.value = -100000;
	for (j = 0; j < i; j++) {
		float a = -1000;
		float b = 1000;
		int h = 0;
		int player = 1;
		boardo[j].value = absearch((const char(*)[BOARD_SIZE])boardo[j].fsear, color, a, b, h + 1, 1 - player);
		if (boardo[j].value > maxvalue.value) maxvalue = boardo[j];
	}
	return maxvalue;
}

//主程序使用的函数二，传入的参数有：1.棋盘当前的状态 2.我方棋种类（黑棋还是白棋） 
//要求return一个Command类型的值 
struct Command aiTurn(const char board[BOARD_SIZE][BOARD_SIZE], int me) {
	struct Command preferedPos;
	Fsearunit thisone;
	if (me==1&&board[9][7] == me && board[9][8] == me && board[9][9] == me && board[5][3] == me && board[5][4] == me && board[5][5] == me && board[2][9] == me && board[3][9] == me)
	{
		thisone.fx = 9;thisone.fy = 7; thisone.foption = 4;
	}
	else if(me==2&&board[8][7]==1&&board[2][2] == me && board[2][3] == me && board[2][4] == me && board[8][2] == me && board[9][2] == me && board[6][6] == me && board[6][7] == me && board[6][8] == me)
	{
		thisone.fx = 6;thisone.fy = 8; thisone.foption = 1;
	}
	else{
	thisone = searchboard(board, me);}
	preferedPos.x = thisone.fx;
	preferedPos.y = thisone.fy;
	preferedPos.option = thisone.foption;
	return preferedPos;
}

