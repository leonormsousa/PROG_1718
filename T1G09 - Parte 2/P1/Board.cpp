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
using namespace std;

//=========================================================================================================================
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

//========================================================================================================================
//Construtor -> creates an empty board with nlines lines and ncol columns 
void Board::board(int nlines, int ncol)
{
	cwboard.resize(nlines);
	for (int i = 0; i < nlines; i++)
	{
		for (int j = 0; j < ncol; j++)
			cwboard[i].push_back('.');
	}
	numLines = nlines;
	numCol = ncol;
}

// =======================================================================================================================
//Constructor -> creates a board from a previous board stored in a file 
string Board::board(ifstream& inFile)
{
	int lineBoard = 0, ncol = 0, nlines = 0;
	string line, dictionaryName;
	vector<vector<char> > vf;
	getline(inFile, dictionaryName);
	getline(inFile, line);
	getline(inFile, line);
	while (line != "")
	{
		nlines++;
		vector<char> v;
		for (size_t i = 0; i < line.length(); i++)
			v.push_back(line[i]);
		vf.push_back(v);
		ncol = v.size();
		getline(inFile, line);
	}
	cwboard = vf;
	numCol = ncol;
	numLines = nlines;
	while (getline(inFile, line) && (line != ""))
		cwWords[line.substr(0, 3)] = line.substr(4);
	return dictionaryName;
}

// =======================================================================================================================
//puts the all word given in uppercases
void Board::toUpperWord(string &word)
{
	for (size_t i = 0; i < word.size(); i++)
	{
		word[i] = toupper(word[i]);
	}
}

// =======================================================================================================================
//allows to change the board adding several words 
//returns true if it is to save the board
bool Board::updateBoard(Dictionary &wDictionary)
{
	cout << endl << "Instructions: " << endl;
	cout << "The position must be inserted in the format LCD (Line-Column-Direction).";
	cout << endl << "Example: AaV -> Line A, Column a, Vertical" << endl;
	cout << "If you wish to remove a word, enter the symbol '-' when asked for the word." << endl;
	cout << "If you want some sugestions, enter the symbol '?' when asked for the word." << endl;
	cout << "When you have finished, please press CTRL-Z." << endl << endl;
	string lcd;
	string word;
	bool stop = false;
	//while the user doesn't press ctrl-Z
	while (true)
	{
		Board::showBoard();
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
			cout << "Position (LCD / CTRL-Z = stop)? ";
			cin >> lcd;
			
		}
		if (stop)
			break;
		lcd[0] = toupper(lcd[0]);
		lcd[1] = toupper(lcd[1]) - 'A' + 'a';
		lcd[2] = toupper(lcd[2]);
		//INPUT OF A WORD
		cout << "Word? ";
		cin >> word;
		//REMOVES THE WORD IN THAT POSITION
		if (word == "-")
		{
			if (!removeWord(lcd))
				cout << endl << "There was no word in that position.";
			cout << endl << endl;
		}
		//SHOWS ALL THE POSSIBLE WORDS TO PUT IN THAT POSITION
		else if (word == "?") 
		{
			showSuggestions(lcd, wDictionary);
		}
		//ADDS THE WORD IN THAT POSITION
		else
		{
			toUpperWord(word);
			int error = verifyWord(word, lcd, wDictionary);
			//CHECKS IF THE WORD CAN BE ADDED
			if (error == 0)
			{
				addWord(word, lcd);
				cout << endl << endl;
			}
			else if (error == 1)
				cout << "The word " << word << " doesn't exist on the dictionary." << endl << endl;
			else if (error == 2)
				cout << "The word " << word << " doesn't fit in the board." << endl << endl;
			else if (error == 3)
				cout << "The word " << word << " has already been put in the board." << endl << endl;
			else if (error == 4)
				cout << "The word " << word << " cannot be placed in that position without changing other preexisting words." << endl << endl;
			else
			{
				char yes_no;
				cout << "The placing of the word " << word << " in that position could create non valid words, as well as errors on the board. Please verify if this is the case."
					<< endl << "Do you still wish to add the word (Y/N)?";
				cin >> yes_no;
				while (!cin || ((yes_no != 'Y') && (yes_no != 'y') && (yes_no != 'n') && (yes_no != 'N')))
				{
					cin.clear();
					cin.ignore(100000, '\n');
					setcolor(RED, WHITE);
					cout << endl << "Invalid Input" << endl;
					setcolor(WHITE, BLACK);
					cout<< "Do you still wish to add the word (Y/N)? ";
					cin >> yes_no;
				}
				if (yes_no == 'Y' || yes_no == 'y')
					addWord(word, lcd);
				cout << endl << endl;
			}
		}
	}
	//SAVES THE BOARD
	char finish_resume;
	cout << endl << endl << "Is this project finished(F) or do you wish to resume it later(R)? ";
	cin >> finish_resume;
	while (!cin || ((finish_resume != 'F') && (finish_resume != 'f') && (finish_resume != 'r') && (finish_resume != 'R')))
	{
		cin.clear();
		cin.ignore(100000, '\n');
		setcolor(RED, WHITE);
		cout << endl << "Invalid Input" << endl;
		setcolor(WHITE, BLACK);
		cout<< "Is this project finished(F) or do you wish to resume it later(R)? ";
		cin >> finish_resume;
	}
	finish_resume = toupper(finish_resume);
	if (finish_resume == 'F')
		finishBoard();
	if (!verifyBoard(wDictionary) && (finish_resume == 'F'))
	{
		cout << "Not all of the words in the board are valid words." << endl <<
			"Your board will be deleted." << endl;
		return false;
	}
	return true;
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
//adds the word given to the board in the position given (lcd)
void Board::addWord(string word, string lcd)
{
	if (lcd[2] == 'V')
	{
		for (size_t i = 0; i < word.length(); i++)
			cwboard[(lcd[0] - 'A') + i][lcd[1] - 'a'] = word[i];
		if (lcd[0] > 'A')
			cwboard[(lcd[0] - 'A' - 1)][lcd[1] - 'a'] = '#';
		int i = lcd[0] - 'A' + word.length() + 1;
		if (i <= numLines)
			cwboard[lcd[0] - 'A' + word.length()][lcd[1] - 'a'] = '#';
	}
	else
	{
		for (size_t i = 0; i < word.length(); i++)
			cwboard[lcd[0] - 'A'][(lcd[1] - 'a') + i] = word[i];
		if (lcd[1] > 'a')
			cwboard[(lcd[0] - 'A')][lcd[1] - 'a' - 1] = '#';
		int i = lcd[1] - 'a' + word.length() + 1;
		if (i <= numCol)
			cwboard[lcd[0] - 'A'][lcd[1] - 'a' + word.length()] = '#';
	}
	cwWords[lcd] = word;
}

