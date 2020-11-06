#pragma once

#ifndef _AMAZON_MAP_H_
#define _AMAZON_MAP_H_

#include <vector>

enum class CellStatus
{
	EMPTY,
	BLOCKED,
	WHITE,
	BLACK
};

class AmazonMap
{
public:
	AmazonMap(int nWidth, int nHeight);

	std::vector<std::pair<int, int>> VisibleCellsFromHere(int row, int col);
public:
private:
private:
	int width, height;
	CellStatus** map;
};



#endif

