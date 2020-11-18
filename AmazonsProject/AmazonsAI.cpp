#include "AmazonsAI.h"

#include <climits>


AmazonsAI::AmazonsAI(PlayerColor mColor, bool nStrategy)
	:
	currentMap(nullptr),
	player(mColor),
	strategy(nStrategy)
{}

AmazonsAI::~AmazonsAI()
{
	currentMap = nullptr;
}


std::tuple<position, position, position> AmazonsAI::BestMove()
{
	PlayerColor otherPlayer = (PlayerColor::BLACK == this->player) ? PlayerColor::WHITE : PlayerColor::BLACK ;
	std::vector<position> mPositions = currentMap->playerPositions[this->player];
	const std::vector<position>& oPositions = currentMap->playerPositions[otherPlayer];

	///		 Opponent scope		 My scope		move:	   start	 end	   block
	std::map<int,		std::map<int,			std::tuple<position, position, position>>> minMax;
	///		 My scope			 Opponent scope		move:	   start	 end	   block
	std::map<int,		std::map<int,				std::tuple<position, position, position>>> maxMin;

	for (auto& st : mPositions)
	{
		std::vector<position> posMoves = currentMap->VisibleCellsFromHere(st);

		for (auto& mo : posMoves)
		{
			AmazonMap copy(*currentMap);
			copy.MakeTheMove(std::tuple(st, mo));
			std::vector<position> posBlocks = copy.VisibleCellsFromHere(mo);

			for (auto& bl : posBlocks)
			{
				copy.Block(bl);
				
				int mScope = copy.GetScope(this->player);
				int oScope = copy.GetScope(otherPlayer);

				minMax[oScope][mScope] = std::tuple(st, mo, bl);
				minMax[mScope][oScope] = std::tuple(st, mo, bl);
			}
		}
	}

	/// Choose based on the strategy
	/// min op scope / max my scope
	/// max my scope / min op scope

	std::tuple<position, position, position> res;

	/// min/max
	if (true == strategy)
	{
		res = minMax.begin()->second.rbegin()->second;
	}
	/// max/min
	else
	{
		res = maxMin.begin()->second.rbegin()->second;
	}

	return res;
}


void AmazonsAI::PassTheMap(AmazonMap* nMap)
{
	currentMap = nMap;
}


void AmazonsAI::MakeBestMove()
{
	auto [st, mo, bl] = this->BestMove();

	currentMap->MakeTheMove(std::tuple(st, mo));
	currentMap->Block(bl);
	currentMap->SwitchPlayer();
}

