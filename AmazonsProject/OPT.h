#pragma once

#ifndef _OPT_H_
#define _OPT_H_

#include "AmazonMap.h"


/// Copied from Domineering project
enum class OutcomeClass
{
	LEFT,
	RIGHT,
	NEXT_FIRST,
	PREVIOUS_SECOND,
	UNDETERMINED
};

/// <summary>
/// Optimal play game tree class
/// 
/// This class is used to check what game
/// tree current game is going to have
/// It is extremely expensive, due to
/// copying and playing out the game up to three
/// (possibly five) moves ahead
/// It is created at every move
/// </summary>

class OPT
{
public:
	OPT(AmazonMap cMap, int maxLevel, int nLevel = 0);
	std::tuple<position, position, position> BestMove(PlayerColor tCol);
public:
	OutcomeClass moc;
	int level;
	bool finished;
private:
private:
	std::map<PlayerColor, std::map<std::tuple<position, position, position>, OPT*>> children;
	std::map<PlayerColor, int> numOfChildren;
};



#endif

