#include<stdio.h>
#include<mutex>
#include<windows.h>
#include<conio.h>
#include<stdbool.h>
#include<string.h>
#include<stdlib.h>
#include<thread>
#include<ctime>
#pragma warning(disable : 4996)
#define screen_x 100
#define screen_y 63
HANDLE wHnd;
COORD bufferSize = { screen_x,screen_y };
SMALL_RECT windowSize = { 0,0,screen_x - 1,screen_y - 1 };
std::mutex mtx;

void setWinPos(void) {
	HWND consoleWindow = GetConsoleWindow();
	SetWindowPos(consoleWindow, 0, 1000, 0, 1000, 0, SWP_NOSIZE | SWP_NOZORDER);
}
int setConsole(int x, int y)
{
	wHnd = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleWindowInfo(wHnd, TRUE, &windowSize);
	SetConsoleScreenBufferSize(wHnd, bufferSize);
	return 0;
}
void setcolor(int fg, int bg)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, bg * 16 + fg);
}
void gotoxy(int x, int y)
{
	COORD c = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}
void setcursor(bool visible)
{
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO lpCursor;
	lpCursor.bVisible = visible;
	lpCursor.dwSize = 20;
	SetConsoleCursorInfo(console, &lpCursor);
}
char cursor(int x, int y) {
	HANDLE hStd = GetStdHandle(STD_OUTPUT_HANDLE);
	char buf[2]; COORD c = { x,y }; DWORD num_read;
	if (
		!ReadConsoleOutputCharacter(hStd, (LPTSTR)buf, 1, c, (LPDWORD)&num_read))

		return '\0';
	else
		return buf[0];

}
int random_between(int min, int max)
{
	int r = (rand() / (float)RAND_MAX) * (max + 1) + min;
	return r > max ? max : r;
}
void boundary(int x, int y) {

	int i, j;
	for (i = 1; i <= y; i++) {
		for (j = 1; j <= x; j++) {
			if (i == 1 || j == 1 || i == y || j == x) {
				setcolor(5, 3);
				printf("*");
			}
			else {
				setcolor(1, 0);
				printf(" ");
			}
		}

		printf("\n");
	}
}
void coord(int x, int y) {
	setcolor(14, 0);
	gotoxy(82, 22);
	printf("X : %d", x);
	gotoxy(82, 23);
	printf("Y : %d", y);
}
void showlinenum(void) {
	setcolor(4, 2);
	for (int i = 0; i < 59; i++) {
		gotoxy(76, i);
		printf("%d", i);
	}
	setcolor(3, 5);
	for (int j = 0; j <= 74; j++) {
		gotoxy(j, 62);
		setcolor(3, 5);
		printf("%d", j % 10);
		if (j == 10 || j == 20 || j == 30 || j == 40 || j == 50 || j == 60 || j == 70) {
			setcolor(15, 9);
			gotoxy(j, 61);
			printf("%d", j/10);
		}
	}
}
void draw_human(int x, int y) {
	setcolor(6, 0);
	gotoxy(x, y);
	printf(" O");
	gotoxy(x, y + 1);
	printf("/|\\");
	gotoxy(x, y + 2);
	printf(" | ");
	gotoxy(x, y + 3);
	printf("/ \\");
}
void delete_human(int x, int y) {
	setcolor(6, 0);
	gotoxy(x, y);
	printf("  ");
	gotoxy(x, y + 1);
	printf("   ");
	gotoxy(x, y + 2);
	printf("  ");
	gotoxy(x, y + 3);
	printf("   ");
}
void delete_human_parachute(int x, int y) {
	x = x - 5;
	y = y - 2;
	for (int i = 0; i <= 6; i++) {
		gotoxy(x, y + i);
		printf("            ");
	}
	/*gotoxy(x, ++y);
	printf("           ");
	gotoxy(x, ++y);
	printf("            ");
	gotoxy(x, ++y);
	printf("            ");
	gotoxy(x, ++y);
	printf("         ");
	gotoxy(x, ++y);
	printf("         ");
	gotoxy(x, ++y);
	printf("         ");*/
}
void draw_human_parachute(int x, int y) {
	x = x - 5;
	y = y - 2;
	setcolor(3, 0);
	gotoxy(x, y);
	printf("    _____ ");
	gotoxy(x, ++y);
	printf("   /|   |\\");
	gotoxy(x, ++y);
	printf("  / |   | \\");
	gotoxy(x, ++y);
	printf(" /  | ");
	setcolor(6, 0);
	printf("O");
	setcolor(3, 0);
	printf(" |"); printf("  \\");
	setcolor(6, 0);
	gotoxy(x, ++y);
	printf("     /|\\");
	gotoxy(x, ++y);
	printf("      |");
	gotoxy(x, ++y);
	printf("     / \\");
}
void draw_bird(int x, int y) {
	mtx.lock();
	gotoxy(x, y);
	printf("\\|/");
	Sleep(100);
	gotoxy(x, y);
	printf("/|\\");
	Sleep(100);
	mtx.unlock();

}
void delete_bird(int x, int y) {
	mtx.lock();
	gotoxy(x, y);
	printf("   ");
	mtx.unlock();
}
int Home_menu() {
	int sel;
	char choice;
	setcolor(14, 0);
	system("cls");
	printf("\n_______________________________\n");
	printf("|          wELCOMe tO          |\n");
	printf("|      Parachute GAMEE!!!      |\n");
	printf("|______________________________|\n\tselect your MENU\n");
	printf("\t1.Play game\n");
	setcolor(8, 0);
	printf("\t[unavaliable]2.Leaderboard\n");
	setcolor(14, 0);
	printf("\t3.Credit\n");
	printf("\t4.Exit\n");
	scanf_s("%d", &sel);
	//choice = getche();
	return sel;
	//return choice;
}
void showtime(double x) {
	gotoxy(80, 30);
    printf("time : 00:00:%.0lf",x);
}
void FontSize(int W, int H) {
	int newWidth = W, newHeight = H;
	CONSOLE_FONT_INFOEX fontStructure = { 0 };
	fontStructure.cbSize = sizeof(fontStructure);
	fontStructure.dwFontSize.X = newWidth;
	fontStructure.dwFontSize.Y = newHeight;
	wcscpy(fontStructure.FaceName, L"Arial");
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetCurrentConsoleFontEx(hConsole, true, &fontStructure);	
}
int calscore(int t, int c) {
	int score = 0;
	if (t < 8) {
		score = 800;
	}
	else if (8 <= t && t <= 16) {
		score = 1600 - (100 * t);
	}
	else
		score = 0;
	if (c > 5) {
		score = score - (50 * c);
	}
	else
		score = score - (10 * c);
	return score;
}
void loading() {
	Sleep(500);
	system("cls");
	for (int i = 0; i <= 3; i++) {
		printf("loading");
		for (int j = 0; j <= 3; j++) {
			printf(".");
			Sleep(100);
		}
		system("cls");
	}
}
void showcol(int c) {
	gotoxy(80, 35);
	setcolor(7, 4);
	printf("collissioncount : %d", c);
}




