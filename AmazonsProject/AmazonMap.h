#pragma once

#ifndef _AMAZON_MAP_H_
#define _AMAZON_MAP_H_

#include <vector>
#include <map>
#include <set>
#include <iostream>
#include <ostream>
#include <cstdio>
#include <fstream>
#include <string>
#include <sstream>


template<typename T>
void EnterNumToChar(T& instream, int& num, const char c = '\0')
{
	std::string nn;
	instream.ignore();
	std::getline(instream, nn, c);

	std::stringstream ss(nn);
	ss >> num;
}


enum class CellStatus
{
	EMPTY,
	BLOCKED,
	WHITE,
	BLACK
};

enum class PlayerColor
{
	BLACK,	/// 1st player --- b(L)ue/black
	WHITE,	/// 2nd player --- (R)ed/white
	UNDEFINED	/// for OPT check
};

using position = std::pair<int, int>;

class AmazonMap
{
public:
	AmazonMap(int nWidth, int nHeight, PlayerColor nPM = PlayerColor::BLACK);
	AmazonMap(const AmazonMap& lhs);
	AmazonMap(std::string textMap);

	std::vector<position> VisibleCellsFromHere(position mP);
	std::vector<position> VisibleCellsFromHere(int row, int col);
	void SwitchPlayer();

	void MakeTheMove(std::tuple<position, position> nMove, PlayerColor forOPT = PlayerColor::UNDEFINED);
	void Block(position nBlock);

	friend std::ostream& operator<<(std::ostream& out, const AmazonMap& am);

	const int& GetHeight() const;
	const int& GetWidth() const;

	int GetScope(PlayerColor nCol);

	PlayerColor GetCurrentPlayer();

//	int GameTreeHeight();											/// yellow XD
//	std::tuple<position, position, position> GetOptimalMove(PlayerColor pColor);
public:
	std::map<PlayerColor, std::vector<position>> playerPositions;
	static std::map<CellStatus, PlayerColor> coherence;
	static std::map<PlayerColor, CellStatus> swCoherence;
private:
private:
	int width, height;
	std::map<int, std::map<int, CellStatus>> map;

	PlayerColor currentPlayerMove;
	position lastMove;
};



#endif

