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



template<typename T>
void EnterNumToChar(T& instream, int& num, const char c = '\0');


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
	WHITE	/// 2nd player --- (R)ed/white
};

using position = std::pair<int, int>;

class AmazonMap
{
public:
	AmazonMap(int nWidth, int nHeight, PlayerColor nPM = PlayerColor::BLACK);
	AmazonMap(const AmazonMap& lhs);
	AmazonMap(std::string textMap);

	std::set<position> VisibleCellsFromHere(position mP);
	std::set<position> VisibleCellsFromHere(int row, int col);
	void SwitchPlayer();

	void MakeTheMove(std::tuple<position, position> nMove);
	void Block(position nBlock);

	friend std::ostream& operator<<(std::ostream& out, const AmazonMap& am);

	const int& GetHeight() const;
	const int& GetWidth() const;

	int GetScope(PlayerColor nCol);

	PlayerColor GetCurrentPlayer();
public:
	std::map<PlayerColor, std::set<position>> playerPositions;
	static std::map<CellStatus, PlayerColor> coherence;
	static std::map<PlayerColor, CellStatus> swCoherence;
private:
private:
	int width, height;
	CellStatus** map;

	PlayerColor currentPlayerMove;
	position lastMove;
};



#endif

