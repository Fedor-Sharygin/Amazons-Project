#include "AmazonMap.h"

#include <string>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <sstream>






std::map<CellStatus, PlayerColor> AmazonMap::coherence =
{
	{	CellStatus::BLACK,	PlayerColor::BLACK	},
	{	CellStatus::WHITE,	PlayerColor::WHITE	}
};
std::map<PlayerColor, CellStatus> AmazonMap::swCoherence =
{
	{	PlayerColor::BLACK,	CellStatus::BLACK	},
	{	PlayerColor::WHITE,	CellStatus::WHITE	}
};




AmazonMap::AmazonMap(int nWidth, int nHeight, PlayerColor nPM)
	:
	width(nWidth), height(nHeight),
	currentPlayerMove(nPM),
	lastMove(-1, -1)
{
	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			map[i][j] = CellStatus::EMPTY;
		}
	}
}

AmazonMap::AmazonMap(const AmazonMap& lhs)
	:
	width(lhs.width), height(lhs.height),
	currentPlayerMove(lhs.currentPlayerMove),
	lastMove(lhs.lastMove),
	playerPositions(lhs.playerPositions)
{
	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			map[i][j] = lhs.map.at(i).at(j);
		}
	}
}


/// <summary>
/// VisibleCellsFromHere takes in the coordinates of a cell
/// and returns vector of nodes that can be reached from here
/// 
/// will be used to analyze available moves for players:
/// 1st - where can the node in this cell move to
/// 2nd - what nodes can the node block from here
/// </summary>
/// <param name="row">
/// row of the cell from which we are checking the visibility
/// </param>
/// <param name="col">
/// column of the cell from which we are checking the visibility
/// </param>
/// <returns>
/// vector of cell positions which can be reached from the row-col cell
/// </returns>
std::vector<position> AmazonMap::VisibleCellsFromHere(int row, int col)
{
	std::vector<position> filler;

	/// if a previous cell was is not in the list
	/// then get don't put this cell in
	/// as well as don't add this cell if it's not empty

	int leftLayers			=	col;
	int rightLayers			=	this->width - col - 1;
	int topLayers			=	this->height - row - 1;
	int bottomLayers		=	row;
	int bottomLeftLayers	=	std::min( leftLayers,	bottomLayers );
	int topLeftLayers		=	std::min( leftLayers,	topLayers );
	int bottomRightLayers	=	std::min( rightLayers,	bottomLayers );
	int topRightLayers		=	std::min( rightLayers,	topLayers );


	/// Check horizontal lines (rows)

	for (int layer = 0; layer < leftLayers; ++layer)
	{
		int nCol = col - 1 - layer;
		if (CellStatus::EMPTY != map[row][nCol])
		{
			break;
		}

		filler.push_back(std::make_pair(row, nCol));
	}

	for (int layer = 0; layer < rightLayers; ++layer)
	{
		int nCol = col + 1 + layer;
		if (CellStatus::EMPTY != map[row][nCol])
		{
			break;
		}

		filler.push_back(std::make_pair(row, nCol));
	}


	/// Check vertical lines (columns)

	for (int layer = 0; layer < topLayers; ++layer)
	{
		int nRow = row + 1 + layer;
		if (CellStatus::EMPTY != map[nRow][col])
		{
			break;
		}

		filler.push_back(std::make_pair(nRow, col));
	}

	for (int layer = 0; layer < bottomLayers; ++layer)
	{
		int nRow = row - 1 - layer;
		if (CellStatus::EMPTY != map[nRow][col])
		{
			break;
		}

		filler.push_back(std::make_pair(nRow, col));
	}


	/// Check diagonals

	for (int layer = 0; layer < bottomLeftLayers; ++layer)
	{
		int nRow = row - 1 - layer;
		int nCol = col - 1 - layer;
		if (CellStatus::EMPTY != map[nRow][nCol])
		{
			break;
		}

		filler.push_back(std::make_pair(nRow, nCol));
	}

	for (int layer = 0; layer < topLeftLayers; ++layer)
	{
		int nRow = row + 1 + layer;
		int nCol = col - 1 - layer;
		if (CellStatus::EMPTY != map[nRow][nCol])
		{
			break;
		}

		filler.push_back(std::make_pair(nRow, nCol));
	}

	for (int layer = 0; layer < bottomRightLayers; ++layer)
	{
		int nRow = row - 1 - layer;
		int nCol = col + 1 + layer;
		if (CellStatus::EMPTY != map[nRow][nCol])
		{
			break;
		}

		filler.push_back(std::make_pair(nRow, nCol));
	}

	for (int layer = 0; layer < topRightLayers; ++layer)
	{
		int nRow = row + 1 + layer;
		int nCol = col + 1 + layer;
		if (CellStatus::EMPTY != map[nRow][nCol])
		{
			break;
		}

		filler.push_back(std::make_pair(nRow, nCol));
	}


	return filler;
}


