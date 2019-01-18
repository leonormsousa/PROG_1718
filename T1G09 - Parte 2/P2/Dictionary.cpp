#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <iomanip>
#include <sstream>
#include "Dictionary.h"
using namespace std;

//====================================================================================================================================
//puts the all word given in uppercases
void Dictionary::toUpperWord(string &word)
{
	for (size_t i = 0; i < word.size(); i++)
	{
		word[i] = toupper(word[i]);
	}
}

//====================================================================================================================================
//verifies if the word exists
bool Dictionary::isValid(string word)
{
	multimap<string, string>::iterator it;
	it = wordDictionary.find(word);
	if (it != wordDictionary.end())
		return true;
	else
		return false;
}

//====================================================================================================================================
//receives a line from the dictionary and creates a vector  with all the possible meanings 
void Dictionary::extractMeanings(string line, string word)
{
	vector<string> meanings;
	int j = int(line.size());
	int i = int(line.find_first_of(':'))+2, k=0;
	string s = line.substr(0, line.find_first_of(':'));
	while ( k <= j && (int(line.find_first_of(',')) != int(line.length())-1 && int(line.find_first_of(',')) != string::npos))
	{	
			if ((line[i] >= 'a') && (line[i] <= 'z') || line[i] == '\'') //beggining of a word
			{
				k = k +int(line.find_first_of(','));
				line = line.substr(i); //removes the words that have already been added
				if (line.substr(1) != "")
				{
					wordDictionary.insert(pair<string, string>(word, line.substr(0, line.find_first_of(','))));
					i = int(line.find_first_of(',')) + 1;
				}
				else
					break;
			}
			else
			{
				k++;
				i++;
			}
	}
}

//====================================================================================================================================
//Extracts all the words from the inFile creating a map where the keys are the words and the values are their meanings 
void Dictionary::extractWords(ifstream& inFile)
{
	string line;
	vector<string> meanings;
	while (getline(inFile, line))
	{
		if ((line[0] >= 'A') && (line[0] <= 'Z')) //if the newline represents a new word
		{
			string word = line.substr(0, line.find_first_of(':'));
			toUpperWord(word);
			if (line.find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz,:' ") == string::npos)
			{	
				string restOfLine = line.substr(line.find_first_of(':') + 1);
				extractMeanings(restOfLine, word);
			}
		}
	}
}

//========================================================================================================================================
//returns a random synonym of the word given
string Dictionary::Hint(string word)
{
	multimap<string, string>::iterator itlow = wordDictionary.lower_bound(word), ithigh = wordDictionary.upper_bound(word), it;
	int i = int(distance(itlow, ithigh));
	srand((unsigned int)time(NULL));
	int k = rand() % (i - 1);
	advance(itlow, k);
	string hint = itlow->second;
	return hint;
}