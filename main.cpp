// My first project in C++
// This is a connect four game
#include <windows.h>
#include <iostream>
//#include <vector>
#include "constant.h"
#include "agent.h"

using namespace constants;

class player {
	public:
		int playerID;
		char playerChess;
		int playerColor;
		player(int id = 0, char chess = 0, int color = 0)
			: playerID{ id }, playerChess{ chess }, playerColor{ color }{};

		int color_fill() { return playerColor + playerColor * 16; }					//  'playerColor' for text color, time 16 for background color,
};

// child class 'agent' with 'player' being the parent
class agent: public player {
	public:
		string name;
		agent(int id = 0, char chess = 0, int color = 0, string agent_name = "")
			: player{id, chess, color}, name{ agent_name }{
		};
		int askMove(char b[b_h][b_w], char enemyChess) {
			return minimax_ai(b, playerChess, enemyChess);

			//return random_ai(b, playerChess);
		}
};

// child class 'human' with 'player' being the parent
class human: public player {
	public:
		string name;
		human(int id = 0, char chess = 0, int color = 0, string human_name = "")
			: player{ id, chess, color }, name{ human_name }{
		};

		int askMove(char b[b_h][b_w], char enemyChess) {

			while (true) {
				cout << "Player " << playerID << "," << name << ", Enter column number for your next move: ";
				int c{};
				cin >> c;

				// Check for failed extraction
				if (cin.fail()) // has a previous extraction failed?
				{
					cin.clear();													// put us back in 'normal' operation mode
					cin.ignore(100, '\n');											// and remove the bad input
					cout << "Oops, that input is invalid.  Please try again.\n";
				}
				else if (c > b_w || c <= 0) {
					cin.clear();													// put us back in 'normal' operation mode
					cin.ignore(100, '\n');											// and remove the bad input
					cout << "Oops, column number must between 1 to " << b_w << ".  Please try again.\n";
				}
				else if (b[0][c - 1] != empty_chess) {
					cin.clear();													// put us back in 'normal' operation mode
					cin.ignore(100, '\n');											// and remove the bad input
					cout << "Oops, column is full already. Please try again.\n";
				}
				else
				{
					cin.ignore(100, '\n'); // remove any extraneous input
					return c - 1;
				}
			}
		}

};

/* section of code used for drawing a simple board, not used as i made a nicer looking board
void drawSeparationLine() { 
	cout << "\n" << string(20, '-') << "\n"; 
}


// draw a simple board
void drawBoard(char b[b_h][b_w]) {
	
	drawSeparationLine();

	for (int i = 0; i < b_h; i++) {									// loop to print board
		for (int j = 0; j < b_w; j++) { cout << b[i][j]; }
		cout << "\n";
	}
	drawSeparationLine();
}
*/

void drawChess(char (&d)[display_height][display_width], int a, int b, char chess) {
	for (int x = 0; x < chessX; x++) {
		for (int y = 0; y < chessY; y++) {
			d[b + y][a + x] = chess;
		}
	}
}
void drawNiceBoard(char b[b_h][b_w], HANDLE hConsole, player p1, player p2) {
	char d[display_height][display_width];			// the board to be printed
	memset(&d, ' ', sizeof(d));						// init to space char

	// set grid
	int lineSepX = squareX + 1;
	int lineSepY = squareY + 1;
	for (int i = 0; i < display_height; i++) {
		for (int j = 0; j < display_width; j++) {
			if (i % lineSepY == 0 && j % lineSepX == 0) {
				d[i][j] = '+';								// '+' for intersections of grid lines
			}else if (i % lineSepY == 0) {
				d[i][j] = '-';								// '-' for x dir
			}else if (j % lineSepX == 0) {
				d[i][j] = '|';								// '|' for y dir
			}
		}
	}

	// set chess
	for (int i = 0; i < b_h; i++) {
		for (int j = 0; j < b_w; j++) { 
			char curr_chess = b[i][j];
			if (curr_chess != empty_chess) {
				int a = j * (squareX + 1) + 2;
				int b = i * (squareY + 1) + 2;
				drawChess(d, a, b, curr_chess);
			}
		}
	}

	// loop to print board
	cout << "\n";
	for (int i = 0; i < display_height; i++) {
		for (int j = 0; j < display_width; j++) { 
			if (d[i][j] == p1.playerChess) {
				SetConsoleTextAttribute(hConsole, p1.color_fill());
				cout << d[i][j];
				SetConsoleTextAttribute(hConsole, default_color);
			}else if (d[i][j] == p2.playerChess) {
				SetConsoleTextAttribute(hConsole, p2.color_fill());
				cout << d[i][j];
				SetConsoleTextAttribute(hConsole, default_color);
			}
			else {
				cout << d[i][j];
			}
			
		}
		cout << "\n";
	}
}

