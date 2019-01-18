#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <iomanip>
#include <sstream>
#include "Dictionary.h"
#include "Player.h"
using namespace std;

class Board
{
	friend class Dictionary;
	friend class Player;
public:																				
	void toUpperWord(string &word);
	void board(ifstream& inFile);
	void updateBoard(Dictionary wDictionary, Board Board2, Player &cwPlayer);
	void showBoard();														
	void addWord(string word, string lcd);									
	bool verifyWord(string word, string lcd);		
	void showHint(string lcd, Dictionary wDictionary, Board Board2);									
	bool removeWord(string lcd);											
	void removeWordVertical(string word, string lcd);						
	void removeWordHorizontal(string word, string lcd);						
	bool changesBoard(string word, char line, char column, char direction);
	void emptyBoard();
	bool equalBoards(Board board2);
	void removeCell(string lcd);
	void addCell(string lcd);
private:
	//bidimensional vector that represents the Board -> acess mode: cwboard[line][column]
	vector<vector<char>> cwboard;	
	int numLines;					//number of lines
	int numCol;						//number of columnsposition
	//map that contains the words already placed on the Board ->key: on the Board; value:word
	map<string, string> cwWords;
};