// =======================================================================================================================
//verifies: if the word doesn't exist ->1
//if there isn't space to place it in the board -> 2
//if it has already been placed in the board -> 3
//if it can't be placed in that location without changing letters (or #) already placed -> 4
//ONLY GIVES A WARNING: if it creates non valid words-> 5
//if valid -> 0
int Board::verifyWord(string word, string lcd, Dictionary &wDictionary)
{
	int i = word.length() - 'A' + lcd[0];
	int j = word.length() - 'a' + lcd[1];
	if (!wDictionary.isValid(word))
		return 1;
	else if ((lcd[2] == 'V')&& (i > numLines))
		return 2;
	else if ((lcd[2] == 'H') && (j > numCol))
		return 2;
	else if (repeatedWord(word))
		return 3;
	else if (changesBoard(word, lcd[0], toupper(lcd[1]), lcd[2]))
		return 4;
	else if (createsNonExistingWord(word, lcd, wDictionary))
		return 5;
	else
		return 0;
}

// =======================================================================================================================
//shows all the words that can be placed in the location (lcd) given
void Board::showSuggestions(string lcd, Dictionary &wDictionary)
{
	bool nowords = true;
	vector<string> words = wDictionary.DictionaryWords();
	//verifies if there is any suggestion to be printed
	for (unsigned int i = 0; i<words.size(); i++)
	{
		string s = words[i];
		int v = verifyWord(s, lcd, wDictionary);
		if (v == 0)
		{
			nowords = false;
			cout << "Here are some suggestions of words to place in the location specified: " << endl;
			break;
		}	
	}
	//if there aren't any suggestions prints a message
	if (nowords)
		cout << "There are no words that can be placed in that position." << endl;
	int counter = 0;
	//prints the suggestions
	for (unsigned int i=0; i<words.size(); i++)
	{
		string s = words[i];
		int v = verifyWord(s, lcd, wDictionary);
		if (v == 0)
		{
			if (counter == 4)
				cout << setw(19) << s << endl;
			else
				cout << setw(19) << s << "    ";
			counter = (counter + 1) % 5;
		}
	}
	cout << endl << endl;
}

