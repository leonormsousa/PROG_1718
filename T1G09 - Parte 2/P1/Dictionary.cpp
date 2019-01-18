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
//receives a line (of several synonyms) from the dictionary and creates a vector with all the possible synonyms 
void Dictionary::extractMeanings(string line, string word)
{
	vector<string> meanings;
	int j = int(line.size());
	int i = int(line.find_first_of(':'))+2, k=0;
	string s = line.substr(0, line.find_first_of(':'));
	while ( k <= j && (int(line.find_first_of(',')) != int(line.length())-1 && int(line.find_first_of(',')) != string::npos))
	{	
			if ((line[i] >= 'a') && (line[i] <= 'z')) //beggining of a word
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
//extracts all the words from the inFile creating a map where the keys are the words and the values are their meanings 
void Dictionary::extractWords(ifstream& inFile)
{
	string line;
	vector<string> meanings;
	while (getline(inFile, line))
	{
		if ((line[0] >= 'A') && (line[0] <= 'Z') || (line[0]=='\'')) //if the newline represents a new word
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

//====================================================================================================================================
// returns a vector with all of the words/headlines from the dictionary
vector<string> Dictionary::DictionaryWords()
{
	vector<string> words;
	for (const auto x:wordDictionary)
	{
		bool repeated = false;
		for (unsigned int i = 0; i < words.size(); i++)
		{
			if (x.first == words[i])
			{
				repeated = true;
				break;
			}
		}
		if (!repeated)
			words.push_back(x.first);
	}
	return words;
}