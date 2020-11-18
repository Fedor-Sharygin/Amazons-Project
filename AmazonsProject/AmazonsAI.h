#pragma once

#ifndef _AMAZONS_AI_H_
#define _AMAZONS_AI_H_

#include <algorithm>

#include "AmazonMap.h"


class AmazonsAI
{
public:
	///									default = min/max
	AmazonsAI(PlayerColor mColor, bool nStrategy = true);
	~AmazonsAI();

	/// <summary>
	/// Analyzes the map
	/// </summary>
	/// <returns>
	/// returns the best move
	/// </returns>
	///		   start	 move to   block
	std::tuple<position, position, position> BestMove();

	void PassTheMap(AmazonMap* nMap);

	void MakeBestMove();
public:
private:
private:
	AmazonMap* currentMap;

	PlayerColor player;

	/// true	-	min/max
	///	false	-	max/min
	bool strategy;
};



#endif

