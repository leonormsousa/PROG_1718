#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <iomanip>
#include <sstream>
#include <ctime>
#include <cstdlib>
#include <windows.h>
#include "Board.h"
#include "Player.h"
using namespace std;

//==========================================================================================================================
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

//========================================================================================================================
// Set text color
void setcolor(unsigned int color)
{
	HANDLE hcon = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hcon, color);
}

//========================================================================================================================
// Set text color & background
void setcolor(unsigned int color, unsigned int background_color)
{
	HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
	if (background_color == BLACK)
		SetConsoleTextAttribute(hCon, color);
	else
		SetConsoleTextAttribute(hCon, color | BACKGROUND_BLUE | BACKGROUND_GREEN |
			BACKGROUND_RED);
}

// ====================================================================================================================== =
//puts the all word given in uppercases
void Board::toUpperWord(string &word)
{
	for (size_t i = 0; i < word.size(); i++)
	{
		word[i] = toupper(word[i]);
	}
}

//========================================================================================================================
//Constructor -> creates a board from a file that already contains a previoulsy started board
void Board::board(ifstream& inFile)
{
	int lineBoard = 0, ncol = 0, nlines = 0;
	string line;
	vector<vector<char> > vf;
	getline(inFile, line);
	getline(inFile, line);
	while (line != "")
	{
		nlines++;
		vector<char> v;
		for (size_t i = 0; i < line.length(); i++)
			v.push_back(line[i]);
		vf.push_back(v);
		ncol = int (v.size());
		getline(inFile, line);
	}
	cwboard = vf;
	numCol = ncol;
	numLines = nlines;
	while (getline(inFile, line) && (line != ""))
	{
		cwWords[line.substr(0, 3)] = line.substr(4);
	}
}