void updateBoard(char (&b)[b_h][b_w],int col, char chess) {
	// check the first empty space in the column
	for (int i = b_h - 1; i >= 0; i--) {
		if (b[i][col] == empty_chess) {
			b[i][col] = chess;
			break;
		}
	}
}

int checkBoard(char b[b_h][b_w], char chess) {
	// check win conditions on board
	// return 0 if win, 1 if condition not met

	// horizontal check 4 connections
	// looping all the rows, start from the bottom so it is faster
	for (int rowNo = b_h - 1; rowNo >= 0; rowNo--) {
		int count{ 0 };
		for (int colNo = 0; colNo < b_w; colNo++) {
			(b[rowNo][colNo] == chess) ? count++: count = 0;				// count++ if match, reset counter if not match
			if (count == 4) { return 0; }								// if 4 connected, return win message
		}
	}
	// vertical check 4 connections
	// Same code as above, but changed axis
	for (int colNo = 0; colNo < b_w; colNo++) {
		int count{ 0 };
		for (int rowNo = b_h - 1; rowNo >= 0; rowNo--) {
			(b[rowNo][colNo] == chess) ? count++ : count = 0;			// count++ if match, reset counter if not match
			if (count == 4) { return 0; }								// if 4 connected, return win message
		}
	}

	// diagonal check
	// going up and right
	// loop on first column
	for (int rowNo = 0; rowNo < b_h; rowNo++) {
		int count{ 0 };
		int x{ 0 };
		int y{ 0 };
		while (x >= 0 && x < b_w && y >= 0 && y < b_h) {		// make sure it doesnt add out of bound
			(b[rowNo + y][x] == chess) ? count++ : count = 0;
			if (count == 4) { return 0; }
			x++;
			y++;
		}
	}

	// loop on top row
	for (int colNo = 0; colNo < b_w; colNo++) {
		int count{ 0 };
		int x{ 0 };
		int y{ 0 };
		while (x >= 0 && x < b_w && y >= 0 && y < b_h) {		// make sure it doesnt add out of bound
			(b[y][colNo + x] == chess) ? count++ : count = 0;
			if (count == 4) { return 0; }
			x++;
			y++;
		}
	}

	// going down and right

	// loop on first column
	for (int rowNo = 0; rowNo < b_h; rowNo++) {
		int count{ 0 };
		int x{ 0 };
		int y{ 0 };
		while (x >= 0 && x < b_w && y >= 0 && y < b_h) {		// make sure it doesnt add out of bound
			(b[rowNo + y][x] == chess) ? count++ : count = 0;
			if (count == 4) { return 0; }
			x++;
			y--;
		}
	}

	// loop on last row
	for (int colNo = 0; colNo < b_w; colNo++) {
		int count{ 0 };
		int x{ 0 };
		int y{ b_h - 1 };
		while (x >= 0 && x < b_w && y >= 0 && y < b_h) {		// make sure it doesnt add out of bound
			(b[y][colNo + x] == chess) ? count++ : count = 0;
			if (count == 4) { return 0; }
			x++;
			y--;
		}
	}
	return 1;
}

int main(){

	cout << welcome_msg;
	cout << "Welcome, this is the game of Connect Four!!\n";

	// initialize board with empty_chess
	char board[::b_h][::b_w];
	memset(&board, empty_chess, sizeof(board));

	// initalize player properties

	agent p1 = agent(1, 'O', red, "random");
	human p2 = human(2, 'X', yellow, "Human");

	player* curr_player = &p1;

	int move{0};
	int chess_char{};

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	
	while (true) {
		cout << "Move " << move << ":";
		drawNiceBoard(board, hConsole, p1, p2);
		if (move % 2 == 0) {
			curr_player = &p1;
			updateBoard(board, p1.askMove(board, p2.playerChess), p1.playerChess);
		}
		else {
			curr_player = &p2;
			updateBoard(board, p2.askMove(board, p1.playerChess), p2.playerChess);
		}
		move++;

		// if win condition is fullfilled, print winning message and break
		if (checkBoard(board, curr_player->playerChess) == 0) {
			drawNiceBoard(board, hConsole, p1, p2);
			SetConsoleTextAttribute(hConsole, curr_player->playerColor);
			cout << win_msg;
			SetConsoleTextAttribute(hConsole, default_color);
			break; 
		}

		// draw if the board is full
		if (move == b_h * b_w) {
			drawNiceBoard(board, hConsole, p1, p2);
			SetConsoleTextAttribute(hConsole, curr_player->playerColor);
			cout << draw_msg;
			SetConsoleTextAttribute(hConsole, default_color);
			break;
		}
	}
	
	return 0;
}