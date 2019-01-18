#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <iomanip>
#include <sstream>
#include "Dictionary.h"
using namespace std;

class Board
{
	friend class Dictionary;
public:
	void board(int nlines, int ncol);										
	string board(ifstream& inFile);											
	void toUpperWord(string &word);
	bool updateBoard(Dictionary &wDictionary);								
	void showBoard();														
	void addWord(string word, string lcd);									
	int verifyWord(string word, string lcd, Dictionary &wDictionary);		
	void showSuggestions(string lcd, Dictionary &wDictionary);				
	bool removeWord(string lcd);											
	void removeWordVertical(string word, string lcd);
	void removeWordHorizontal(string word, string lcd);
	void saveBoard(string dictionaryName, int numOutFile);					
	void finishBoard();														
	bool verifyBoard(Dictionary wDictionary);							
	bool repeatedWord(string word);
	bool changesBoard(string word, char line, char column, char direction);
	bool createsNonExistingWord(string word, string lcd, Dictionary wDictionary);
private:
	// bidimensional vector that represents the Board -> acess mode: cwboard[line][column]
	vector<vector<char>> cwboard;
	int numLines;                 //number of lines
	int numCol;                   //number of columns
	//map that contains the words already placed in the board ->key:position on the Board; value:word
	map<string, string> cwWords;
};