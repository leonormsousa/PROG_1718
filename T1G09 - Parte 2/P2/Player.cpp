#include "Player.h"
#include <iomanip>
#include <fstream>
#include <ctime>

//==========================================================================================================================
//constructor -> creates a new player
void Player::player(string name)
{
	pName = name;
	initialTime = time(0);
	totalTime = 0;
	numClues = 0;
}

//==========================================================================================================================
//saves the player's data
void Player::savePlayer(string outFileName)
{
	ofstream outFile;
	outFile.open(outFileName, ofstream::out | ofstream::app);
	outFile << setw(10) << pName << setw(10) << totalTime << setw(13) << numClues <<endl;
	outFile.close();
}

//==========================================================================================================================
//finishes an attempt -> calculates the time spent
void Player::finishTry()
{
	time_t now = time(0);
	totalTime = now - initialTime;
}

//==========================================================================================================================
//increments the number of clues used by a player
void Player::incrementClues()
{
	numClues++;
}