// =======================================================================================================================
//removes the word in the location lcd from the board (returns false if there isn't a word on that location)
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
	//deletes any '#' before the word
	if (line - 'A' - 1 >= 0)
	{
		bool erase_hashtag = true;
		if (line - 'A' - 2 >= 0)
		{
			if (isalpha(cwboard[line - 'A' - 2][column - 'a']))
				erase_hashtag = false;
		}
		if (column - 'a' - 1 >= 0)
		{
			if (isalpha(cwboard[line - 'A' - 1][column - 'a' - 1]))
				erase_hashtag = false;
		}
		if (column - 'a' + 1 < numCol)
		{
			if (isalpha(cwboard[line - 'A' - 1][column - 'a' + 1]))
				erase_hashtag = false;
		}
		if (erase_hashtag == true)
			cwboard[line - 'A' - 1][column - 'a'] = '.';
	}
	//deletes any '#'after the word
	int i = line - 'A' + word.length();
	
	if (i < numLines)
	{
		bool erase_hashtag = true;
		if (i+1 < numLines)
		{
			if (isalpha(cwboard[line - 'A' + word.length() + 1][column - 'a']))
				erase_hashtag = false;
		}
		if (column - 'a' - 1 >= 0)
		{
			if (isalpha(cwboard[line - 'A' + word.length()][column - 'a' - 1]))
				erase_hashtag = false;
		}
		if (column - 'a' + 1 < numCol)
		{
			if (isalpha(cwboard[line - 'A' + word.length()][column - 'a' + 1]))
				erase_hashtag = false;
		}
		if (erase_hashtag == true)
			cwboard[line - 'A' + word.length()][column - 'a'] = '.';
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
	//deletes any '#' before the word
	if (column - 'a' - 1 >= 0)
	{
		bool erase_hashtag = true;
		if (column - 'a' - 2 >= 0)
		{
			if (isalpha(cwboard[line - 'A'][column - 'a' - 2]))
				erase_hashtag = false;
		}
		if (line - 'A' - 1 >= 0)
		{
			if (isalpha(cwboard[line - 'A' - 1][column - 'a' - 1]))
				erase_hashtag = false;
		}
		if (line - 'A' + 1 < numLines)
		{
			if (isalpha(cwboard[line - 'A' + 1][column - 'a' - 1]))
				erase_hashtag = false;
		}
		if (erase_hashtag == true)
			cwboard[line - 'A'][column - 'a' - 1] = '.';
	}
	//deletes any '#' after the word
	int i = column - 'a' + word.length();
	if (i < numCol)
	{
		bool erase_hashtag = true;
		if (i+1 < numCol)
		{
			if (isalpha(cwboard[line - 'A'][column - 'a' + word.length() + 1]))
				erase_hashtag = false;
		}
		if (line - 'A' - 1 >= 0)
		{
			if (isalpha(cwboard[line - 'A' - 1][column - 'a' + word.length()]))
				erase_hashtag = false;
		}
		if (line - 'A' + 1 < numLines)
		{
			if (isalpha(cwboard[line - 'A' + 1][column - 'a' + word.length()]))
				erase_hashtag = false;
		}
		if (erase_hashtag == true)
			cwboard[line - 'A'][column - 'a' + word.length()] = '.';
	}
}

// =======================================================================================================================
//saves the board and the words in it on a file (numOutFile)
void Board::saveBoard(string dictionaryName, int numOutFile)
{
	//creates the name of the output file
	string nameOutFile = "b";
	ofstream outFile;
	if (numOutFile < 10)
	{
		nameOutFile.append("00");
		nameOutFile.append(to_string(numOutFile));
	}
	else if (numOutFile < 100)
	{
		nameOutFile.append("0");
		nameOutFile.append(to_string(numOutFile));
	}
	else
		nameOutFile.append(to_string(numOutFile));
	nameOutFile.append(".txt");
	outFile.open(nameOutFile);
	//loads the dictionary name to the file
	outFile << dictionaryName << endl << endl;
	//loads the board to the file
	for (int i = 0; i < numLines; i++)
	{
		for (int j = 0; j < numCol; j++)
			outFile << cwboard[i][j];
		outFile << endl;
	}
	outFile << endl;
	//loads the cwWords map to the file
	for (map<string, string>::const_iterator mi = cwWords.begin(); mi != cwWords.end(); mi++)
		outFile << mi->first << ' ' << mi->second << endl;
	outFile.close();
}

// =======================================================================================================================
//finishes the board adding '#' in every blank space
void Board::finishBoard()
{
	for (int i = 0; i < numLines; i++)
	{
		for (int j = 0; j < numCol; j++)
			if (cwboard[i][j] == '.')
				cwboard[i][j] = '#';
	}
}

