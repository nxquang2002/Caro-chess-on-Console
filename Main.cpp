#pragma comment (lib, "Winmm.lib")
#include <iostream>
#include <iomanip>
#include <conio.h> 
#include <Windows.h>
#include <fstream>
#include <mmsystem.h>
#include <string>
#include <vector>
#include "graphic.h"
#include "console.h"

//Tạo một số biến toàn cục để sử dụng xuyên suốt chương trình
using namespace std;
short n = 3;
short mode = 1; //Biến kiểm soát chế độ chơi, nếu mode = 1 => P vs P; mode = 2 => p vs C
short level = 0; // Biến kiểm soát level của Bot: 0 -> easy, 1 -> medium, 2 -> hard
short tableX, tableY; //Biến toàn cục để theo dõi vị trí của con trỏ trong Board
int diem[100][100];
//typedef enum { X, O, e } xo; //Kiểu dữ liệu xo để lưu trạng thái của ô cờ
typedef enum { nguoi, may } PorC;
char snguoi, smay, splayer1 = 'X', splayer2 = 'O'; // Biến char để xác định ký hiệu của người và máy, người chơi 1 và người chơi 2
string sound = "ON ";  //Biến bool để lưu trạng thái tắt mở âm thanh


/* Hàm kiểm tra xem vị trí ô còn trống hay không */
bool CheckWhetherAvailable(int x, int y, char a[100][100]);

/* Hàm kiểm tra người thắng theo hàng */
void CheckHorizontal(char a[100][100], bool &isEnd,int win, char s1, char s2, short& winner);
//---------------------------------------------------------
/* Hàm kiểm tra người thắng theo cột */
void CheckVertical(char a[100][100], bool& isEnd, int win, char s1, char s2,short& winner);
//---------------------------------------------------------
/* Hàm kiểm tra đường chéo từ Top Left đến Bottom Right*/
void CheckTheFirstDiagonal(char a[100][100], bool& isEnd, int win, char s1, char s2, short& winner);
//---------------------------------------------------------
/* Hàm kiếm tra đường chéo từ Top Right đến Bottom Left*/
void CheckTheSecondDiagonal(char a[100][100], bool& isEnd, int win, char s1, char s2, short& winner);
/*Hàm kiểm tra hòa cờ*/
void isDraw(char a[100][100], bool& isEnd, short& winner);

int checkHang(char a[100][100], int hang, char luot) {
	int count = 0;
	for (int i = 1; i <= n; ++i) {
		if (a[i][hang] == luot) {
			count++;
		}
	}
	return count;
}
int checkCot(char a[100][100], int cot, char luot) {
	int count = 0;
	for (int i = 1; i <= n; ++i) {
		if (a[cot][i] == luot) count++;
	}
	return count;
}
int checkDuongCheo1(char a[100][100], char luot) {
	int count = 0;
	for (int i = 1; i <= n; ++i) {
		if (a[i][i] == luot) {
			count++;
		}
	}
	return count;
}
int checkDuongCheo2(char a[100][100], char luot) {
	int count = 0;
	for (int i = 1; i <= n; ++i) {
		if (a[n - i + 1][i] == luot) count++;
	}
	return count;
}


void Statistics(short& time);
void ReadStatistics();
void UpdateStatistics();
void ResetStatistics(short& time);
//Kiểu dữ liệu Menu
struct _Menu {
	char* player1 = new char[14];
	char* player2 = new char[14];
	char* player;
	char tempS1 = 'X', tempS2 = 'O';

	void Menu(int choose) {
		Nocursortype();
		clrscr();
		TextColor(9);
		ShowScrollbar(false);
		
		DrawRectangle2(16, 3, 80, 7);
		gotoXY(20, 5);
		cout << " XXXXXXXXX    OOOOOOO   XXXXX     XXXXX   OOOOOOO   XXX   XXX OOO     OOO";
		gotoXY(20, 6);
		cout << "XXX          OOO   OOO  XXX XXX XXX XXX  OOO   OOO  XXX XXX   OOO     OOO";
		gotoXY(20, 7);
		cout << "XXX   XXXXX OOO     OOO XXX  XXXXX  XXX OOO     OOO XXXXXX    OOO     OOO";
		gotoXY(20, 8);
		cout << "XXX    XXX   OOO   OOO  XXX         XXX  OOO   OOO  XXX XXX   OOO     OOO";
		gotoXY(20, 9);
		cout << " XXXXXXXXX    OOOOOOO   XXX         XXX   OOOOOOO   XXX   XXX  OOOOOOOOO ";
		TextColor(15);


		bool check = true;
		short time = 0; //Biến chặn người chơi reset statistics nhiều lần
		while (check == true) {
			gotoXY(55, 14);
			cout << "New Game";
			gotoXY(55, 16);
			cout << "Settings";
			gotoXY(55, 18);
			cout << "Statistics";
			gotoXY(55, 20);
			cout << "About";
			gotoXY(55, 22);
			cout << "Help";
			gotoXY(55, 24);
			cout << "Exit";

			TextColor(3);
			switch (choose) {
			case 0:
				gotoXY(55, 14);
				cout << "New Game";
				break;
			case 1:
				gotoXY(55, 16);
				cout << "Settings";
				break;
			case 2:
				gotoXY(55, 18);
				cout << "Statistics";
				break;
			case 3:
				gotoXY(55, 20);
				cout << "About";
				break;
			case 4:
				gotoXY(55, 22);
				cout << "Help";
				break;
			case 5:
				gotoXY(55, 24);
				cout << "Exit";
				break;
			}

			TextColor(15);
			//Hàm lấy lựa chọn từ người chơi
			char key = -1;
			if (_kbhit()) key = toupper(_getch());
			switch (key)
			{
			case 27:
				choose = 5;
				break;
			case 'A': case 75:
				break;
			case 'W': case 72:
				choose--;
				break;
			case 'S': case 80:
				choose++;
				break;
			case 'D': case 77:
				break;
			case 13: case 32:
				check = false;
				switch (choose) {
				case 0:
					clrscr();
					UnNocursortype();
					SetUpBeforeNewGame();
					break;
				case 1:
					clrscr();
					Setting(0);
					break;
				case 2:
					//clrscr();
					Statistics(time);
					Menu(2);
					break;
				case 3:
					clrscr();
					DrawRectangle2(23, 5, 68, 17);
					gotoXY(52, 7);
					cout << "<< ABOUT >>";
					gotoXY(40, 10);
					cout << "DESIGN BY\t:\tNGUYEN XUAN QUANG";
					gotoXY(40, 12);
					cout << "CLASS\t\t:\t20CTT2 HCMUS";
					gotoXY(40, 14);
					cout << "SID\t\t:\t20125049";
					gotoXY(40, 16);
					cout << "VERSION\t\t:\tGOMOKUHCMUS 1.0";
					gotoXY(40, 20);
					system("pause");
					Menu(3);
					break;
				case 4:
					clrscr();
					DrawRectangle2(23, 5, 68, 17);
					gotoXY(52, 7);
					cout << "<< Help >>";
					gotoXY(40, 10);
					cout << "W/Up Arrow\t:\tMove Upward";
					gotoXY(40, 12);
					cout << "S/Down Arrow\t:\tMove Downward";
					gotoXY(40, 14);
					cout << "A/Left Arrow\t:\tMove Left";
					gotoXY(40, 16);
					cout << "D/Right Arrow\t:\tMove Right";
					gotoXY(40, 18);
					cout << "ENTER\t\t:\tChoose";
					gotoXY(40, 21);
					system("pause");
					Menu(4);
					break;
				case 5:
					//Menu(5);
					clrscr();
					if (sound == "ON ") {
						PlaySound(TEXT("Result-screen.wav"), NULL, SND_ASYNC);
					}
					Sleep(100);
					DrawRectangle2(48, 11, 18, 1);
					gotoXY(50, 12);
					cout << "THANKS FOR USED!";
					gotoXY(45, 20);
					Sleep(100);
					system("pause");
					exit(0);
					break;
				}
			}
			if (choose < 0) choose = 5;
			if (choose > 5) choose = 0;
			Sleep(50);
		}
	}

