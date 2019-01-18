#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <iomanip>
#include <cstdlib>
#include <windows.h>
#include "Dictionary.h"
#include "Board.h"
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

//=======================================================================================================================
//creates a new board
bool newBoard(int &numOutFile)
{
	string dictionaryName;
	ifstream dictionaryFile;
	int ncol, nlines;
	setcolors(LIGHTCYAN);
	cout << endl;
	cout << "-------------------------------" << endl;
	cout << "             CREATE            " << endl;
	cout << "-------------------------------" << endl;
	setcolors(WHITE);
	//Opens the dictionary
	cout << "Please insert the dictionary file: ";
	if (cin.peek() != NULL)
		cin.ignore(100000, '\n');
	getline(cin, dictionaryName);
	dictionaryFile.open(dictionaryName);
	if (dictionaryFile.fail())
	{
		setcolors(RED, WHITE);
		cerr << "Input file opening failed." << endl << endl;;
		setcolors(WHITE, BLACK);
		return false;
	}
	Dictionary wDictionary;
	wDictionary.extractWords(dictionaryFile);
	//reads number of lines
	cout << endl << "Number of lines (maximum:26): ";
	cin >> nlines;
	while (!cin || (nlines <= 0) || (nlines > 26))
	{
		cin.clear();
		cin.ignore(100000, '\n');
		setcolors(RED, WHITE);
		cout << "Invalid Input" << endl;
		setcolors(WHITE, BLACK);
		cout<< "Number of lines (maximum:26): ";
		cin >> nlines;
	}
	//reads number of columns
	cout << endl << "Number of columns (maximum:26): ";
	cin >> ncol;
	while (!cin || (ncol<=0) || (ncol > 26))
	{
		cin.clear();
		cin.ignore(100000, '\n');
		setcolors(RED, WHITE);
		cout <<"Invalid Input" << endl;
		setcolors(WHITE, BLACK);
		cout<< "Number of columns (maximum:26): ";
		cin >> ncol;
	}
	//creates, updates and saves board
	Board cwBoard;
	cwBoard.Board::board(nlines, ncol);
	if (cwBoard.Board::updateBoard(wDictionary))
		cwBoard.saveBoard(dictionaryName, numOutFile);
	else numOutFile--;
	dictionaryFile.close();
	return true;
}

//=======================================================================================================================
//resumes an already existing board
bool resumeBoard(int &numOutFile)
{
	string nameFile, dictionaryName;
	ifstream inFile, dictionaryFile;
	setcolors(LIGHTCYAN);
	cout << "-------------------------" << endl;
	cout << "          RESUME         " << endl;
	cout << "-------------------------" << endl;
	setcolors(WHITE);
	//Input of the unfinished board file
	cout << "Where have you stored the crossword? ";
	if (cin.peek() != NULL)
		cin.ignore(1000000, '\n');
	getline(cin, nameFile);
	inFile.open(nameFile);
	if (inFile.fail())
	{
		setcolors(RED, WHITE);
		cerr << "Input file opening failed." << endl;
		setcolors(WHITE, BLACK);
		return false;
	}
	Board cwBoard;
	//opens the dictionary
	dictionaryName=cwBoard.Board::board(inFile);
	dictionaryFile.open(dictionaryName);
	if (dictionaryFile.fail())
	{
		setcolors(RED, WHITE);
		cerr << "Dictionary file opening failed." << endl << endl;;
		setcolors(WHITE, BLACK);
		return false;
	}
	Dictionary wDictionary;
	wDictionary.extractWords(dictionaryFile);
	//updates and saves the board
	if (cwBoard.Board::updateBoard(wDictionary))
		cwBoard.saveBoard(dictionaryName, numOutFile);
	else
		numOutFile--;
	inFile.close();
	dictionaryFile.close();
	return true;
}

//====================================================================================================================
int main()
{
	//Obtains the number of the previous board
	int numOutFile;
	fstream board_number_file;
	board_number_file.open("board_number.txt", std::fstream::in | std::fstream::out);
	if (board_number_file.peek() == ifstream::traits_type::eof())
		numOutFile = 1;
	else
	{
		string n_board;
		getline(board_number_file, n_board);
		numOutFile = (stoi(n_board)) % 1000;
	}
	board_number_file << 0;
	board_number_file.close();
	char option;
	ifstream dictionaryFile;
	setcolors(LIGHTCYAN);
	cout << "===================================" << endl;
	cout << "===  CROSSWORDS PUZZLE CREATOR  ==="<<endl;
	cout << "===================================" << endl;
	setcolors(WHITE);
	//MENU
	while (true)
	{
		setcolors(WHITE);
		cout << endl << "OPTIONS:" << endl;
		cout << "1 - Create puzzle" << endl;
		cout << "2 - Resume puzzle" << endl;
		cout << "0 - Exit" << endl << endl;
		cout << "Option? ";
		cin >> option;
		while (!cin || ((option != '1') && (option != '2') && (option != '0')))
		{
			cin.clear();
			cin.ignore(100000, '\n');
			setcolors(RED, WHITE);
			cout << endl << "Invalid Input" << endl;
			setcolors(WHITE, BLACK);
			cout<< "Option? ";
			cin >> option;
		}
		cout << endl;
		if (option == '1')
		{
			if (!newBoard(numOutFile))
				return 1;
		}
		else if (option == '2')
		{
			if (!resumeBoard(numOutFile))
				return 1;
		}
		else
			break;
		numOutFile++;
		numOutFile = numOutFile % 1000;
		setcolors(LIGHTCYAN);
		cout << "===============================" << endl << endl;
	}
	setcolors(LIGHTCYAN);
	cout << endl << "===============================" << endl << endl;
	cout << "Program developed by: " << endl;
	cout << "Leonor M. Sousa" << endl << "Silvia Rocha" << endl <<
		"Informatics and Computing Engineering Students in the Faculty of Engineering of the University of Porto"<< endl << endl;
	setcolors(WHITE);
	//changes board_number on file
	board_number_file.open("board_number.txt", std::fstream::out | std::fstream::trunc);
	board_number_file << numOutFile;
	board_number_file.close();
	return 0;
}