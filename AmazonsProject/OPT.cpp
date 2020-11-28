#include "OPT.h"

#include <cstdlib>
#include <ctime>


std::map<PlayerColor, OutcomeClass> pcTOoc = {
	{	PlayerColor::BLACK,		OutcomeClass::LEFT		},
	{	PlayerColor::WHITE,		OutcomeClass::RIGHT		}
};

OPT::OPT(AmazonMap cMap, int maxLevel, int nLevel)
	:
	moc(OutcomeClass::UNDETERMINED),
	level(nLevel),
	finished(true),
	children(),
	numOfChildren()
{
	if (level > maxLevel)
	{
		/// The game tree is too large:
		/// feel free to delete me
	}
	else
	{
		PlayerColor mPlayer = cMap.GetCurrentPlayer();
		PlayerColor otherPlayer = (PlayerColor::BLACK == mPlayer) ? PlayerColor::WHITE : PlayerColor::BLACK;
		std::vector<position> mPositions = cMap.playerPositions[mPlayer];
		std::vector<position> oPositions = cMap.playerPositions[otherPlayer];

		std::map<PlayerColor, bool> ocResult;

		for (auto& st : mPositions)
		{
			std::vector<position> posMoves = cMap.VisibleCellsFromHere(st);

			for (auto& mo : posMoves)
			{
				AmazonMap copy(cMap);
				copy.MakeTheMove(std::tuple(st, mo), mPlayer);
				std::vector<position> posBlocks = copy.VisibleCellsFromHere(mo);

				for (auto& bl : posBlocks)
				{
					copy.Block(bl);
					OPT* smbChild = new OPT(copy, maxLevel, level + 1);
					children[mPlayer][std::tuple(st, mo, bl)] = smbChild;
					if (pcTOoc[mPlayer] == smbChild->moc || OutcomeClass::PREVIOUS_SECOND == smbChild->moc)
					{
						ocResult[mPlayer] = true;
					}
					else if (OutcomeClass::UNDETERMINED == smbChild->moc)
					{
						/// if the result of the child is undetermined
						/// then that child would have height more than
						/// the given max level
						/// 
						/// just return it
						finished = false;
						return;
					}
					++numOfChildren[mPlayer];
				}
			}
		}

		for (auto& st : oPositions)
		{
			std::vector<position> posMoves = cMap.VisibleCellsFromHere(st);

			for (auto& mo : posMoves)
			{
				AmazonMap copy(cMap);
				copy.MakeTheMove(std::tuple(st, mo), otherPlayer);
				std::vector<position> posBlocks = copy.VisibleCellsFromHere(mo);

				for (auto& bl : posBlocks)
				{
					copy.Block(bl);
					OPT* smbChild = new OPT(copy, maxLevel, level + 1);
					children[otherPlayer][std::tuple(st, mo, bl)] = smbChild;
					if (pcTOoc[otherPlayer] == smbChild->moc || OutcomeClass::PREVIOUS_SECOND == smbChild->moc)
					{
						ocResult[otherPlayer] = true;
					}
					else if (OutcomeClass::UNDETERMINED == smbChild->moc)
					{
						/// if the result of the child is undetermined
						/// then that child would have height more than
						/// the given max level
						/// 
						/// just return it
						finished = false;
						return;
					}
					++numOfChildren[otherPlayer];
				}
			}
		}

		if (true == ocResult[PlayerColor::BLACK] && true == ocResult[PlayerColor::WHITE])
		{
			moc = OutcomeClass::NEXT_FIRST;
		}
		if (true == ocResult[PlayerColor::BLACK] && false == ocResult[PlayerColor::WHITE])
		{
			moc = OutcomeClass::LEFT;
		}
		if (false == ocResult[PlayerColor::BLACK] && true == ocResult[PlayerColor::WHITE])
		{
			moc = OutcomeClass::RIGHT;
		}
		if (false == ocResult[PlayerColor::BLACK] && false == ocResult[PlayerColor::WHITE])
		{
			moc = OutcomeClass::PREVIOUS_SECOND;
		}
	}
}

std::tuple<position, position, position> OPT::BestMove(PlayerColor tCol)
{
	/// if this tree isn't my color to win
	/// or the first player doesn't win from here
	/// choose a random move
	if (pcTOoc[tCol] != this->moc && OutcomeClass::NEXT_FIRST != this->moc)
	{
		std::srand(std::time(nullptr));
		int moveNum = std::rand() % (children[tCol].size());
		for (auto& [move, _] : children[tCol])
		{
			if (0 == moveNum)
			{
				return move;
			}
			--moveNum;
		}
	}

	/// otherwise go through the children
	/// and return the move for the child with
	/// PREVIOUS or same color Outcome
	/// (this is guaranteed to happen)
	for (auto& [move, child] : children[tCol])
	{
		if (pcTOoc[tCol] == child->moc || OutcomeClass::PREVIOUS_SECOND == child->moc)
		{
			return move;
		}
	}
}