int main() {
	time_t start, end, now;
	int select, score = 0, collissioncount = 0;
	char name[50];
	setWinPos();
	setConsole(30, 30);
	select = Home_menu();
	system("cls");
	if (select == 1) {
		printf("Enter your name :");
		scanf("%s", name);
		printf("OK");
		setcolor(10, 0);
		printf(" %s ", name);
		setcolor(14, 0);
		printf("let's go into the game\nPlease wait...");
		loading();
		system("cls");
		time(&start);
		int x = 30, y = 5, i, select, bird_x[8], bird_y[8], t;
		char ch = '.';
		/*
		*parashute status
		* game status
		 */

		boolean Pstatus = 0, life = 1, game = 1;


		
		setConsole(screen_x, screen_y);
		srand(time(NULL));
		for (i = 0; i < 8; i++) {
			bird_x[i] = random_between(2, 65);
			bird_y[i] = random_between(40,56);
		}
		boundary(75, 58);
		showlinenum();
		draw_human(x, y);
		coord(x, y);
		showcol(collissioncount);
		do {
			for (i = 0; i < 8; i++) {
				if (bird_y[i] > 1) {
					
					setcolor(7, 0);
					std::thread q(delete_bird, bird_x[i], bird_y[i]);
					q.detach();					
					setcolor(11, 0);
					std::thread w(draw_bird, bird_x[i], --bird_y[i]);
					w.detach();
					Sleep(10);
					
				}
			}

			if (_kbhit()) {
				ch = _getch();
				if (ch == ' ' && Pstatus == 0 && y > 4 && y < 53 && x < 39 && x > 1) {
					Pstatus = !Pstatus;
					delete_human_parachute(x, y);
					draw_human_parachute(x, y += 1);
				}
				else if (ch == 'a' && x > 1 && Pstatus == 0 && y < 53) {//y<53 is can't move after touch down
					delete_human(x, y);
					draw_human(--x, y);
				}
				else if (ch == 'd' && x < 73 && Pstatus == 0 && y < 53) {//y<53 is can't move after touch down
					delete_human(x, y);
					draw_human(++x, y);
				}
				else if (ch == 's' && y < 53 && Pstatus == 0) {
					delete_human(x, y);
					draw_human(x, ++y);
				}
				else if (ch == 'w' && y > 1 && Pstatus == 0 && y < 53) {//y<53 is can't move after touch down
					delete_human(x, y);
					draw_human(x, --y);
				}
				else if (ch == 'a' && x > 1 && Pstatus == 1 && y < 53) {//y<53 is can't move after touch down
					delete_human_parachute(x, y);
					draw_human_parachute(--x, y);
				}
				else if (ch == 'd' && x < 73 && Pstatus == 1 && y < 53) {//y<53 is can't move after touch down
					delete_human_parachute(x, y);
					draw_human_parachute(++x, y);
				}
				else if (ch == 's' && y < 53 && Pstatus == 1) {
					delete_human_parachute(x, y);
					draw_human_parachute(x, ++y);
				}
				else if (ch == 'w' && y > 1 && Pstatus == 1 && y < 50) {//y<53 is can't move after touch down
					delete_human_parachute(x, y);
					draw_human_parachute(x, --y);
				}
				fflush(stdin);
			}
			if (Pstatus == 1 && 0 < y && y <= 50) {
				delete_human_parachute(x, y);
				draw_human_parachute(x, y += 2);
				Sleep(800);
				for (i = 3; i <= 5; i++) {
					if (cursor(x + i, y + 7) != ' ') {
						collissioncount++;
						showcol(collissioncount);
					}
				}
				if (y >= 51) {
					delete_human_parachute(x, y);
					y = 53;
					draw_human(x, y);
					game = 0;
					//Pstatus = 0;
				}
			}
			else if (y < 53 && Pstatus == 0) {
				delete_human(x, y);
				draw_human(x, y += 3);
				Sleep(100);
			}
			if (y == 53 && Pstatus == 0) {
				life = 0;
				game = 0;
			}
			/*std::thread a(showtime, time(NULL) - start);
			a.detach();*/
			showtime(time(NULL) - start);
			coord(x, y);
			Sleep(100);
		} while ((ch != 'x') && (life == 1) && (game == 1));
		time(&end);
		/*
		*  t is time player used and will calculate for score
		*/
		t = end - start;
		score=calscore(t,collissioncount);

		if (life == 0) {
			system("cls");
			for (int k = 25; k <= 35; k++) {
				gotoxy(0, k);
				setcolor(7, 4);
				for (int i = 0; i <= 100; i++) {
					printf(" ");
				}
				//printf("\n");
			}
			gotoxy(40, 30);
			setcolor(9, 4);
			//FontSize(10, 50);
			printf("%s DIED with score : %d",name,score);
			goto Ex;
		}
		else if (life == 1 && game == 0) {
			system("cls");
			for (int k = 25; k <= 35; k++) {
				gotoxy(0, k);
				setcolor(1, 2);
				for (int i = 0; i <= 100; i++) {
					printf(" ");
				}
				//printf("\n");
			}
			gotoxy(35, 29);
			printf("%s took %d second to end the game", name, t);
			gotoxy(40, 30);
			printf("%s's score : %d", name, score);
			goto Ex;
		}
	}
	else if (select == 3) {
	B:		char ch;
		gotoxy(0, 10);
		setcolor(5, 11);
		for (int i = 0; i <= 120; i++) { printf(" "); }
		printf("This game created and modified by Sawat Lapprasertlum");
		for (int i = 0; i <= 27; i++) { printf(" "); }
		for (int i = 0; i <= 42; i++) { printf(" "); }
		gotoxy(40, 12);
		printf("Student ID : 61011085");
		for (int i = 0; i <= 38; i++) { printf(" "); }
		gotoxy(40, 15);
	A:		printf("return to Menu [y,n] :");
		ch = getch();
		if (ch == 'y' || ch == 'Y') {
			main();
		}
		else if (ch == 'n' || ch == 'N') {
			
			goto B;
		}
		else {
			setcolor(6, 4);
			printf("\nIncorrect try agian\n");
			goto A;
		}
	}
	else if (select == 4) {
		goto Ex;
	}
	else {
	gotoxy(45, 10);
		setcolor(6, 4);
		printf("Incorrect choice");
		gotoxy(45, 11);
		printf("please try again");
		Sleep(1500);
		main();
	}

Ex:	setcolor(10, 0);
	printf("\n\n\n\n\n\n\n");
	return 0;
	//system("pause");
}
