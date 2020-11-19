#include <cstdio>
#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <string>
#include <sstream>

#include "AmazonMap.h"
#include "AmazonsAI.h"


#define ERROR_VALUE() std::cout << "ERROR: improper value. Please, reenter the value" << std::endl;



void PlayerMakeMove(AmazonMap* map)
{
	int stR, stC, moR, moC, blR, blC;
	position start, move, block;

	std::cout << "Enter cell positions: (row, col) (top-bottom, left-right)" << std::endl << std::endl;

	std::cout << "Please, enter the position of the piece you want to move (enter (row, col))" << std::endl;
	EnterNumToChar(std::cin, stR, ',');
	EnterNumToChar(std::cin, stC, ')');
	start = position(stR, stC);

	std::cout << "Please, enter the position where you want to move (enter (row, col))" << std::endl;
	EnterNumToChar(std::cin, moR, ',');
	EnterNumToChar(std::cin, moC, ')');
	move = position(moR, moC);

	map->MakeTheMove(std::tuple(start, move));


	std::cout << "Please, enter the position you want to block (enter (row, col))" << std::endl;
	EnterNumToChar(std::cin, blR, ',');
	EnterNumToChar(std::cin, blC, ')');
	block = position(blR, blC);

	map->Block(block);

	
	map->SwitchPlayer();
}


int main(int argc, char** argv)
{
	int compNum = -1;
	while (true)
	{
		std::cout << "Please, enter the number of AI opponents (max -- 2):" << std::endl;
		std::cin >> compNum;

		if (0 != compNum && 1 != compNum && 2 != compNum)
		{
			ERROR_VALUE();
		}
		else
		{
			break;
		}
	}
	
	AmazonsAI* black = nullptr;
	AmazonsAI* white = nullptr;

	if (0 != compNum)
	{
		std::string mmName;
		bool minMax;
		while (true)
		{
			std::cout << "Please, enter the AI player strategy (minMax or maxMin):" << std::endl;
			std::cin >> mmName;

			if (0 != mmName.compare("minMax") && 0 != mmName.compare("maxMin"))
			{
				ERROR_VALUE();
			}
			else
			{
				break;
			}
		}

		if (0 == mmName.compare("minMax"))
		{
			minMax = true;
		}
		if (0 == mmName.compare("maxMin"))
		{
			minMax = false;
		}

		if (1 == compNum)
		{
			std::string aiColorName;
			while (true)
			{
				std::cout << "Please, enter the color of the AI player (black -- 1st, white -- 2nd):" << std::endl;
				std::cin >> aiColorName;

				if (0 != aiColorName.compare("black") && 0 != aiColorName.compare("white"))
				{
					ERROR_VALUE();
				}
				else
				{
					break;
				}
			}

			if (0 == aiColorName.compare("black"))
			{
				black = new AmazonsAI(PlayerColor::BLACK, minMax);
			}
			if (0 == aiColorName.compare("white"))
			{
				white = new AmazonsAI(PlayerColor::WHITE, minMax);
			}
		}

		if (2 == compNum)
		{
			black = new AmazonsAI(PlayerColor::BLACK, minMax);
			white = new AmazonsAI(PlayerColor::WHITE, minMax);
		}
	}

	std::map<PlayerColor, AmazonsAI*> ai = { { PlayerColor::BLACK, black }, { PlayerColor::WHITE, white } };

	std::string fName;
	std::cout << "Please, enter the name of the text file with the map:" << std::endl;
	std::cin >> fName;

	std::string fileContents;
	std::fstream ifs(fName);
	fileContents.assign( (std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()) );

	AmazonMap mainMap(fileContents);

	if (nullptr != black)
	{
		black->PassTheMap(&mainMap);
	}
	if (nullptr != white)
	{
		white->PassTheMap(&mainMap);
	}

	while (0 != mainMap.GetScope(mainMap.GetCurrentPlayer()))
	{
		std::cout << mainMap;

		std::system("pause");

		AmazonsAI* player = ai[mainMap.GetCurrentPlayer()];

		/// if current player is nullptr
		/// then it's human player's turn
		if (nullptr == player)
		{
			PlayerMakeMove(&mainMap);
		}
		/// otherwise it's the ai's turn
		else
		{
			player->MakeBestMove();
		}
	}

	std::cout << mainMap;

	PlayerColor lastPlayer = mainMap.GetCurrentPlayer();

	if (PlayerColor::BLACK == lastPlayer)
	{
		std::cout << "White";
	}
	if (PlayerColor::WHITE == lastPlayer)
	{
		std::cout << "Black";
	}
	std::cout << " won!" << std::endl;

	std::system("pause");

	return 0;
}