// =======================================================================================================================
//returns true if all the words on the board are valid
//updates cwWords
bool Board::verifyBoard(Dictionary wDictionary)
{
	map <string, string> newCwWords;
	bool answer = true;
	//GOES THROUGH EVERY LINE
	for (int i = 0; i < numLines; i++)
	{
		char actualColumn = 'a';
		string linha = "";
		//transforms a line into a string
		for (int j = 0; j < numCol; j++)
		{
			linha.push_back(cwboard[i][j]);
		}
		//verifies if there is actually a word on that line
		if (linha.find_first_of("ABCDEFGHIJKLMNOPQRSTUVWXYZ") == string::npos)
			linha = "";
		else
		{
			actualColumn = actualColumn + char(linha.find_first_of("ABCDEFGHIJKLMNOPQRSTUVWXYZ"));
			linha = linha.substr(linha.find_first_of("ABCDEFGHIJKLMNOPQRSTUVWXYZ"));
		}
		//while there are characters to be processed in the line
		while (linha != "")
		{
			string word;
			word = linha.substr(0, linha.find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZ"));
			//verifies if the word is valid and adds it to the newCwWords if it is
			if (!wDictionary.isValid(word)&& word.length()>1)
				answer = false;
			else if (word.length()>1)
			{
				string lcd(1, char(i + 'A'));
				lcd += char(actualColumn);
				lcd += 'H';
				newCwWords[lcd] = word;
			}
			//prepares the line to be processed next
			if (linha.find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZ") == string::npos)
				break;
			actualColumn = actualColumn + char(linha.find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZ"));
			linha = linha.substr(linha.find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZ"));
			if (linha.find_first_of("ABCDEFGHIJKLMNOPQRSTUVWXYZ") == string::npos)
				break;
			actualColumn = actualColumn + char(linha.find_first_of("ABCDEFGHIJKLMNOPQRSTUVWXYZ"));
			linha = linha.substr(linha.find_first_of("ABCDEFGHIJKLMNOPQRSTUVWXYZ"));

		}
	}
		//GOES THROUGH EVERY COLUMN
		for (int j = 0; j < numCol; j++)
		{
			char actualLine = 'A';
			string linha = "";
			//transforms a column into a string
			for (int i = 0; i < numLines; i++)
			{
				linha.push_back(cwboard[i][j]);
			}
			//verifies if there is actually a word on that column
			if (linha.find_first_of("ABCDEFGHIJKLMNOPQRSTUVWXYZ") == string::npos)
				linha = "";
			else
			{
				actualLine = actualLine + char(linha.find_first_of("ABCDEFGHIJKLMNOPQRSTUVWXYZ"));
				linha = linha.substr(linha.find_first_of("ABCDEFGHIJKLMNOPQRSTUVWXYZ"));
			}
			//while there are characters to be processed in the line
			while (linha != "")
			{
				string word;
				word = linha.substr(0, linha.find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZ"));
				//verifies if the word is valid and adds it to the newCwWords if it is
				if (!wDictionary.isValid(word)&& word.length()>1)
					answer = false;
				else if (word.length()>1)
				{
					string lcd(1, char(actualLine));
					lcd += (j + 'a');
					lcd += 'V';
					newCwWords[lcd] = word;
				}
				//prepares the line to be processed next
				if (linha.find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZ") == string::npos)
					break;
				actualLine = actualLine + char(linha.find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZ"));
				linha = linha.substr(linha.find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZ"));
				if (linha.find_first_of("ABCDEFGHIJKLMNOPQRSTUVWXYZ") == string::npos)
					break;
				actualLine = actualLine + char(linha.find_first_of("ABCDEFGHIJKLMNOPQRSTUVWXYZ"));
				linha = linha.substr(linha.find_first_of("ABCDEFGHIJKLMNOPQRSTUVWXYZ"));
			}
		}
	cwWords = newCwWords;
	return answer;
}

// =======================================================================================================================
//verifies if the word given was already placed on the board
bool Board::repeatedWord(string word)
{
	bool answer = false;
	for (map<string, string>::const_iterator mi = cwWords.begin(); mi != cwWords.end(); mi++)
	{
		if ((mi->second) == word)
		{
			answer = true;
			break;
		}
	}
	return answer;
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
	int i = line - 'A' + word.length();
	int j = column - 'A' + word.length();
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

//========================================================================================================================
//verifies if placing the word will create non existing words
bool Board::createsNonExistingWord(string word, string lcd, Dictionary wDictionary)
{
	Board temporaryBoard;
	temporaryBoard.cwboard = cwboard;
	temporaryBoard.numCol = numCol;
	temporaryBoard.numLines = numLines;
	temporaryBoard.addWord(word, lcd);
	if (temporaryBoard.verifyBoard(wDictionary))
		return false;
	else
		return true;
}