// =======================================================================================================================
//allows to change the board adding several words 
//returns true if it is to save the board
void Board::updateBoard(Dictionary wDictionary, Board board2, Player &cwPlayer)
{
	cout << endl << "Instructions: " << endl;
	cout << "The position must be inserted in the format LCD (Line-Column-Direction) and it must be the starting position of a word.";
	cout << endl << "Example: AaV -> Line A, Column a, Vertical" << endl;
	cout << "If you wish to remove a word, enter the symbol '-' when asked for the word." << endl;
	cout << "If you wish to remove a cell, enter the symbol '_' when asked for the word." << endl;
	cout << "If you wish to add just a letter, enter the symbol '+' when asked for the word." << endl;
	cout << "If you need a hint, enter the symbol '?' when asked for the word." << endl;
	cout << "If you wish to clear the board, enter the symbol '!' when asked for the word." << endl;
	cout << "Warning: You shouln't use '_' or '+' unless it is really necessary. These operations may cause some errors and you might have to clear the board." << endl;
	cout << "When you have finished, please press CTRL-Z." << endl << endl;
	string lcd;
	string word;
	bool stop = false;
	//while the player doesn't press ctrl-Z
	vector<string> verticals, horizontals, vhints, hhints, vlcd, hlcd;
	while (true)
	{
		Board::showBoard();
		//PRINTS THE  HINTS ON THE SCREEN
		for (map<string, string>::iterator l = board2.cwWords.begin(); l != board2.cwWords.end(); l++)
		{
			string	lcd = l->first;
			if (lcd[2] == 'V')
			{
				vlcd.push_back(lcd);
				verticals.push_back(board2.cwWords[lcd]);
			}
			else
			{
				hlcd.push_back(lcd);
				horizontals.push_back(board2.cwWords[lcd]);
			}
		}
		//prints the vertical words hints
		setcolor(LIGHTCYAN);
		cout << endl<< endl << "Vertical words: " << endl;
		setcolor(WHITE);
		if (!(vhints.empty()))
		{
			for (unsigned int k = 0; k < vhints.size(); k++)
			{
				string vlc = vlcd[k];
				cout << vlc[0] << vlc[1] << ". " << vhints[k] << endl;
			}
				
		}
		else
		{
			for (unsigned int i = 0; i < verticals.size(); i++)
			{
				string vlc = vlcd[i];
				string hint = wDictionary.Hint(verticals[i]);
				vhints.push_back(hint);
				cout << vlc[0]<<vlc[1] << ". " << hint << endl;
			}
		}
		setcolor(LIGHTCYAN);
		//prints the horizontal words hints
		cout << endl << "Horizontal words: " << endl;
		setcolor(WHITE);
		if (!(hhints.empty()))
		{
			for (unsigned int k = 0; k < hhints.size(); k++)
			{
				string hlc = hlcd[k];
				cout << hlc[0] << hlc[1] << ". " << hhints[k] << endl;
			}
		}
		else
		{
			for (unsigned int i = 0; i < horizontals.size(); i++)
			{
				string hlc = hlcd[i];
				string hint = wDictionary.Hint(horizontals[i]);
				hhints.push_back(hint);
				cout << hlc[0] << hlc[1] << ". " << wDictionary.Hint(horizontals[i]) << endl;
			}
		}
		//INPUT OF LCD
		cout << endl << "Position (LCD / CTRL-Z = stop)? ";
		cin >> lcd;
		while (!cin || (cin.eof()) || lcd.length() != 3 || ((lcd[2] != 'V') && (lcd[2] != 'H') && (lcd[2] != 'v') && (lcd[2] != 'h')) ||
			(toupper(lcd[0]) < 'A') || (toupper(lcd[0]) >= ('A' + numLines)) || (toupper(lcd[1]) < 'A') || (toupper(lcd[1]) >= ('A' + numCol)))
		{
			if (cin.eof())
			{
				stop = true;
				cin.clear();
				break;
			}
			cin.clear();
			cin.ignore(100000, '\n');
			setcolor(RED, WHITE);
			cout << "Invalid Input." << endl;
			setcolor(WHITE, BLACK);
			cout<< "Position (LCD / CTRL-Z = stop)? ";
			cin >> lcd;	
		}
		if (stop)
			break;
		lcd[0] = toupper(lcd[0]);
		lcd[1] = toupper(lcd[1]) - 'A' + 'a';
		lcd[2] = toupper(lcd[2]);
		//INPUT OF THE WORD
		cout << "Word? ";
		cin >> word;
		//GIVES AN HINT
		if (word == "?")
		{
			map<string, string> words = board2.cwWords;
			if (words.find(lcd) == words.end())
				cout << "That is not a starting position of a word." << endl;
			else
			{
				showHint(lcd, wDictionary, board2);
				cwPlayer.incrementClues();
			}
		}
		//REMOVES THE WORD
		else if (word == "-")
		{
			if (!removeWord(lcd))
				cout << endl << "There is no word starting in that position. (Verify if you didn't add the word using '+'.)";
			cout << endl << endl;
		}
		//REMOVES THE CELL
		else if (word == "_")
		{
			removeCell(lcd);
			cout << endl << endl;
		}
		//ADDS A CELL
		else if (word == "+")
		{
			addCell(lcd);
			cout << endl << endl;
		}
		//CLEANS THE BOARD
		else if (word == "!")
		{
			emptyBoard();
			cout << endl << endl;
		}
		//ADDS A WORD
		else
		{
			toUpperWord(word);
			if (verifyWord(word, lcd))
			{
				addWord(word, lcd);
				cout << endl << endl;
			}
			else
				cout << "The word " << word << " can't be placed in there." << endl << endl;
		}
	}
}

// =======================================================================================================================
//prints the board on the screen
void Board::showBoard()
{
	cout << "  ";
	setcolor(LIGHTCYAN);
	for (int i = 0; i < numCol; i++)
		cout << setw(2) << char('a' + i);
	for (int i = 0; i < numLines; i++)
	{
		setcolor(LIGHTCYAN);
		cout << endl <<char('A' + i) << " ";
		setcolor(BLACK, LIGHTGRAY);
		for (int j = 0; j < numCol; j++)
		{
			if (cwboard[i][j] == '#')
			{
				cout << " ";
				setcolor(LIGHTGRAY, BLACK);
				cout << cwboard[i][j];
				setcolor(BLACK, LIGHTGRAY);
			}
			else
				cout << " " << cwboard[i][j];
		}
		cout << " ";
		setcolor(WHITE, BLACK);
	}
}

