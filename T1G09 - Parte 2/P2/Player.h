#pragma once
#include <iostream>
#include <string>
#include <vector>
using namespace std;

class Player
{
public:
	void player(string name);
	void savePlayer(string outFileName);
	void finishTry();
	void incrementClues();
private:
	string pName;			//player's name
	time_t initialTime;		//time when the player starts to play
	time_t totalTime;		//total time spent by player
	int numClues;			//number of clues requested by player
};