#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <iomanip>
#include <sstream>
using namespace std;

class Dictionary
{
	friend class Board;
public:
	void toUpperWord(string &word);
	bool isValid(string word);
	void extractMeanings(string line, string word);
	void extractWords(ifstream& inFile);
	string Hint(string word);
private:
	//multimap that contains the words and synonyms from the dictionary -> key:word, value:1 synonym
	multimap<string, string> wordDictionary;
};