// =======================================================================================================================
//adds a given word on a certain location (lcd)
void Board::addWord(string word, string lcd)
{
	if (lcd[2] == 'V')
	{
		for (size_t i = 0; i < word.length(); i++)
			cwboard[(lcd[0] - 'A') + i][lcd[1] - 'a'] = word[i];
	}
	else
	{
		for (size_t i = 0; i < word.length(); i++)
			cwboard[lcd[0] - 'A'][(lcd[1] - 'a') + i] = word[i];
	}
	cwWords[lcd] = word;
}

// =======================================================================================================================
//returns true if there is space to place the word on the board
bool Board::verifyWord(string word, string lcd)
{
	if ((lcd[2] == 'V') && (int(word.length()) - 'A' + lcd[0] > numLines))
		return false;
	if (changesBoard(word, lcd[0], toupper(lcd[1]), lcd[2]))
		return false;
	else
		return true;
}

// =======================================================================================================================
//shows a meaning for the word on the position given 
void Board::showHint(string lcd,Dictionary dictionary, Board Board2)
{
	string word = Board2.cwWords[lcd];
	string hint = dictionary.Hint(word);
	cout << "The hint is: "<< hint << endl<<endl;
}

// =======================================================================================================================
//removes a word from the board on the location (lcd) given (returns false if there is no word on that location)
bool Board::removeWord(string lcd)
{
	if (cwWords.find(lcd) == cwWords.end())
		return false;
	else
	{
		string word = cwWords[lcd];
		if (lcd[2] == 'V')
			removeWordVertical(word, lcd);
		else
			removeWordHorizontal(word, lcd);
		cwWords.erase(lcd);
		return true;
	}
}

// =======================================================================================================================
//removes the vertical word that starts in the location (lcd) given
void Board::removeWordVertical(string word, string lcd)
{
	char line = lcd[0], column = lcd[1], direction = lcd[2];
	//deletes the letters
	for (unsigned int i = 0; i < word.length(); i++)
	{
		//if the vertical word is on the left
		if ((column - 'a' == 0) && numCol >= 2)
		{
			if (cwboard[line - 'A' + i][column - 'a' + 1] == '.')
				cwboard[line - 'A' + i][column - 'a'] = '.';
			if (cwboard[line - 'A' + i][column - 'a' + 1] == '#')
			{
				string lcd_letra;
				lcd_letra.push_back(line + i);
				lcd_letra.push_back(column);
				lcd_letra.push_back('H');
				if (cwWords.find(lcd_letra) == cwWords.end())
					cwboard[line - 'A' + i][column - 'a'] = '.';
			}
		}
		//if the vertical word is on the right
		else if (column - 'a' == numCol - 1)
		{
			if (cwboard[line - 'A' + i][column - 'a' - 1] == '.')
				cwboard[line - 'A' + i][column - 'a'] = '.';
			if (cwboard[line - 'A' + i][column - 'a' - 1] == '#')
			{
				string lcd_letra;
				lcd_letra.push_back(line + i);
				lcd_letra.push_back(column);
				lcd_letra.push_back('H');
				if (cwWords.find(lcd_letra) == cwWords.end())
					cwboard[line - 'A' + i][column - 'a'] = '.';
			}
		}
		//if the vertical word is on the center of the board
		else
		{
			bool eraseLetter = true;
			int k = line - 'A' + i + 1;
			if (cwboard[line - 'A' + i][column - 'a' - 1] == '.' && cwboard[line - 'A' + i][column - 'a' + 1] == '.')
				eraseLetter = true;
			if (isalpha(cwboard[line - 'A' + i][column - 'a' - 1]) || isalpha(cwboard[line - 'A' + i][column - 'a' + 1]))
				eraseLetter = false;
			if (cwboard[line - 'A' + i][column - 'a' - 1] == '#')
			{
				if (column - 'a' - 2 >= 0)
				{
					if (isalpha(cwboard[line - 'A' + i][column - 'a' - 2]))
						eraseLetter = false;
				}
				else if (line - 'A' + i - 1 >= 0)
				{
					if (isalpha(cwboard[line - 'A' + i - 1][column - 'a' - 1]))
						eraseLetter = false;
				}
				else if (k < numLines)
				{
					if (isalpha(cwboard[line - 'A' + i + 1][column - 'a' - 1]))
						eraseLetter = false;
				}
			}
			int l = line - 'A' + i + 1;
			if (cwboard[line - 'A' + i][column - 'a' + 1] == '#')
			{
				if (column - 'a' + 2 >= 0)
				{
					if (isalpha(cwboard[line - 'A' + i][column - 'a' + 2]))
						eraseLetter = false;
				}
				else if (line - 'A' + i - 1 >= 0)
				{
					if (isalpha(cwboard[line - 'A' + i - 1][column - 'a' + 1]))
						eraseLetter = false;
				}
				else if (l < numLines)
				{
					if (isalpha(cwboard[line - 'A' + i + 1][column - 'a' + 1]))
						eraseLetter = false;
				}
			}
			if (eraseLetter == true)
				cwboard[line - 'A' + i][column - 'a'] = '.';
		}
	}
}