	void SetUpBeforeNewGame() {
		Nocursortype();
		clrscr();
		DrawRectangle2(24, 5, 70, 15);
		gotoXY(54, 7);
		cout << "<< NEW GAME >>";
		short chooseMode = 0;
		gotoXY(30, 10);
		cout << "Mode: ";
		TextColor(3);
		cout << "   Player vs Player >>";
		bool check = true;
		while (check) {
			char key = -1;
			if (_kbhit()) key = toupper(_getch());
			switch (key) {
			case 'A': case 75:  //Mũi tên sang traí hoặc phím 'A' thì thay đổi chế độ
				TextColor(3);
				gotoXY(36, 10);
				cout << "   Player vs Player >>";
				chooseMode = 0;
				break;
			case 'D': case 77:  //Mũi tên xang phải hoặc phím 'D'
				TextColor(3);
				gotoXY(36, 10);
				cout << "<< Player vs Computer ";
				chooseMode = 1;
				break;
			case 13:
				check = false;
				switch (chooseMode) {
				case 0:
					mode = 1;
					gotoXY(36, 10);
					cout << "Player vs Player      ";
					break;
				case 1:
					mode = 2;
					gotoXY(36, 10);
					cout << "Player vs Computer      ";
					break;
				}
			}
		}

		TextColor(15);
		UnNocursortype();
		if (mode == 1) {
			gotoXY(30, 12);
			cout << "Player 1: ";
			TextColor(3);
			fflush(stdin);
			int x = whereX();
			int y = whereY();
			cin.getline(player1, 14);
			while (strlen(player1) == 0) {
				gotoXY(x, y);
				cin.getline(player1, 14);
			}
			TextColor(15);
			gotoXY(30, 14);
			cout << "Player 2: ";
			TextColor(3);
			fflush(stdin);
			x = whereX();
			y = whereY();
			cin.getline(player2, 14);
			while (strlen(player2) == 0) {
				gotoXY(x, y);
				cin.getline(player2, 14, '\n');
			}
			TextColor(15);
			gotoXY(30, 16);
			Nocursortype();
			cout << "Board: ";
			TextColor(3);
			cout << "<< " << n << " x " << n << " >>";
			bool checkSize = true;
			short chooseSize = 0;
			while (checkSize) {
				char key = -1;
				if (_kbhit()) key = toupper(_getch());
				gotoXY(37, 16);
				switch (key) {
				case 'A': case 75:
					if (n == 3) {
						cout << "<< 7 x 7 >>";
						n = 7;
					}
					else if (n == 5) {
						cout << "<< 3 x 3 >>";
						n = 3;
					}
					else {
						cout << "<< 5 x 5 >>";
						n = 5;
					}
					break;
				case 'D': case 77:
					if (n == 3) {
						cout << "<< 5 x 5 >>";
						n = 5;
					}
					else if (n == 5) {
						cout << "<< 7 x 7 >>";
						n = 7;
					}
					else {
						cout << "<< 3 x 3 >>";
						n = 3;
					}
					break;
				case 13:
					checkSize = false;
					if (n == 3) {
						cout << "3 x 3      ";
					}
					else if (n == 5) {
						cout << "5 x 5      ";
					}
					else {
						cout << "7 x 7      ";
					}
					break;
				}
			}
			splayer1 = tempS1;
			splayer2 = tempS2;
		}
		else if (mode == 2) {
			gotoXY(30, 12);
			cout << "Player: ";
			TextColor(3);
			player = new char[14];
			fflush(stdin);
			int x = whereX();
			int y = whereY();
			cin.getline(player,14,'\n');
			while (strlen(player) == 0) {
				gotoXY(x, y);
				cin.getline(player, 14, '\n');
			}
			TextColor(15);
			Nocursortype();
			gotoXY(30, 14);
			cout << "Board: ";
			TextColor(3);
			cout << "<< " << n << " x " << n << " >>";
			bool checkSize = true;
			while (checkSize) {
				char key = -1;
				if (_kbhit()) key = toupper(_getch());
				gotoXY(37, 14);
				switch (key) {
				case 'A': case 75:
					if (n == 3) {
						cout << "<< 7 x 7 >>";
						n = 7;
					}
					else if (n == 5) {
						cout << "<< 3 x 3 >>";
						n = 3;
					}
					else {
						cout << "<< 5 x 5 >>";
						n = 5;
					}
					break;
				case 'D': case 77:
					if (n == 3) {
						cout << "<< 5 x 5 >>";
						n = 5;
					}
					else if (n == 5) {
						cout << "<< 7 x 7 >>";
						n = 7;
					}
					else {
						cout << "<< 3 x 3 >>";
						n = 3;
					}
					break;
				case 13:
					checkSize = false;
					if (n == 3) {
						cout << "3 x 3      ";
					}
					else if (n == 5) {
						cout << "5 x 5      ";
					}
					else {
						cout << "7 x 7      ";
					}
					break;
				}
			}
			//Chọn ký tự 
			gotoXY(30, 16);
			TextColor(15);
			cout << "Symbol: ";
			TextColor(3);
			cout << "   " << tempS1 << " >>";
			bool checkSymbol = true;
			short chooseSymbol = 0;
			while (checkSymbol) {
				char key = -1;
				if (_kbhit()) key = toupper(_getch());
				gotoXY(38, 16);
				switch (key) {
				case 'A': case 75:
					chooseSymbol = 0;
					cout << "   " << tempS1 << " >>";
					break;
				case 'D': case 77:
					chooseSymbol = 1;
					cout << "<< " << tempS2 << "   ";
					break;
				case 13:
					checkSymbol = false;
					if (chooseSymbol == 0) {
						snguoi = tempS1;
						smay = tempS2;
						cout << snguoi << "      ";
					}
					else {
						snguoi = tempS2;
						smay = tempS1;
						cout << snguoi << "      ";
					}
					break;
				}
			}
			//Chọn mức độ: dễ - trung bình - khó
			TextColor(15);
			gotoXY(30, 18);
			cout << "Level: ";
			TextColor(3);
			cout << "   Easy >>  ";
			bool checkLevel = true;
			while (checkLevel) {
				char key = -1;
				if (_kbhit()) key = toupper(_getch());
				gotoXY(36, 18);
				switch (key) {
				case 'A': case 75:
					if (level == 0 || level == 1) {
						cout << "   Easy >>  ";
						level = 0;
					}
					else if (level == 2) {
						cout << "<< Medium >>";
						level = 1;
					}
					break;
				case 'D': case 77:
					if (level == 1 || level == 2) {
						cout << "<< Hard     ";
						level = 2;
					}
					else if (level == 0) {
						cout << "<< Medium >>";
						level = 1;
					}
					break;
				case 13:
					checkLevel = false;
					gotoXY(36, 18);
					if (level == 0) cout << " Easy       ";
					else if (level == 1) cout << " Medium     ";
					else cout << " Hard       ";
					break;
				}
			}
		}
		TextColor(15);
		PlaySound(NULL, NULL, SND_ASYNC);
		for (int i = 0; i <= 100; ++i) {
			gotoXY(70, 18);
			cout << "Loading " << i << "%";
			Sleep(30);
		}
		UnNocursortype();
	}

