#include <iostream>
#include <stdio.h>
#include <windows.h>
#include <string>
#include <vector>
#include <stdlib.h>
#include <conio.h>

using namespace std;

//Game Area
char tmp_map[21][60];
char map[21][60] = {
"+#########################################+",
"#                                         #",
"#  ###########                            #",
"#            #                            #",
"#            #######                      #",
"#                                         #",
"#                                         #",
"#                #############            #",
"#                                         #",
"#                                         #",
"#    ##########                           #",
"#             #                           #",
"#             #                           #",
"#             #        ############       #",
"#                                #        #",
"#                                ####     #",
"#                                         #",
"#                                         #",
"#                                         #",
"+#########################################+"
};

//Display Map
void ShowMap() {
	for (int i = 0; i < 21; i++) {
		printf("%s\n", map[i]);
	}
}

//Player Position
void gotoxy(short x, short y) {
	HANDLE hstdout = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD position = {x, y};
	SetConsoleCursorPosition(hstdout, position);
}

//Player movement mechanics
struct walk {
	long walk_count;
	long x;
	long y;
	long back;
};

//AI movement mechanics
struct target {
	long x;
	long y;
};

vector<target> walk_queue;
vector<walk> BFSArray;

//Collision mechanics
void AddArray( int x, int y, int wx, int back) {
	if(tmp_map[y][x] == ' ' || tmp_map[y][x] == '.') {
		tmp_map[y][x] = '#';
		walk tmp;
		tmp.x = x;
		tmp.y = y;
		tmp.back = back;
		BFSArray.push_back(tmp);
	}
}

//AI mechanics
void FindPath(int sx, int sy, int x, int y) {
	memcpy(tmp_map, map, sizeof(map));
	BFSArray.clear();
	walk tmp;
	tmp.x = sx;
	tmp.y = sy;
	tmp.walk_count = 0;
	tmp.back = -1;
	BFSArray.push_back(tmp);
	int i = 0;
	while (i < BFSArray.size()) {
		if(BFSArray[i].x == x && BFSArray[i].y == y) {
			walk_queue.clear();
			target tmp2;
			while (BFSArray[i].walk_count != 0) {
				tmp2.x = BFSArray[i].x;
				tmp2.y = BFSArray[1].y;
				walk_queue.push_back(tmp2);
				i = BFSArray[i].back;
				
			}
			break;
		}
		AddArray(BFSArray[i].x + 1, BFSArray[i].y, BFSArray[i].walk_count + 1, i);
		AddArray(BFSArray[i].x - 1, BFSArray[i].y, BFSArray[i].walk_count + 1, i);
		AddArray(BFSArray[i].x, BFSArray[i].y + 1, BFSArray[i].walk_count + 1, i);
		AddArray(BFSArray[i].x, BFSArray[i].y - 1, BFSArray[i].walk_count + 1, i);
		i++;
	}
	BFSArray.clear();
}

int main() {
	bool running = true;
	int x = 14;
	int y = 16;
	int old_x;
	int old_y;
	int a = 1000000000;
	int ex = 4;
	int ey = 3;
	int pts = 0;
	
	//Display Main Screen
	printf("Welcome to the PACMAN C++ Game\n\n");
	
	//Difficulty Level mechanics
	printf("Choose Difficulty Level: \n\n H -> Hard\n M -> Medium\n E -> Easy\n\n");
	char diffi;
	int speedmod = 3;
	cin >> diffi;
	if (diffi == 'M') {
		speedmod = 2;
	}
	if (diffi == 'H') {
		speedmod = 1;
	}
	if (diffi == 'n') {
		speedmod = 2;
	} else if (diffi == 'h') {
		speedmod = 1;
	}
	system("cls");
	ShowMap();
	gotoxy(x,y); cout << "m";
	int frame = 0;
	FindPath(ex, ey, x, y);
	
	//Steering player at arrow keys mechanics
	while (running) {
		gotoxy(x, y); 
		printf(" ");
		old_x = x;
		old_y = y;
		
		if (GetAsyncKeyState(VK_UP)) {
			if (map[y - 1][x] == '.') {
				y--;
				pts += 10;
			} else if (map[y - 1][x] == ' ') y--;
			if (map[y - 1][x] == '|') y--;
			if (map[y - 1][x] == '_') y--;
		}
		if (GetAsyncKeyState(VK_DOWN)) {
			if (map[y + 1][x] == '.') {
				y++;
				pts += 10;
			}   else if (map[y + 1][x] == ' ') y++;
			if (map[y + 1][x] == '|') y++;
			if (map[y + 1][x] == '_') y++;	
		}
		if (GetAsyncKeyState(VK_LEFT)) {
			if (map[y][x - 1] == '.') {
				x--;
				pts += 10;
			} else if (map[y][x - 1] == ' ') x--;
			if (map[y][x - 1] == '|') x--;
			if (map[y][x - 1] == '_') x--;	
		}
		if (GetAsyncKeyState(VK_RIGHT)) {
			if (map[y][x + 1] == '.') {
				x++;
				pts += 10;
			} else if (map[y][x + 1] == ' ') x++;
			if (map[y][x + 1] == '|') x++;
			if (map[y][x + 1] == '_') x++;
		}
		if (old_x != x || old_y != y) {
			FindPath(ex, ey, x, y);
		}
		
		//Display player and AI avatar 
		gotoxy(x, y);
		printf("c");
		map[ey][ex] = '.';
		gotoxy(ex, ey);
		printf(".");
		if (frame%speedmod == 0 && walk_queue.size() != 0) {
			ex = walk_queue.back().x;
			ey = walk_queue.back().y;
			walk_queue.pop_back();
		}
		gotoxy(ex, ey);
		printf("$");
		if (ex == x && ey == y) {
			break;
		}
		gotoxy(32, 18);
		gotoxy(45, 1); 
		cout << pts;
		Sleep(100);
		frame++;
	}
	
	//Win/Lose mechanics
	if (pts < 1000000) {
		system("cls");
		printf("You Lose.You have earned the following number of points: %i", pts);
		cin.get();
		cin.get();
		cin.get();
		cin.get();
		cin.get();
		return 0;
	} else if (pts > 1000000) {
		system("cls");
		printf("You Win :) You have earned the following number of points: %i", pts);
		cin.get();
		cin.get();
		cin.get();
		cin.get();
		cin.get();
		return 0;
	}
}