// =======================================================================================================================
//removes the horizontal word that starts in the location (lcd) given
void Board::removeWordHorizontal(string word, string lcd)
{
	char line = lcd[0], column = lcd[1], direction = lcd[2];
	//deletes the letters
	for (unsigned int i = 0; i < word.length(); i++)
	{
		//if the horizontal word is on the top of the board
		if ((line - 'A' == 0) && numLines >= 2)
		{
			if (cwboard[line - 'A' + 1][column - 'a' + i] == '.')
				cwboard[line - 'A'][column - 'a' + i] = '.';
			if (cwboard[line - 'A' + 1][column - 'a' + i] == '#')
			{
				string lcd_letra;
				lcd_letra.push_back(line);
				lcd_letra.push_back(column + i);
				lcd_letra.push_back('V');
				if (cwWords.find(lcd_letra) == cwWords.end())
					cwboard[line - 'A'][column - 'a' + i] = '.';
			}
		}
		//if the horizontal word is on the bottom of the board
		else if (line - 'A' == numLines - 1)
		{
			if (cwboard[line - 'A' - 1][column - 'a' + i] == '.')
				cwboard[line - 'A'][column - 'a' + i] = '.';
			if (cwboard[line - 'A' - 1][column - 'a' + i] == '#')
			{
				string lcd_letra;
				lcd_letra.push_back(line);
				lcd_letra.push_back(column + i);
				lcd_letra.push_back('V');
				if (cwWords.find(lcd_letra) == cwWords.end())
					cwboard[line - 'A'][column - 'a' + i] = '.';
			}
		}
		//if the horizontal word is on the middle of the board
		else
		{
			int k = column - 'a' + 1 + i;
			bool eraseLetter = true;
			if (cwboard[line - 'A' - 1][column - 'a' + i] == '.' && cwboard[line - 'A' + 1][column - 'a' + i] == '.')
				eraseLetter = true;
			if (isalpha(cwboard[line - 'A' - 1][column - 'a' + i]) || isalpha(cwboard[line - 'A' + 1][column - 'a' + i]))
				eraseLetter = false;
			if (cwboard[line - 'A' - 1][column - 'a' + i] == '#')
			{
				if (line - 'A' - 2 >= 0)
				{
					if (isalpha(cwboard[line - 'A' - 2][column - 'a' + i]))
						eraseLetter = false;
				}
				else if (column - 'a' + i - 1 >= 0)
				{
					if (isalpha(cwboard[line - 'A' - 1][column - 'a' - 1 + i]))
						eraseLetter = false;
				}
				else if (k <= numCol)
				{
					if (isalpha(cwboard[line - 'A' - 1][column - 'a' + i + 1]))
						eraseLetter = false;
				}
			}
			if (cwboard[line - 'A' + 1][column - 'a' + i] == '#')
			{
				if (line - 'A' + 2 >= 0)
				{
					if (isalpha(cwboard[line - 'A' + 2][column - 'a' + i]))
						eraseLetter = false;
				}
				else if (column - 'a' + i - 1 >= 0)
				{
					if (isalpha(cwboard[line - 'A' + 1][column - 'a' + i - 1]))
						eraseLetter = false;
				}
				else if (k <= numCol)
				{
					if (isalpha(cwboard[line - 'A' + 1][column - 'a' + i + 1]))
						eraseLetter = false;
				}
			}
			if (eraseLetter == true)
				cwboard[line - 'A'][column - 'a' + i] = '.';
		}

	}
}

