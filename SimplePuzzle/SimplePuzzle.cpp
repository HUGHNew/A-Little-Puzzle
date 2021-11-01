// SimplePuzzle.cpp : This file contains the 'main' function. Program execution begins and ends there.
//


#include<iostream>
#include"base.hpp"
#include"FallingChar.h"
#include"TicTacToe.h"
void GUI() {
	initgraph(200, 100);
	int step = 0;
	int seq[3] = { 1,2,3 };
	//printf("%d", sizeof(short));
	int mask = 1 << (sizeof(short)*8 - 1);
	//printf("%x",mask);
	while (step!=3) {
		if(GetAsyncKeyState('a')&1){
			++step;
			puts("Steps");
		}
		else {
			outtextxy(50,50,char(step + 'A'));
			//puts
		}
		if (GetAsyncKeyState(VK_ESCAPE) & mask) {
			break;
		}
	}
	// 关闭图形模式
	closegraph();
}
int main() {
	return 0;
}
