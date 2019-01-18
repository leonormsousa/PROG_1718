#include <iostream>
#include <fstream>
#include<ctime>
#include <cstdlib>
#include <windows.h>
#include "Player.h"
#include "Board.h"
#include "Dictionary.h"
using namespace std;

//=======================================================================================================================
//COLOR CODES: (alternative: use symbolic const’s)
#define BLACK 0
#define BLUE 1
#define GREEN 2
#define CYAN 3
#define RED 4
#define MAGENTA 5
#define BROWN 6
#define LIGHTGRAY 7
#define DARKGRAY 8
#define LIGHTBLUE 9
#define LIGHTGREEN 10
#define LIGHTCYAN 11
#define LIGHTRED 12
#define LIGHTMAGENTA 13
#define YELLOW 14
#define WHITE 15

//=======================================================================================================================
// Set text color
void setcolors(unsigned int color)
{
	HANDLE hcon = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hcon, color);
}

//=======================================================================================================================
// Set text color & background
void setcolors(unsigned int color, unsigned int background_color)
{
	HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
	if (background_color == BLACK)
		SetConsoleTextAttribute(hCon, color);
	else
		SetConsoleTextAttribute(hCon, color | BACKGROUND_BLUE | BACKGROUND_GREEN |
			BACKGROUND_RED);
}

//============================================================================================================================
//creates a player and allows him to try to solve the board 
void playerSolve(Board solutionBoard, string outFileName, Dictionary wDictionary)
{
	Board playerBoard = solutionBoard;
	playerBoard.emptyBoard();
	//saves the player's name
	string name;
	cout << endl << "What's your name (maximum: 10 letters)?";
	cin >> name;
	while (!cin || name.length()>10)
	{
		cin.clear();
		cin.ignore(100000, '\n');
		setcolors(RED, WHITE);
		cout << "Invalid Input." << endl;
		setcolors(WHITE, BLACK);
		cout<< "What's your name? ";
		cin >> name;
	}
	cout << endl;
	Player cwPlayer;
	cwPlayer.player(name);
	//the user changes the board
	playerBoard.updateBoard(wDictionary, solutionBoard, cwPlayer);
	//sees if the player was abble to solve the board
	if (playerBoard.equalBoards(solutionBoard))
	{
		cwPlayer.finishTry();
		cwPlayer.savePlayer(outFileName);
		cout << "Congratulations! You've correctly solved the crossword's puzzle!" << endl <<
			"Your data was saved in the file " << outFileName << endl << endl;
	}
	else
		cout << "Sorry! That was not the correct solution to the crossword's puzzle." << endl << endl;
}

//============================================================================================================================
//Opens a specific board and presents a menu to that board
bool solvePuzzle()
{
	//LOADS THE BOARD
	string boardName;
	ifstream boardFile;
	cout << "Please insert the board file: ";
	if (cin.peek() != NULL)
		cin.ignore(100000, '\n');
	getline(cin, boardName);
	boardFile.open(boardName);
	if (boardFile.fail())
	{
		setcolors(RED, WHITE);
		cerr << "Input file opening failed." << endl << endl;;
		return false;
	}
	//Creates the dicitionary
	string dictionaryName;
	getline(boardFile, dictionaryName);
	ifstream dictionaryFile;
	Dictionary wDictionary;
	dictionaryFile.open(dictionaryName);
	if (dictionaryFile.fail())
	{
		setcolors(RED, WHITE);
		cerr << "Dictionary file opening failed." << 
			"Please check if the path to the dictionary file is correctly saved in the board file." << endl << endl;;
		return false;
	}
	wDictionary.extractWords(dictionaryFile);
	//creates/opens the player's file
	int size = int(boardName.length())-7;
	string nBoard = boardName.substr(size,3);
	Board solutionBoard;
	solutionBoard.board(boardFile);
	string nameOutFile = "b";
	ofstream outFile;
	nameOutFile.append(nBoard);
	nameOutFile.append("_p.txt");
	//puts the current time and date in player's file
	struct tm ltm;
	time_t now = time(0);
	localtime_s(&ltm, &now);
	int year = 1900 + (ltm.tm_year);
	int month = 1 + (ltm.tm_mon);
	int day = (ltm.tm_mday);
	int hour = (ltm.tm_hour);
	int min = (ltm.tm_min);
	int sec = (ltm.tm_sec);
	outFile.open(nameOutFile, ofstream::out | ofstream::app);
	outFile << endl << "          " << setw(2) << hour << setw(1) << 'h'<< setw(2) << min <<setw(1)<< 'm' <<
		setw(2) << sec << setw(9) << "s   ->   " << setw(2) << day << setw(1) << '/' << setw(2) << month <<
		setw(1) << '/' << setw(4) << year;
	outFile << endl<< setw(10) << "Name" << setw(14) << "Time spent" << setw(15) << "Clues spent" << endl;
	outFile.close();
	//MENU do board
	while (true)
	{
		char option;
		setcolors(LIGHTCYAN);
		cout << endl << "BOARD " << nBoard;
		cout << endl << "=========================" << endl << endl;
		setcolors(WHITE);
		cout << endl << "OPTIONS:" << endl;
		cout << "1 - Start attempt" << endl;
		cout << "0 - I'm done with this puzzle. I want to solve a different one." << endl << endl;
		cout << "Option? ";
		cin >> option;
		while (!cin || ((option != '1') && (option != '0')))
		{
			cin.clear();
			cin.ignore(100000, '\n');
			setcolors(RED, WHITE);
			cout << endl << "Invalid Input" << endl;
			setcolors(WHITE, BLACK);
			cout<< "Option? ";
			cin >> option;
		}
		if (option == '0')
			break;
		playerSolve(solutionBoard, nameOutFile, wDictionary);
	}
	//presents the solution to the player
	cout << endl << "Here is the solution: " << endl << endl;
	solutionBoard.showBoard();
	cout << endl;
	dictionaryFile.close();
	return true;
}

//=============================================================================================================================
int main()
{
	setcolors(LIGHTCYAN);
	cout << "===========================" << endl;
	cout << "===  CROSSWORDS PLAYER  ===" << endl;
	cout << "===========================" << endl << endl;
	//MAIN MENU
	char option;
	while (true)
	{
		setcolors(WHITE);
		cout << endl << "OPTIONS:" << endl;
		cout << "1 - Choose Puzzle" << endl;
		cout << "0 - Exit" << endl << endl;
		cout << "Option? ";
		cin >> option;
		while (!cin || ((option != '1') && (option != '0')))
		{
			cin.clear();
			cin.ignore(100000, '\n');
			setcolors(RED, WHITE);
			cout << endl << "Invalid Input" << endl;
			setcolors(WHITE, BLACK);
			cout<< "Option? ";
			cin >> option;
		}
		if (option == '0')
			break;
		if (!solvePuzzle())
			return 1;
		setcolors(LIGHTCYAN);
		cout << endl << "==========================" << endl << endl;
	}
	setcolors(LIGHTCYAN);
	cout << endl << "===========================" << endl << endl;
	cout << "Program developed by: " << endl;
	cout << "Leonor M. Sousa" << endl << "Silvia Rocha" << endl <<
		"Informatics and Computing Engineering Students in the Faculty of Engineering of the University of Porto" << endl << endl;
	setcolors(WHITE);
	return 0;
}