// =======================================================================================================================
//verifies if placing the word given on the location given will overwrite any other word already placed
bool Board::changesBoard(string word, char line, char column, char direction)
{
	bool answer = false;
	//verifies for each cell where the word will be placed, if the letter of the word equals the letter on the board
	if (direction == 'V')
		for (unsigned int i = 0; i < word.length(); i++)
		{
			if ((cwboard[line - 'A' + i][(column - 'A')] != word[i]) && (cwboard[line - 'A' + i][(column - 'A')] != '.'))
				answer = true;
		}
	else
		for (unsigned int i = 0; i < word.length(); i++)
		{
			if ((cwboard[line - 'A'][(column - 'A' + i)] != word[i]) && (cwboard[line - 'A'][(column - 'A' + i)] != '.'))
				answer = true;
		}
	int i = line - 'A' + int(word.length());
	int j = column - 'A' + int(word.length());
	//verifies if there is a letter on the cell after the ending cell of the word
	//or a letter onthe cell before the starting cell of the word
	if (direction == 'V')
	{
		if (line - 'A' > 0)
			if ((cwboard[(line - 'A' - 1)][(column - 'A')] != '.') && (cwboard[(line - 'A' - 1)][(column - 'A')] != '#'))
				answer = true;
		if (i < numLines)
			if ((cwboard[(line - 'A' + word.length())][(column - 'A')] != '.') && (cwboard[(line - 'A' + word.length())][(column - 'A')] != '#'))
				answer = true;
	}
	else
	{
		if (column - 'A' > 0)
			if ((cwboard[(line - 'A')][(column - 'A' - 1)] != '.') && (cwboard[(line - 'A')][(column - 'A' - 1)] != '#'))
				answer = true;
		if (j < numCol)
			if ((cwboard[(line - 'A')][(column - 'A' + word.length())] != '.') && (cwboard[(line - 'A')][(column - 'A' + word.length())] != '#'))
				answer = true;
	}
	return answer;
}

// =======================================================================================================================
//Removes all the words from the board -> only leaves the existing '#'
//Removes all the words from cwWords
void Board::emptyBoard()
{
	for (int i = 0; i < numLines; i++)
	{
		for (int j = 0; j < numCol; j++)
		{
			if (cwboard[i][j] != '#')
				cwboard[i][j] = '.';
		}	
	}
	cwWords.clear();
}

// =======================================================================================================================
//Returns true if cwboard from the board is equal to cwboard from board2
bool Board::equalBoards(Board board2)
{
	if (this->cwboard == board2.cwboard)
		return true;
	else
		return false;
}

// =======================================================================================================================
//Removes a cell from the board
void Board::removeCell(string lcd)
{
	if (cwboard[lcd[0] - 'A'][lcd[1] - 'a'] == '#')
		cout << "You can't remove a black cell!" << endl;
	else
		cwboard[lcd[0] - 'A'][lcd[1] - 'a'] = '.';
}

// =======================================================================================================================
//Adds a letter to a specific cell
void Board::addCell(string lcd)
{
	if (cwboard[lcd[0] - 'A'][lcd[1] - 'a'] == '#')
		cout << "You can't add a word over a black cell." << endl;
	else if (cwboard[lcd[0] - 'A'][lcd[1] - 'a'] != '.')
		cout << "There's already a letter in that place." << endl;
	else
	{
		string letter;
		cout << "What is the letter you wish to add? ";
		cin >> letter;
		while (!cin || !isalpha(letter[0]) || letter.length()!=1)
		{
			cin.clear();
			cin.ignore(100000, '\n');
			cout << "What is the letter you wish to add? ";
			cin >> letter;
		}
		cwboard[lcd[0] - 'A'][lcd[1] - 'a'] = toupper(letter[0]);
	}
}