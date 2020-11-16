#pragma once

#ifndef _AMAZON_MAP_H_
#define _AMAZON_MAP_H_

#include <vector>
#include <map>
#include <set>
#include <iostream>
#include <ostream>
#include <cstdio>


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

	std::vector<position> VisibleCellsFromHere(position mP);
	std::vector<position> VisibleCellsFromHere(int row, int col);
	void SwitchPlayer();
	void UpdatePlayerPositions();

	void MakeTheMove(std::tuple<position, position, position> nMove);

	friend std::ostream& operator<<(std::ostream& out, const AmazonMap& am);

	const int& GetHeight() const;
	const int& GetWidth() const;
public:
	std::map<PlayerColor, std::set<position>> playerPositions;
	static std::map<CellStatus, PlayerColor> coherence;
	static std::map<PlayerColor, CellStatus> swCoherence;
private:
private:
	int width, height;
	CellStatus** map;

	PlayerColor currentPlayerMove;
};



#endif