	void Setting(short choose) {
		Nocursortype();
		clrscr;
		DrawRectangle2(38, 5, 40, 17);
		gotoXY(52, 7);
		cout << "<< SETTINGS >>";
		bool check = true;
		short chooseSetting = 0;
		short chooseSymbol = 0;  // 0 -> Default; 1 -> Optional
		while (check) {
			TextColor(15);
			gotoXY(50, 10);
			cout << "Sound   :  " << sound;
			gotoXY(50, 12);
			cout << "Board   :  " << n << " x " << n;
			gotoXY(50, 14);
			if (chooseSymbol) cout << "Symbol  :  OPTIONAL?";
			else cout << "Symbol  :  DEFAULT  ";
			gotoXY(50, 20);
			cout << "<< Back to Menu";

			TextColor(3);
			switch (choose) {
			case 0:
				gotoXY(50, 10);
				cout << "Sound   :  " << sound;
				break;
			case 1:
				gotoXY(50, 12);
				cout << "Board   :  " << n << " x " << n;
				break;
			case 2:
				gotoXY(50, 14);
				if (chooseSymbol) cout << "Symbol  :  OPTIONAL?";
				else cout << "Symbol  :  DEFAULT  ";
				break;
			case 3:
				gotoXY(50, 20);
				cout << "<< Back to Menu";
				break;
			}


			char key = -1;
			if (_kbhit()) key = toupper(_getch());
			switch (key)
			{
			case 'A': case 75:
				if (choose == 0) {
					TextColor(3);
					if (sound == "ON ") {
						sound = "OFF";
						gotoXY(50, 10);
						cout << "Sound   :  " << sound;
						if (PlaySound(TEXT("music.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP) == true) {
							PlaySound(NULL, NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
						}
					}
					else {
						sound = "ON ";
						gotoXY(50, 10);
						cout << "Sound   :  " << sound;
						if (!PlaySound(TEXT("music.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP)) {
							PlaySound(TEXT("music.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
						}
					}
				}
				else if (choose == 1) {
					TextColor(3);
					gotoXY(50, 12);
					cout << "Board   :  ";
					gotoXY(61, 12);
					if (n == 5) {
						cout << "3 x 3";
						n = 3;
					}
					else if (n == 7) {
						cout << "5 x 5";
						n = 5;
					}
					else if (n == 3) {
						cout << "7 x 7";
						n = 7;
					}
				}
				else if (choose == 2) {
					TextColor(3);
					gotoXY(50, 14);
					if (chooseSymbol == 1) {
						cout << "Symbol  :  DEFAULT  ";
						tempS1 = 'X';
						tempS2 = 'O';
						gotoXY(53, 16);
						cout << "               ";
						gotoXY(53, 18);
						cout << "               ";
						chooseSymbol = 0;
					}
					else {
						cout << "Symbol  :  OPTIONAL?";
						char key = _getch();
						TextColor(15);
						if (key == 13) {
							gotoXY(53, 16);
							UnNocursortype();
							cout << "X  -->  ";
							cin >> tempS1;
							cin.ignore();
							gotoXY(53, 18);
							cout << "O  -->  ";
							cin >> tempS2;
							cin.ignore();
							Nocursortype();
						}
						chooseSymbol = 1;
					}
				}
				break;
			case 'D': case 77:
				if (choose == 0) {
					TextColor(3);
					if (sound == "ON ") {
						sound = "OFF";
						gotoXY(50, 10);
						cout << "Sound   :  " << sound;
						if (PlaySound(TEXT("Perfect-Game.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP))
							PlaySound(NULL, NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
					}
					else {
						sound = "ON ";
						gotoXY(50, 10);
						cout << "Sound   :  " << sound;
						if (!PlaySound(TEXT("Perfect-Game.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP))
							PlaySound(TEXT("Perfect-Game.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
					}
				}
				else if (choose == 1) {
					TextColor(3);
					gotoXY(50, 12);
					cout << "Board   :  ";
					gotoXY(61, 12);
					if (n == 5) {
						cout << "7 x 7";
						n = 7;
					}
					else if (n == 7) {
						cout << "3 x 3";
						n = 3;
					}
					else if (n == 3) {
						cout << "5 x 5";
						n = 5;
					}
				}
				else if (choose == 2) {
					TextColor(3);
					gotoXY(50, 14);
					if (chooseSymbol == 1) {
						cout << "Symbol  :  DEFAULT  ";
						tempS1 = 'X';
						tempS2 = 'O';
						gotoXY(53, 16);
						cout << "               ";
						gotoXY(53, 18);
						cout << "               ";
						chooseSymbol = 0;
					}
					else {
						cout << "Symbol  :  OPTIONAL?";
						char key = _getch();
						TextColor(15);
						if (key == 13) {
							gotoXY(53, 16);
							UnNocursortype();
							cout << "X  -->  ";
							cin >> tempS1;
							cin.ignore();
							gotoXY(53, 18);
							cout << "O  -->  ";
							cin >> tempS2;
							cin.ignore();
							Nocursortype();
						}
						chooseSymbol = 1;
					}
				}
				break;
			case 'W': case 72:
				choose--;
				break;
			case 'S': case 80:
				choose++;
				break;
			case 13: case 32:
				if (choose == 3) {
					check = false;
					Menu(1);
				}
			}
			if (choose < 0) choose = 3;
			if (choose > 3) choose = 0;
			Sleep(50);
		}
	}

	
};

void Update(_Menu Menu);
void Play(_Menu);

//Bot trong chế độ P vs C
struct _Bot {   
	int inputLevel;
	void PrintMove(char a[100][100], PorC& luot ,int x, int y, int startX, int startY) {
		gotoXY(startX + 4 * x - 2, startY + 2 * y - 1);
		cout << smay;
		a[tableX][tableY] = smay;
		gotoXY(startX + 4 * x - 2, startY + 2 * y - 1);
		luot = nguoi;
	}
	
	///Level dễ: random tại vị trí bất kỳ trong bảng, trừ những ô đã được đánh rồi
	void easyLevelStep(char a[100][100], PorC& luot, int startX, int startY) {
		while (true) {
			int x = rand() % (n - 1 + 1) + 1;  //Cho x là 1 giá trị random bất kỳ từ 1 đến n
			int y = rand() % (n - 1 + 1) + 1;  //Tương tự cho y random
			if (CheckWhetherAvailable(x,y,a)) {
				tableX = x;
				tableY = y;
				PrintMove(a, luot, x, y, startX, startY);
				return;
			}
		}
	}
	
	/// Level trung bình: random tại các ô lân cận với ô vừa được người chơi đánh, trừ các ô đã được đánh rồi
	void mediumLevelStep(char a[100][100], PorC& luot, int startX, int startY) {
		short count = 0;
		short ranX, ranY;
		while (true) {
			ranX = rand() % 3;
			ranY = rand() % 3;
			if (CheckWhetherAvailable(tableX + ranX - 1, tableY + ranY - 1, a) && tableX + ranX - 1 >= 1 && tableX + ranX - 1 <= n && tableY + ranY - 1 >= 1 && tableY + ranY - 1 <= n) {
				tableX = tableX + ranX - 1;
				tableY = tableY + ranY - 1;
				PrintMove(a, luot, tableX, tableY, startX, startY);
				return;
			}
			else count++;
			if (count == 9) {
				short rX, rY;
				while (true) {
					rX = rand() % n + 1;
					rY = rand() % n + 1;
					if (CheckWhetherAvailable(rX, rY, a)) {
						tableX = rX;
						tableY = rY;
						PrintMove(a, luot, tableX, tableY, startX, startY);
						return;
					}
				}
			}
		}
	}

	///Level khó: phân ra hai thuật toán: cho 3x3 + 5x5 và cho 7x7
	void tinhDiem(char a[100][100]) {
		for (int i = 1; i <= n; ++i) {
			for (int j = 1; j <= n; ++j) {
				diem[i][j] = 0;
				if (i == j || j == n - i + 1) {
					diem[j][i]++;
				}
			}
		}

		int p = 0, q = 0;
		for (int i = 1; i <= n; ++i) {
			if (checkCot(a, i, snguoi) && checkCot(a, i, smay)) {
				for (int k = 1; k <= n; k++)
					diem[i][k] = 0;
			}
			if (checkHang(a, i, snguoi) && checkHang(a, i, smay))
				for (int k = 1; k <= n; k++)
					diem[k][i] = 0;
		}

		p = 0, q = 0;
		if ((p = checkDuongCheo1(a, snguoi)) && (q = checkDuongCheo1(a, smay)))
			for (int k = 1; k <= n; k++)
				diem[k][k] = 0;
		else
			for (int k = 1; k <= n; k++)
				diem[k][k] += (p > q ? p : q);

		if ((p = checkDuongCheo2(a, snguoi)) && (q = checkDuongCheo2(a, smay)))
			for (int k = 1; k <= n; k++)
				diem[n - k + 1][k] = 0;
		else
			for (int k = 1; k <= n; k++)
				diem[n - k + 1][k] += (p > q ? p : q);

		for (int i = 1; i <= n; ++i) {
			p = 0; q = 0;
			if (((p = checkHang(a, i, snguoi) && !checkHang(a, i, smay)) || ((q = checkHang(a, i, smay) && !checkHang(a, i, snguoi))))) {
				for (int k = 1; k <= n; ++k) {
					diem[k][i] += (p > q ? p : q);
				}
			}
			p = 0; q = 0;
			if (((p = checkCot(a, i, snguoi) && !checkCot(a, i, smay))) || ((q = checkCot(a, i, smay) && !checkCot(a, i, snguoi)))) {
				for (int k = 1; k <= n; ++k) {
					diem[i][k] += (p > q ? p : q);
				}
			}

			int empty = 0;
			if((empty = checkHang(a, i, ' ')) && !checkHang(a,i,snguoi) && !checkHang(a,i,smay))
				for (int k = 1; k <= n; ++k) {
					diem[k][i] += 1;
				}

			empty = 0;
			if ((empty = checkCot(a, i, ' ')) && !checkCot(a, i, snguoi) && !checkCot(a, i, smay))
				for (int k = 1; k <= n; ++k)
					diem[i][k] += 1;
		}

		for (int i = 1; i <= n; ++i) {
			for (int j = 1; j <= n; ++j) {
				if (a[i][j] == snguoi || a[i][j] == smay) {
					diem[i][j] = -1;
				}
			}
		}
	}
	void hardLevelStep5x5 (char a[100][100], PorC& luot, int startX, int startY) {
		int k = 1, l = 1;
		tinhDiem(a);
		for (int i = 1; i <= n; ++i) {
			for (int j = 1; j <= n; ++j) {
				if (diem[k][l] < diem[i][j] && CheckWhetherAvailable(i, j, a)) {
					k = i;
					l = j;
				}
			}
		}
		tableX = k;
		tableY = l;
		PrintMove(a, luot, tableX, tableY, startX, startY);
		return;
	}


	//Đếm số ô liên tục trong cùng hàng có nước đi giống nhau: oxxx -> countHor = 3
	void CountConsecutive_Horizontal(char a[100][100], int hang, int i, int dau, char luot, int& countHor) {
		while (true) {
			if (a[i][hang] != luot || i < 1 || i > n) break;
			countHor++;
			i += dau;
		}
		return;
	}
	//Đếm số ô liên tục trong cùng cột có nước đi giống nhau
	void CountConsecutive_Vertical(char a[100][100], int cot, int j, int dau, char luot, int& countVer) {
		while (true) {
			if (a[cot][j] != luot || j < 1 || j > n) break;
			countVer++;
			j += dau;
		}
		return;
	}
	//Đếm số ô liên tục trong cùng đường chéo thứ nhất có nước đi giống nhau.
	void CountConsecutive_Diagonal1(char a[100][100], int i, int j, int dau, char luot, int& countDi1) {
		while (true) {
			if (a[i][j] != luot || i < 1 || i > n || j < 1 || j > n) break;
			countDi1++;
			i += dau;
			j += dau;
		}
	}
	//Đếm số ô liên tục trong cùng đường chéo thứ hai có nước đi giống nhau.
	void CountConsecutive_Diagonal2(char a[100][100], int i, int j, int dau, char luot, int& countDi2) {
		while (true) {
			if (a[i][j] != luot || i < 1 || i > n || j < 1 || j > n) break;
			countDi2++;
			i += dau;
			j -= dau;
		}
	}
	void tinhdiem7x7(char a[100][100]) {
		for (int j = 1; j <= n; ++j) {
			for (int i = 1; i <= n; ++i) {
				diem[i][j] = 0;
			}
		}
		for (int j = 1; j <= n; ++j) {
			for (int i = 1; i <= n; ++i) {
				if (a[i][j] != smay && a[i][j] != snguoi) {
					int countHor = 0, countVer = 0, countDi1 = 0, countDi2 = 0;
					CountConsecutive_Horizontal(a, j, i + 1, 1, smay, countHor);
					if (countHor == 4) {
						diem[i][j] += 100;
						return;
					}
					else if (countHor == 3 && CheckWhetherAvailable(i + 4, j, a)) {
						diem[i][j] += 30;
					}
					countHor = 0;
					CountConsecutive_Horizontal(a, j, i - 1, -1, smay, countHor);
					if (countHor == 4)
					{
						diem[i][j] += 100;
						return;
					}
					else if (countHor == 3 && CheckWhetherAvailable(i - 4, j, a)) {
						diem[i][j] += 30;
					}
					CountConsecutive_Vertical(a, i, j + 1, 1, smay, countVer);
					if (countVer == 4)
					{
						diem[i][j] += 100;
						return;
					}
					else if (countVer == 3 && CheckWhetherAvailable(i, j + 4, a)) {
						diem[i][j] += 30;
					}
					countVer = 0;
					CountConsecutive_Vertical(a, i, j - 1, -1, smay, countVer);
					if (countVer == 4)
					{
						diem[i][j] += 100;
						return;
					}
					else if (countVer == 3 && CheckWhetherAvailable(i, j - 4, a)) {
						diem[i][j] += 30;
					}
					CountConsecutive_Diagonal1(a, i + 1, j + 1, 1, smay, countDi1);
					if (countDi1 == 4)
					{
						diem[i][j] += 100;
						return;
					}
					else if (countDi1 == 3 && CheckWhetherAvailable(i + 4, j + 4, a)) {
						diem[i][j] += 30;
					}
					countDi1 = 0;
					CountConsecutive_Diagonal1(a, i - 1, j - 1, -1, smay, countDi1);
					if (countDi1 == 4)
					{
						diem[i][j] += 100;
						return;
					}
					else if (countDi1 == 3 && CheckWhetherAvailable(i - 4, j - 4, a)) {
						diem[i][j] += 30;
					}
					CountConsecutive_Diagonal2(a, i + 1, j - 1, 1, smay, countDi2);
					if (countDi2 == 4)
					{
						diem[i][j] += 100;
						return;
					}
					else if (countDi2 == 3 && CheckWhetherAvailable(i + 4, j - 4, a)) {
						diem[i][j] += 30;
					}
					countDi2 = 0;
					CountConsecutive_Diagonal2(a, i - 1, j + 1, -1, smay, countDi2);
					if (countDi2 == 4)
					{
						diem[i][j] += 100;
						return;
					}
					else if (countDi2 == 3 && CheckWhetherAvailable(i - 4, j + 4, a)) {
						diem[i][j] += 30;
					}


					countHor = 0; countVer = 0; countDi1 = 0; countDi2 = 0;
					CountConsecutive_Horizontal(a, j, i + 1, 1, snguoi, countHor);
					if (countHor == 4)
					{
						diem[i][j] += 100;
						return;
					}
					else if (countHor == 3 && CheckWhetherAvailable(i+4,j,a)) {
						diem[i][j] += 30;
					}
					else {
						diem[i][j] += countHor;
						countHor = 0;
					}
					
					CountConsecutive_Horizontal(a, j, i - 1, -1, snguoi, countHor);
					if (countHor == 4)
					{
						diem[i][j] += 100;
						return;
					}
					else if (countHor == 3 && CheckWhetherAvailable(i - 4,j,a)) {
						diem[i][j] += 30;
					}
					else {
						diem[i][j] += countHor;
						countHor = 0;
					}

					CountConsecutive_Vertical(a, i, j + 1, 1, snguoi, countVer);
					if (countVer == 4)
					{
						diem[i][j] += 100;
						return;
					}
					else if (countVer == 3 && CheckWhetherAvailable(i,j+4,a)) {
						diem[i][j] += 30;
					}
					else {
						diem[i][j] += countVer;
						countVer = 0;
					}
					CountConsecutive_Vertical(a, i, j - 1, -1, snguoi, countVer);
					if (countVer == 4)
					{
						diem[i][j] += 100;
						return;
					}
					else if (countVer == 3 && CheckWhetherAvailable(i,j - 4,a)) {
						diem[i][j] += 30;
					}
					else {
						diem[i][j] += countVer;
							countVer = 0;
					}

					CountConsecutive_Diagonal1(a, i + 1, j + 1, 1, snguoi, countDi1);
					if (countDi1 == 4)
					{
						diem[i][j] += 100;
						return;
					}
					else if (countDi1 == 3 && CheckWhetherAvailable(i+4,j+4,a)) {
						diem[i][j] += 30;
					}
					else {
						diem[i][j] += countDi1;
						countDi1 = 0;
					}
					CountConsecutive_Diagonal1(a, i - 1, j - 1, -1, snguoi, countDi1);
					if (countDi1 == 4)
					{
						diem[i][j] += 100;
						return;
					}
					else if (countDi1 == 3 && CheckWhetherAvailable(i - 4, j - 4, a) ) {
						diem[i][j] += 30;
					}
					else {
						diem[i][j] += countDi1;
						countDi1 = 0;
					}

					CountConsecutive_Diagonal2(a, i + 1, j - 1, 1, snguoi, countDi2);
					if (countDi2 == 4)
					{
						diem[i][j] += 100;
						return;
					}
					else if (countDi2 == 3 && CheckWhetherAvailable(i + 4, j - 4, a)) {
						diem[i][j] += 30;
					}
					else {
						diem[i][j] += countDi2;
						countDi2 = 0;
					}
					CountConsecutive_Diagonal2(a, i - 1, j + 1, -1, snguoi, countDi2);
					if (countDi2 == 4)
					{
						diem[i][j] += 100;
						return;
					}
					else if (countDi2 == 3 && CheckWhetherAvailable(i - 4, j + 4, a)) {
						diem[i][j] += 30;
					}
					else {
						diem[i][j] += countDi2;
						countDi2 = 0;
					}
					
				}
			}
		}
	}
	void hardLevelStep7x7 (char a[100][100], PorC & luot, int startX, int startY) {
		int k = 1, l = 1;
		tinhdiem7x7(a);
		for (int i = 1; i <= n; ++i) {
			for (int j = 1; j <= n; ++j) {
				if (diem[k][l] < diem[i][j] && CheckWhetherAvailable(i, j, a)) {
					k = i;
					l = j;
				}
			}
		}
		tableX = k;
		tableY = l;
		PrintMove(a, luot, tableX, tableY, startX, startY);
		return;
	}
};

//Kiểu dữ liệu Player
struct _Player {
	char* name = new char[14];
	int order = 0;
	int thenumofWin = 0, thenumofLose = 0, thenumofDraw = 0;
	int scoreRank;
	short time = 0;

	void MovingCursorEvent1(int& x, int& y, char a[100][100], int& turn, int startX, int startY,_Menu Menu) {
		char key_press = -1;
		if (turn == 1) {   //Đến lượt thì mới được di chuyển
			if (_kbhit()) key_press = toupper( _getch());
			switch (key_press) {
			case 72:  //Đi lên
				if (tableY > 1) {   //Câu lệnh if để giới hạn con trỏ không vượt ra khỏi bản
					gotoXY(x, y - 2);
					tableY--;
					break;
				}
				else {  //Nếu con trỏ đã ở sát cạnh trên thì sau đó nó sẽ trở về cuối bảng
					gotoXY(x,startY + 2 * n - 1);
					tableY = n;
					break;
				}
			case 80:  //Đi xuống
				if (tableY < n) {
					gotoXY(x, y + 2);
					tableY++;
					break;
				}
				else {
					gotoXY(x,startY + 1);
					tableY = 1;
					break;
				}
			case 75:  //Sang trái
				if (tableX > 1) {
					gotoXY(x - 4, y);
					tableX--;
					break;
				}
				else {
					gotoXY(startX + 4 * n - 2, y);
					tableX = n;
					break;
				}
			case 77	:  //Sang phải
				if (tableX < n) {
					gotoXY(x + 4, y);
					tableX++;
					break;
				}
				else {
					gotoXY(startX +2, y);
					tableX = 1;
					break;
				}
			case 13:   //Phím Enter để đánh "X" vào ô di chuyển đến
				if (CheckWhetherAvailable(tableX, tableY, a)) {
					cout << splayer1;
					a[tableX][tableY] = splayer1;
					gotoXY(x, y);
					turn = 2;//Nếu đã xuất ra thì không thể di chuyển cho đến lượt của mình
				}
				else turn = 1;
				break;
			case 27:
				SetWindowSize(115, 30);
				Update(Menu);
				break;
			case 59:
				Play(Menu);
				break;
			case 60:
				if (sound == "ON ") {
					PlaySound(NULL, NULL, SND_FILENAME | SND_ASYNC );
					sound = "OFF";
					break;
				}
				if (sound == "OFF") {
					sound = "ON ";
					break;
				}
			case 62:
				clrscr();
				SetWindowSize(115, 30);
				if (sound == "ON ") {
					PlaySound(TEXT("Result-screen.wav"), NULL, SND_ASYNC);
				}
				DrawRectangle2(48, 11, 18, 1);
				Sleep(100);
				gotoXY(50, 12);
				cout << "THANKS FOR USED!";
				gotoXY(45, 20);
				Sleep(100);
				system("pause");
				exit(0);
				break;
			}

		}
	}

	void MovingCursorEvent2(int& x, int& y, char a[100][100], int& turn, int startX, int startY, _Menu Menu) {
		if (turn == 2) {   //Đến lượt thì mới được di chuyển
			char key_press = -1;
			if (_kbhit()) key_press = toupper(_getch());
			switch (key_press) {
			case 'W':  //Đi lên (w)
				if (tableY > 1) {   //Câu lệnh if để giới hạn con trỏ không vượt ra khỏi bản
					gotoXY(x, y - 2);
					tableY--;
					break;
				}
				else {  //Nếu con trỏ đã ở sát cạnh trên thì sau đó nó sẽ trở về cuối bảng
					gotoXY(x,startY + 2 * n - 1);
					tableY = n;
					break;
				}
			case 'S':  //Đi xuống (s)
				if (tableY < n) {
					gotoXY(x, y + 2);
					tableY++;
					break;
				}
				else {
					gotoXY(x, startX + 1);
					tableY = 1;
					break;
				}
			case 'A':  //Sang trái
				if (tableX > 1) {
					gotoXY(x - 4, y);
					tableX--;
					break;
				}
				else {
					gotoXY(startX + 4 * n - 2, y);
					tableX = n;
					break;
				}
			case 'D':  //Sang phải
				if (tableX < n) {
					gotoXY(x + 4, y);
					tableX++;
					break;
				}
				else {
					gotoXY(startX + 2, y);
					tableX = 1;
					break;
				}
			case 13:   //Phím Enter để đánh "0" vào ô di chuyển đến
				if (CheckWhetherAvailable(tableX, tableY, a)) {
					cout << splayer2;
					a[tableX][tableY] = splayer2;
					gotoXY(x, y);
					turn = 1;;//Nếu đã Enter thì không thể di chuyển cho đến lượt của mình
				}
				else turn = 2;
				break;
			case 27:
				SetWindowSize(115, 30);
				Update(Menu);
				break;
			case 59:
				Play(Menu);
				break;
			case 60:
				if (sound == "ON ") {
					PlaySound(NULL, NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
					sound = "OFF";
					break;
				}
				if (sound == "OFF") {
					sound == "ON ";
					break;
				}
			case 62:
				clrscr();
				SetWindowSize(115, 30);
				if (sound == "ON ") {
					PlaySound(TEXT("Result-screen.wav"), NULL, SND_ASYNC);
				}
				DrawRectangle2(48, 11, 18, 1);
				Sleep(100);
				gotoXY(50, 12);
				cout << "THANKS FOR USED!";
				gotoXY(45, 20);
				Sleep(100);
				system("pause");
				exit(0);
				break;
			}
		}
	}

	void MovingCursorEventFromPlayer(int& x, int& y, char a[100][100], PorC& luot, int startX, int startY, _Menu Menu) {
		char key_press;
		int ascii_value;
		if (luot == nguoi) {   //Đến lượt thì mới được di chuyển
			key_press = _getch();
			ascii_value = key_press;
			switch (ascii_value) {
			case 72:  //Đi lên
				if (tableY > 1) {   //Câu lệnh if để giới hạn con trỏ không vượt ra khỏi bản
					gotoXY(x, y - 2);
					tableY--;
					break;
				}
				else {  //Nếu con trỏ đã ở sát cạnh trên thì sau đó nó sẽ trở về cuối bảng
					gotoXY(x,startY + 2 * n - 1);
					tableY = n;
					break;
				}
			case 80:  //Đi xuống
				if (tableY < n) {
					gotoXY(x, y + 2);
					tableY++;
					break;
				}
				else {
					gotoXY(x,startY + 1);
					tableY = 1;
					break;
				}
			case 75:  //Sang trái
				if (tableX > 1) {
					gotoXY(x - 4, y);
					tableX--;
					break;
				}
				else {
					gotoXY(startX + 4 * n - 2, y);
					tableX = n;
					break;
				}
			case 77:  //Sang phải
				if (tableX < n) {
					gotoXY(x + 4, y);
					tableX++;
					break;
				}
				else {
					gotoXY(startX + 2, y);
					tableX = 1;
					break;
				}
			case 13:   //Phím Enter để đánh "X" vào ô di chuyển đến
				if (CheckWhetherAvailable(tableX, tableY, a)) {
					cout << snguoi;
					a[tableX][tableY] = snguoi;
					gotoXY(x, y);
					luot = may;//Nếu đã xuất ra thì không thể di chuyển cho đến lượt của mình
				}
				else luot = nguoi;
				break;
			case 27:
				SetWindowSize(115, 30);
				Update(Menu);
				break;
			case 59:
				Play(Menu);
				break;
			case 60:
				if (sound == "ON ") {
					PlaySound(NULL, NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
					sound = "OFF";
					break;
				}	
				if (sound == "OFF") {
					sound = "ON ";
					break;
				}
			case 62:
				clrscr();
				SetWindowSize(115, 30);
				if (sound == "ON ") {
					PlaySound(TEXT("Result-screen.wav"), NULL, SND_ASYNC);
				}
				DrawRectangle2(48, 11, 18, 1);
				Sleep(100);
				gotoXY(50, 12);
				cout << "THANKS FOR USED!";
				gotoXY(45, 20);
				Sleep(100);
				system("pause");
				exit(0);
				break;
			}
		}
	}
};
_Player statsPlayer[20];  //Tạo một mảng kiểu Player để lưu xếp hạng
int sizeStats;
bool CheckStatistics(_Player player);
void Play(_Menu Menu) {
	_Board board;
	if (n == 7) board.DrawBoard(n, 10, 1, mode, level);
	else if (n == 5) board.DrawBoard(n, 14, 3, mode, level);
	else board.DrawBoard(n, 18, 5, mode, level);
	if (sound == "ON ") {
		PlaySound(NULL, NULL, SND_ASYNC);
		PlaySound(TEXT("Game-start.wav"), NULL, SND_ASYNC);
	}

	char arr[100][100];
	bool isEnd = false;  //Biến boolean kiểm tra xem đã có người thắng hay chưa
	int x, y;
	short winner = 0;
	short win = 5; //Biến kiểm soát số lượng để chiến thắng
	if (n == 3) win = 3;
	else win = 5;
	tableX = 1;
	tableY = 1;
	for (int i = 1; i <= n; ++i) {
		for (int j = 1; j <= n; ++j) {
			arr[i][j] = ' ';
		}
	} // Khởi tạo mảng để lưu trạng thái bàn cờ với các ô ban đầu đều là rỗng 
	if (mode == 1) {
		_Player player1;
		_Player player2;
		player1.name = Menu.player1;
		player2.name = Menu.player2;
		gotoXY(68, 11);
		cout << Menu.player1;
		gotoXY(68, 13);
		cout << splayer1;
		gotoXY(68, 21);
		cout << Menu.player2;
		gotoXY(68, 23);
		cout << splayer2;
		gotoXY(board.startX + 2, board.startY + 1);
		int turn1 = 1;  //Biến giữ vai trò kiếm soát xem đến lượt của ai: turn1 = 1 -> P1, turn1 = 2 -> P2
		while (isEnd == false) {
			x = whereX();
			y = whereY();
			int key = -1;
			if (turn1 == 1) {
				player1.MovingCursorEvent1(x, y, arr, turn1, board.startX, board.startY, Menu);
			}
			else {
				player2.MovingCursorEvent2(x, y, arr, turn1, board.startX, board.startY, Menu);
			}
			CheckHorizontal(arr, isEnd, win, splayer1, splayer2, winner);
			CheckVertical(arr, isEnd, win, splayer1, splayer2, winner);
			CheckTheFirstDiagonal(arr, isEnd, win, splayer1, splayer2, winner);
			CheckTheSecondDiagonal(arr, isEnd, win, splayer1, splayer2, winner);
			isDraw(arr, isEnd, winner);
		}

		if (winner == 1) {
			if(sound == "ON ")
				PlaySound(TEXT("Win.wav"), NULL, SND_ASYNC);
			player1.thenumofWin = 1;
			player1.thenumofLose = 0;
			player1.thenumofDraw = 0;
			player1.scoreRank = 1;
			player2.thenumofWin = 0;
			player2.thenumofLose = 1;
			player2.thenumofDraw = 0;
			player2.scoreRank = -1;
		}
		else if (winner = 2) {
			if(sound == "ON ")
				PlaySound(TEXT("Win.wav"), NULL, SND_ASYNC);
			player1.thenumofWin = 0;
			player1.thenumofLose = 1;
			player1.thenumofDraw = 0;
			player1.scoreRank = -1;
			player2.thenumofWin = 1;
			player2.thenumofLose = 0;
			player2.thenumofDraw = 0;
			player2.scoreRank = 1;
		}
		else {
			if (sound == "ON ") {
				PlaySound(TEXT("Draw.wav"), NULL, SND_ASYNC);
			}
			player1.thenumofWin = 0;
			player1.thenumofLose = 0;
			player1.thenumofDraw = 1;
			player1.scoreRank = 0;
			player2.thenumofWin = 0;
			player2.thenumofLose = 0;
			player2.thenumofDraw = 1;
			player2.scoreRank = 0;
		}
		if (CheckStatistics(player1)) {
			UpdateStatistics();
		}
		else cout << "Statistic File is full! Cannot add player's information!";
		if (CheckStatistics(player2)) {
			UpdateStatistics();
		}
		else cout << "Statistic File is full! Cannot add player's information!";
		gotoXY(8, 19);
		cout << "Would you like to play again? (y/n): ";
		char choice;
		cin >> choice;
		cin.ignore();
		if (choice == 'y' || choice == 'Y') { Play(Menu); }
		else  {
			clrscr();
			Sleep(100);
			if (sound == "ON ") {
				PlaySound(TEXT("Result-screen.wav"), NULL, SND_ASYNC);
			}
			SetWindowSize(115, 30);
			Sleep(100);
			DrawRectangle2(48, 11, 18, 1);
			gotoXY(50, 12);
			cout << "THANKS FOR USED!";
			gotoXY(45, 20);
			Sleep(100);
			system("pause");
			exit(0);
		}
	}
	else {
		PorC luot = may;
		_Bot bot;
		_Player player;
		player.name = Menu.player;
		gotoXY(68, 11);
		cout << "Computer";
		gotoXY(68, 13);
		cout << smay;
		gotoXY(68, 21);
		cout << Menu.player;
		gotoXY(68, 23);
		cout << snguoi;
		gotoXY(board.startX + 2, board.startY + 1);
		while (isEnd == false) {
			x = whereX();
			y = whereY();
			if (luot == may) {
				if (level == 0) {
					bot.easyLevelStep(arr, luot, board.startX, board.startY);
				}
				else if (level == 1) {
					bot.mediumLevelStep(arr, luot, board.startX, board.startY);
				}
				else {
					if (n == 3 || n == 5) {
						bot.hardLevelStep5x5(arr, luot, board.startX, board.startY);
					}
					else {
						bot.hardLevelStep7x7(arr, luot, board.startX, board.startY);
					}
				}
			}
			else {
				player.MovingCursorEventFromPlayer(x, y, arr, luot, board.startX, board.startY, Menu);
			}
			CheckHorizontal(arr, isEnd, win, smay, snguoi, winner);
			CheckVertical(arr, isEnd, win, smay, snguoi, winner);
			CheckTheFirstDiagonal(arr, isEnd, win, smay, snguoi, winner);
			CheckTheSecondDiagonal(arr, isEnd, win, smay, snguoi, winner);
			isDraw(arr, isEnd, winner);
		}
		if (winner == 1) {
			if (sound == "ON ") {
				PlaySound(TEXT("Lose.wav"), NULL, SND_ASYNC);
			}
			player.thenumofWin = 0;
			player.thenumofLose = 1;
			player.thenumofDraw = 0;
			player.scoreRank = -1;
		}
		else if (winner == 2) {
			if (sound == "ON ") {
				PlaySound(TEXT("Win.wav"), NULL, SND_ASYNC);
			}
			player.thenumofWin = 1;
			player.thenumofLose = 0;
			player.thenumofDraw = 0;
			player.scoreRank = 1;
		}
		else {
			if (sound == "ON ") {
				PlaySound(TEXT("Draw.wav"), NULL, SND_ASYNC);
			}
			player.thenumofWin = 0;
			player.thenumofLose = 0;
			player.thenumofDraw = 1;
			player.scoreRank = 0;
		}
		if (CheckStatistics(player)) {
			UpdateStatistics();
		}
		else cout << "Statistic File is full! Cannot add player's information!";
		gotoXY(8, 19);
		cout << "Would you like to play again? (y/n): ";
		char choice;
		cin >> choice;
		cin.ignore();  // Ăn mất ký tự '\n' sau chữ y
		if (choice == 'y' || choice == 'Y') { Play(Menu); }
		else  {
			clrscr();
			Sleep(100);
			if (sound == "ON ") {
				PlaySound(TEXT("Result-screen.wav"), NULL, SND_ASYNC);
			}
			SetWindowSize(115, 30);
			Sleep(100);
			DrawRectangle2(48, 11, 18, 1);
			gotoXY(50, 12);
			cout << "THANKS FOR USED!";
			gotoXY(45, 20);
			Sleep(100);
			system("pause");
			exit(0);
		}
	}
}
void SplashScreen() {
	Nocursortype();
	DrawRectangle2(15, 3, 90, 20);
	TextColor(14);
	gotoXY(45, 5);
	cout << "<< WELCOME TO GOMOKUHCMUS 1.0 >>";
	TextColor(15);
	PlaySound(TEXT("Title.wav"), NULL, SND_ASYNC);
	Sleep(100);
	gotoXY(32, 10);
	cout << (char)254 <<" GOMOKUHCMUS 1.0 is a mini game created on a platform" << endl;
	Sleep(100);
	gotoXY(32, 12);
	cout << "of a console application";
	Sleep(100);
	gotoXY(32, 14);
	cout<< (char)254 << " It stimulates a popular folk game named Gomoku";
	Sleep(100);
	gotoXY(32, 16);
	cout << "or Caro";
	Sleep(100);
	TextColor(12);
	gotoXY(20, 9); 
	cout << " __    __";
	Sleep(100);
	gotoXY(20, 10);
	cout << "|  |__|  |";
	Sleep(100);
	gotoXY(20, 11);
	cout << "|        |";
	Sleep(100);
	gotoXY(20, 12);
	cout << "|_      _|";
	Sleep(100);
	gotoXY(20, 13);
	cout << "  |    |";
	Sleep(100);
	gotoXY(20, 14);
	cout << " _|    |_";
	Sleep(100);
	gotoXY(20, 15);
	cout << "|        |";
	Sleep(100);
	gotoXY(20, 16);
	cout << "|   __   |";
	Sleep(100);
	gotoXY(20, 17);
	cout << "|__|  |__|";
	TextColor(9);
	Sleep(100);
	gotoXY(88, 9);
	cout << " __________ ";
	Sleep(100);
	gotoXY(88, 10);
	cout << "|          |";
	Sleep(100);
	gotoXY(88, 11);
	cout << "|          |";
	Sleep(100);
	gotoXY(88, 12);
	cout << "|   ____   |";
	Sleep(100);
	gotoXY(88, 13);
	cout << "|  |    |  |";
	Sleep(100);
	gotoXY(88, 14);
	cout << "|  |____|  |";
	Sleep(100);
	gotoXY(88, 15);
	cout << "|          |";
	Sleep(100);
	gotoXY(88, 16);
	cout << "|          |";
	Sleep(100);
	gotoXY(88, 17);
	cout << "|__________|";
	TextColor(15);
	DrawRectangle1(83, 20,9,1);
	TextColor(3);
	gotoXY(85, 21);
	cout << "NEXT >>";
	char key = -1;
	TextColor(15);
	bool check = true;
	while(check){
		if (_kbhit()) key = _getch();
		if (key == 13 || key == 77) {
			clrscr();
			PlaySound(TEXT("Perfect-Game.wav"), NULL, SND_ASYNC | SND_LOOP);
			Sleep(100);
			DrawRectangle2(37, 11, 38, 1);
			gotoXY(40, 12);
			cout << ">> Hope you have great feeling! <<";
			for (int i = 0; i <= 100; ++i) {
				gotoXY(52, 16);
				cout << "Loading " << i << "%";
				Sleep(50);
			}
			check = false;
		}
	}

	return;
}
void Update(_Menu Menu) {
	SetWindowSize(118, 30);
	
	ReadStatistics();
	Menu.Menu(0);
	Play(Menu);
}


/*Kiểm tra ô trống*/
bool CheckWhetherAvailable(int x, int y, char a[100][100]) {
	int m = x;
	int n = y;
	if (mode == 1) {
		if (a[m][n] != splayer1 && a[m][n] != splayer2) return true;
		else return false;
	}
	else {
		if (a[m][n] != snguoi && a[m][n] != smay)  return true;
		else return false;
	}
}

/*Xử lý thắng thua*/
void CheckHorizontal(char a[100][100],bool& isEnd, int win, char s1, char s2, short&winner) {
	if (!isEnd) {
		for (int j = 1; j <= n; ++j) {  //Kiểm từng hàng
			int dem1 = 0, dem2 = 0;
			for (int i = 1; i <= n; ++i) {  //Kiểm từng cột
				if (a[i][j] == s1) {
					dem1++;
					if (dem1 >= win) {
						gotoXY(13, 17);
						cout << "Player 1 is the winner!" << endl;
						winner = 1;
						isEnd = true;
						return;
					}
				}
				else {
					if (dem1 < win) dem1 = 0;
				}

				if (a[i][j] == s2) {
					dem2++;
					if (dem2 >= win) {
						gotoXY(13, 17);
						cout << "Player 2 is the winner!" << endl;
						winner = 2;
						isEnd = true;
						return;
					}
				}
				else {
					if (dem2 < win) dem2 = 0;
				}
			}
		}
	}
}
//------------------------------------------------------------------
void CheckVertical(char a[100][100], bool& isEnd, int win, char s1, char s2, short& winner) {
	if (!isEnd) {
		for (int i = 1; i <= n; ++i) {  //Kiểm từng cột
			int dem1 = 0, dem2 = 0;
			for (int j = 1; j <= n; ++j) {  //Kiểm từng hàng
				if (a[i][j] == s1) {
					dem1++;
					if (dem1 >= win) {
						gotoXY(13, 17);
						cout << "Player 1 is the winner!" << endl;
						winner = 1;
						isEnd = true;
						return;
					}
				}
				else {
					if (dem1 < win) dem1 = 0;
				}

				if (a[i][j] == s2) {
					dem2++;
					if (dem2 >= win) {
						gotoXY(13,17);
						cout << "Player 2 is the winner!" << endl;
						winner = 2;
						isEnd = true;
						return;
					}
				}
				else {
					if (dem2 < win) dem2 = 0;
				}
			}
		}
	}
}
//------------------------------------------------------------------
void CheckTheFirstDiagonal(char a[100][100],bool& isEnd, int win, char s1, char s2, short& winner) {
	if (!isEnd) {
		int dem1 = 0, dem2 = 0;
		int m = 1; int k = 1;
		for (int j = 1; j <= n; ++j) {
			for (int i = 1; i <= n; ++i) {
				dem1 = 0;
				dem2 = 0;
				k = i;
				m = j;
				while (k <= n && m <= n) {
					if (a[k][m] == s1) {
						dem1++;
						if (dem1 >= win) {
							gotoXY(13, 17);
							cout << "Player 1 is the winner!" << endl;
							winner = 1;
							isEnd = true;
							return;
						}
					}
					else {
						if (dem1 < win) dem1 = 0;
					}
					if (a[k][m] == s2) {
						dem2++;
						if (dem2 >= win) {
							gotoXY(13, 17);
							cout << "Player 2 is the winner!" << endl;
							winner = 2;
							isEnd = true;
							return;
						}
					}
					else {
						if (dem2 < win) dem2 = 0;
					}
					m++;
					k++;
				}
			}
		}
	}
}
//------------------------------------------------------------------
void CheckTheSecondDiagonal(char a[100][100], bool& isEnd,int win, char s1, char s2, short& winner) {
	if (!isEnd) {
		int dem1 = 0, dem2 = 0;
		int m = n, k = 1;
		for (int j = 1; j <= n; ++j) {
			for (int i = n; i >= 1; --i) {
				dem1 = 0;
				dem2 = 0;
				m = j;
				k = i;
				while (m <= n && k >= 1) {
					if (a[k][m] == s1) {
						dem1++;
						if (dem1 >= win) {
							gotoXY(13, 17);
							cout << "Player 1 is the winner!" << endl;
							winner = 1;
							isEnd = true;
							return;
						}
					}
					else {
						if (dem1 < win) dem1 = 0;
					}
					if (a[k][m] == s2) {
						dem2++;
						if (dem2 >= win) {
							gotoXY(13, 17);
							cout << "Player 2 is the winner!" << endl;
							winner = 2;
							isEnd = true;
							return;
						}
					}
					else {
						if (dem2 < win) dem2 = 0;
					}
					k--;
					m++;
				}
			}
		}
	}
}
//------------------------------------------------------------------
void isDraw(char a[100][100], bool& isEnd, short& winner) {
	if (!isEnd) {
		int count = 0;
		for (int j = 1; j <= n; ++j) {
			for (int i = 1; i <= n; ++i) {
				if (a[i][j] == ' ') {
					return;
				}
			}
		}
		if (count == 0) {
			gotoXY(23, 17);
			cout << "DRAW!";
			winner = 0;
			isEnd = true;
		}
	}
	return;
}

/*Statistics*/
//Hàm in dữ liệu đã đọc ra console
void Statistics(short& time) {
	//SetWindowSize(118, 30);
	DrawRectangle2(3, 1, 112, 26);
	for (int i = 5; i <= 24; ++i) {
		gotoXY(60, i);
		cout << (char)179;
	}
	gotoXY(52, 2);
	cout << "<< STATISTICS >>";
	TextColor(14);
	gotoXY(5, 5);
	cout << "Order" << "  " << "Name" << "	" << "Win" << "\t" << "Lose" << "\t" << "Draw" << "\t" << "Score";
	TextColor(15);
	int x = 5, y = 7;
	for (int i = 0; i < sizeStats; ++i) {
		if (i == 9) {
			x = 63;
			y = 7;
		}
		gotoXY(x, y);
		if (strlen(statsPlayer[i].name) < 5)
			cout << "   " << i + 1 << "  " << statsPlayer[i].name << "\t\t" << statsPlayer[i].thenumofWin << "\t" << statsPlayer[i].thenumofLose << "\t"
			<< statsPlayer[i].thenumofDraw << "\t" << statsPlayer[i].scoreRank;
		else cout << "   " << i + 1<< "  " << statsPlayer[i].name << "\t" << statsPlayer[i].thenumofWin << "\t" << statsPlayer[i].thenumofLose << "\t"
			<< statsPlayer[i].thenumofDraw << "\t" << statsPlayer[i].scoreRank;
		y += 2;
	}

	char key = -1;
	short choose = 2;
	bool check = true;

	while (check) {
		TextColor(15);
		DrawRectangle1(9, 25, 15, 1);
		gotoXY(10, 26);
		cout << "<< Back to Menu";
		DrawRectangle1(93, 25, 16, 1);
		gotoXY(94, 26);
		cout << "Reset Statistics";
		TextColor(3);
		switch (choose) {
		case 0:
			gotoXY(10, 26);
			cout << "<< Back to Menu";
			break;
		case 1:
			gotoXY(94, 26);
			cout << "Reset Statistics";
			break;
		}
		TextColor(15);
		if (_kbhit()) key = _getch();;
		switch (key) {
		case 75: 
			choose = 0;
			break;
		case 77:
			choose = 1;
			break;
		case 80: case 72:
			if (choose != 0 && choose != 1) choose = 0;
			break;
		case 13:
			if (choose == 0) {
				check = false;
				return;
			}
			else {
				if(sizeStats != 0){
					ResetStatistics(time); 
					if (time != 0) {
						return;
					}
				}		
			}
			break;
		}
		Sleep(50);
	}
}
//----------------------------------------------------------------
//Hàm đọc dữ liệu từ file statistics
void ReadStatistics() {
	ifstream f;
	f.open("statistics.txt", ios::in);
	if (!f) {
		return;
	}
	else {
		while (!f.eof()) {
			f >> sizeStats;
			for (int i = 0; i < sizeStats; ++i) {
				f.ignore();
				f.get(statsPlayer[i].name,14);
				fflush(stdin);
				f.ignore();
				f >> statsPlayer[i].thenumofWin;
				f >> statsPlayer[i].thenumofLose;
				f >> statsPlayer[i].thenumofDraw;
				f >> statsPlayer[i].scoreRank;
			}
		}
	}
	f.close();
	return;
}
//----------------------------------------------------------------
//Hàm so sánh hai chuỗi 
bool Compare(char* s1, char* s2) {
	if (strlen(s1) != strlen(s2)) return false;
	else {
		for (int i = 0; i < strlen(s1); ++i) {
			if (s1[i] != s2[i]) return false;
		}
		return true;
	}
}
//----------------------------------------------------------------
/// Hàm kiểm tra xem tài khoản người chơi đã tồn tại hay chưa
/// Nếu tồn tại thì ghi vào tk cũ, chưa thì thêm tk vào statistic
bool CheckStatistics(_Player player){
	if (sizeStats == 20) {
		return false;
	}
	int checkExist = false;
	for (int i = 0; i < sizeStats; ++i) {
		if (Compare(player.name, statsPlayer[i].name)) {
			statsPlayer[i].thenumofWin += player.thenumofWin;
			statsPlayer[i].thenumofLose += player.thenumofLose;
			statsPlayer[i].thenumofDraw += player.thenumofDraw;
			statsPlayer[i].scoreRank += player.scoreRank;
			return true;
		}
	}
	statsPlayer[sizeStats] = player;
	sizeStats++;
	return true;
}
//-----------------------------------------------------------------
//Hàm cập nhật statistic vào file statistics.txt
void UpdateStatistics() {
	ofstream f;
	f.open("statistics.txt", ios::out);
	f << sizeStats << endl;
	if(sizeStats != 0){
		f << statsPlayer[0].name << endl;
		f << statsPlayer[0].thenumofWin << " " << statsPlayer[0].thenumofLose << " " << statsPlayer[0].thenumofDraw << " " << statsPlayer[0].scoreRank;
	}
	for (int i = 1; i < sizeStats; ++i) {

		f << endl <<statsPlayer[i].name << endl;
		f <<statsPlayer[i].thenumofWin << " " << statsPlayer[i].thenumofLose << " " << statsPlayer[i].thenumofDraw << " " << statsPlayer[i].scoreRank;
	}
	f.close();
	return;
}
//-----------------------------------------------------------------
//Hàm reset lại Statistics
void ResetStatistics( short& time) {
	for (int i = 0; i < sizeStats; ++i) {
		statsPlayer[i] = statsPlayer[i + 1];
	}
	sizeStats = 0;
	UpdateStatistics();
	Statistics(++time);
	return;
}

//-----------------------------------------------------------------
//Hàm main
int main() {
	DisableResizeWindow();  // Vô hiệu hóa thay đổi kích thước Console
	DisableCtrButton(false, true, true); // Vô hiệu hóa các nút Minimize, Maximize
	ShowScrollbar(false);
	//PlaySound(TEXT("music.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
	_Menu Menu;
	SplashScreen();
	Update(Menu);

	_getch();
	return 0;
}