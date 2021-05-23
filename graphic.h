#pragma once
#include <iostream>
#include "console.h"
using namespace std;

void DrawRectangle1(int ox, int oy, short width, short height);
void DrawRectangle2(int ox, int oy, short width, short height);
struct _Board {
	int endX, endY;
	int startX, startY;
	
	/* Khởi tạo board cho game*/
	void DrawBoard(short n, int ox, int oy, short mode, short level) {
		clrscr();  //Clear màn hình trước khi vẽ bảng
		SetWindowSize(80, 26);
		DecorGamePlay(mode, level);
		gotoXY(ox, oy); //Toạ độ điểm trên cùng bên trái của bảng, thay đổi ox,oy là thay đổi vị trí của bảng
		startX = whereX();
		startY = whereY();
		short width = 3; //Biến để tăng độ rộng cho mỗi cột
		TextColor(2);
		Sleep(50);
		//Cạnh trên cùng
		cout << (char)201; //Góc vuông trên trái
		for (int i = 1; i <= n; ++i) {
			for (int k = 1; k <= width; ++k)cout <<(char)205; //Gạch ngang
			if (i == n) break;
			cout << (char)203; //chữ T 
		}
		cout << (char)187 << endl;
		gotoXY(ox, ++oy);
		for (int i = 1; i <= n; ++i) {
			Sleep(50);
			//Tăng độ cao cho hàng bằng cách chèn thêm (char)179 trên mỗi dấu cộng
			for (int j = 1; j <= n + 1; ++j) {
				cout << (char)186;
				cout << "   ";
			}
			cout << endl;
			gotoXY(ox, ++oy);
			if (i == n) break; //Nếu đã tới hàng cuối thì không cần tạo thêm dấu cộng
			cout << (char)204;
			for (int j = 1; j <= n; ++j) {
				for (int k = 1; k <= width; ++k)cout << (char)205; //Gạch ngang
				if (j != n) {
					cout << (char)206; //Dấu cộng
				}
			}
			cout << (char)185 << endl;
			gotoXY(ox, ++oy);
		}
		Sleep(50);
		//Cạnh dưới cùng
		cout << (char)200; //Góc vuông dưới trái
		for (int i = 1; i <= n; ++i) {
			for (int k = 1; k <= width; ++k)cout << (char)205; //Gạch ngang
			if (i == n) break;
			cout << (char)202;  //Chứ T ngược
		}
		cout << (char)188;  //Góc vuông dưới phải
		TextColor(15);
		endX = whereX();
		endY = whereY();
	}



	void DecorGamePlay(short mode, short level) {
		for (int i = 0; i <= 26; ++i) {
			if (i == 20) {
				gotoXY(50, i);
				cout << (char)185;
				gotoXY(0, i);
				for (int i = 0; i < 50; ++i) {
					cout << (char)205;
				}
				continue;
			}
			if (i == 17 || i == 3 || i == 7) {
				gotoXY(50, i);
				cout << (char)204;
				gotoXY(51, i);
				for (int j = 0; j < 30; ++j) {
					cout << (char)205;
				}
				continue;
			}
			gotoXY(50, i);
			cout << (char)186;
		}

		gotoXY(58, 1);
		TextColor(11);
		cout << "GOMOKUHCMUS 1.0";
		gotoXY(52, 4);
		TextColor(14);
		cout << "Mode  : ";
		TextColor(15);
		if (mode == 1) cout << "Player vs Player";
		else cout << "Player vs Computer";
		gotoXY(52, 6);
		TextColor(14);
		cout << "Level : ";
		TextColor(15);
		if (mode == 1) cout << "No level";
		else if (level == 0) cout << "Easy";
		else if (level == 1) cout << "Medium";
		else cout << "Hard";
		DrawRectangle1(51, 8, 27, 5);
		TextColor(14);
		gotoXY(53, 9);
		cout << "Player 1";
		TextColor(15);
		gotoXY(56, 10);
		cout << (char)179;
		gotoXY(56, 11);
		cout << (char)195 << (char)196 <<" Name   :\t" ;
		gotoXY(56, 12);
		cout << (char)179;
		gotoXY(56, 13);
		cout << (char)192 << (char)196 <<" Symbol :\t" ;
		
		DrawRectangle1(51, 18, 27, 5);
		TextColor(14);
		gotoXY(53, 19);
		cout << "Player 2";
		TextColor(15);
		gotoXY(56, 20);
		cout << (char)179;
		gotoXY(56, 21);
		cout << (char)195 << (char)196 << " Name   :\t" ;
		gotoXY(56, 22);
		cout << (char)179;
		gotoXY(56, 23);
		cout << (char)192 << (char)196 << " Symbol :\t" ;

		gotoXY(2, 22);
		cout << "ESC  : BACK TO MENU           F1   : REPLAY";
		gotoXY(2, 24);
		cout << "F2   : ON/OFF SOUND           F4   : EXIT GAME" ;
	}
};

//Hàm vẽ khung hình chữ nhật để trang trí
void DrawRectangle1(int ox, int oy, short width, short height) {
	gotoXY(ox, oy);
	cout << (char)218; //Góc vuông trên trái
	for (int i = 1; i <= width; ++i) {
		cout << (char)196; //Gach ngang
	}
	cout << (char)191 << endl;
	gotoXY(ox, ++oy);
	for (int i = 1; i <= height; ++i) {
		cout << (char)179;
		for (int j = 1; j <= width; ++j) {
			cout << " ";
		}
		cout << (char)179 << endl;
		gotoXY(ox, ++oy);
	}

	cout << (char)192;
	for (int i = 1; i <= width; ++i) {
		cout << (char)196;
	}
	cout << (char)217;
}
void DrawRectangle2(int ox, int oy, short width, short height) {
	//TextSize(16, 30);
	gotoXY(ox, oy);
	cout << (char)201; //Góc vuông trên trái
	for (int i = 1; i <= width; ++i) {
		cout << (char)205; //Gach ngang
	}
	cout << (char)187 << endl;
	gotoXY(ox, ++oy);
	for (int i = 1; i <= height; ++i) {
		cout << (char)186;
		for (int j = 1; j <= width; ++j) {
			cout << " ";
		}
		cout << (char)186 << endl;
		gotoXY(ox, ++oy);
	}

	cout << (char)200;
	for (int i = 1; i <= width; ++i) {
		cout << (char)205;
	}
	cout << (char)188;
	//TextSize(10, 16);
}