std::vector<position> AmazonMap::VisibleCellsFromHere(position mP)
{
	return this->VisibleCellsFromHere(mP.first, mP.second);
}


void AmazonMap::SwitchPlayer()
{
	if (PlayerColor::BLACK == currentPlayerMove)
	{
		currentPlayerMove = PlayerColor::WHITE;
	}
	else
	{
		currentPlayerMove = PlayerColor::BLACK;
	}
}


void AmazonMap::MakeTheMove(std::tuple<position, position> nMove, PlayerColor forOPT)
{
	PlayerColor forCheck;
	if (PlayerColor::UNDEFINED == forOPT)
	{
		forCheck = this->currentPlayerMove;
	}
	else
	{
		forCheck = forOPT;
	}
	auto [start, move] = nMove;

	this->map[move.first][move.second] = this->map[start.first][start.second];
	this->map[start.first][start.second] = CellStatus::EMPTY;

	auto it = std::find(playerPositions[forCheck].begin(), playerPositions[forCheck].end(), start);

	playerPositions[forCheck].erase(it);
	playerPositions[forCheck].push_back(move);
}

void AmazonMap::Block(position nBlock)
{
	this->map[nBlock.first][nBlock.second] = CellStatus::BLOCKED;
}


const int& AmazonMap::GetHeight() const
{
	return height;
}

const int& AmazonMap::GetWidth() const
{
	return width;
}



std::ostream& operator<<(std::ostream& out, const AmazonMap& am)
{
	const int& h = am.height;
	const int& w = am.width;
	for (int cRow = 0; cRow < h; ++cRow)
	{
		for (int cCol = 0; cCol < w; ++cCol)
		{
			CellStatus cCell = am.map.at(cRow).at(cCol);
			if (CellStatus::EMPTY == cCell)
			{
				out << '-';
			}
			if (CellStatus::BLOCKED == cCell)
			{
				out << 'X';
			}
			if (CellStatus::BLACK == cCell)
			{
				out << 'B';
			}
			if (CellStatus::WHITE == cCell)
			{
				out << 'W';
			}
		}
		out << std::endl;
	}

	out << std::endl << std::endl;
	
	return out;
}



int AmazonMap::GetScope(PlayerColor nCol)
{
	std::vector<position> scope;
	for (auto& blPiece : playerPositions[nCol])
	{
		std::vector<position> visPos = this->VisibleCellsFromHere(blPiece);
		scope.insert(scope.end(), visPos.begin(), visPos.end());
	}

	return scope.size();
}


///	width = val, height = val
///	player = player color
/// the map
AmazonMap::AmazonMap(std::string textMap)
	:
	lastMove(-1, -1)
{
	if (true == textMap.empty())
	{
		std::cout << "ERROR: empty text passed" << std::endl;
		exit(-1);
	}

	std::stringstream ss(textMap);
	std::string store;

	std::vector<std::string> lines;

	while (std::getline(ss, store, '\n'))
	{
		lines.push_back(store);
	}

	std::stringstream sizeStream(lines[0]);
	int nWidth;
	sizeStream.ignore(10, ' ');
	sizeStream.ignore(2);
//	EnterNumToChar(sizeStream, nWidth, ',');
	std::string nn;
	std::getline(sizeStream, nn, ',');
	std::stringstream ss1(nn);
	ss1 >> nWidth;
	width = nWidth;

	int nHeight;
	sizeStream.ignore(10, '=');
	sizeStream.ignore();
//	EnterNumToChar(sizeStream, nHeight);
	std::string nn2;
	std::getline(sizeStream, nn2);
	std::stringstream ss2(nn2);
	ss2 >> nHeight;
	height = nHeight;


	std::stringstream plStream(lines[1]);
	std::string ncName;
	PlayerColor nCol;
	plStream.ignore(10, ' ');
	plStream.ignore(2);
	plStream >> ncName;
	if (0 == ncName.compare("black"))
	{
		nCol = PlayerColor::BLACK;
	}
	if (0 == ncName.compare("white"))
	{
		nCol = PlayerColor::WHITE;
	}
	currentPlayerMove = nCol;
	for (int i = 2; i < lines.size(); ++i)
	{
		for (int c = 0; c < width; ++c)
		{
			char ch = lines[i][c];

			if ('-' == ch)
			{
				map[i - 2][c] = CellStatus::EMPTY;
			}
			if ('X' == ch)
			{
				map[i - 2][c] = CellStatus::BLOCKED;
			}
			if ('B' == ch)
			{
				map[i - 2][c] = CellStatus::BLACK;
				playerPositions[PlayerColor::BLACK].push_back(position(i - 2, c));
			}
			if ('W' == ch)
			{
				map[i - 2][c] = CellStatus::WHITE;
				playerPositions[PlayerColor::WHITE].push_back(position(i - 2, c));
			}
		}
	}
}



PlayerColor AmazonMap::GetCurrentPlayer()
{
	return currentPlayerMove;
}


