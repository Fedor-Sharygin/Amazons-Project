#include "AmazonsAI.h"

#include <climits>


AmazonsAI::AmazonsAI(PlayerColor mColor)
	:
	currentMap(nullptr),
	player(mColor)
{}

AmazonsAI::~AmazonsAI()
{
	currentMap = nullptr;
}


std::tuple<position, position, position> AmazonsAI::BestMove()
{
	PlayerColor otherPlayer = (PlayerColor::BLACK == this->player) ? PlayerColor::WHITE : PlayerColor::BLACK ;
	std::set<position> mPositions = currentMap->playerPositions[this->player];
	const std::set<position>& oPositions = currentMap->playerPositions[otherPlayer];

	position bestStart	(-1, -1);
	position bestMove	(-1, -1);
	position bestBlock	(-1, -1);

	int myMaxRange = INT_MIN;
	int otherMinRange = INT_MAX;

	AmazonMap copy(*currentMap);

	for (auto& st : mPositions)
	{
		std::vector<position> posMoves = currentMap->VisibleCellsFromHere(st);

		for (auto& mo : posMoves)
		{
			
			std::vector<position> posBlocks = copy.VisibleCellsFromHere(mo);

			for (auto& bl : posBlocks)
			{

			}
		}
	}

	return std::tuple(bestStart, bestMove, bestBlock);
}


void AmazonsAI::PassTheMap(AmazonMap* nMap)
{
	currentMap = nMap;
}


