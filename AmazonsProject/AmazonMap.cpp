#include "AmazonMap.h"


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
	currentPlayerMove(nPM)
{
	map = new CellStatus*[height];
	for (int i = 0; i < height; ++i)
	{
		map[i] = new CellStatus[width];
		for (int j = 0; j < width; ++j)
		{
			map[i][j] = CellStatus::EMPTY;
		}
	}
}

AmazonMap::AmazonMap(const AmazonMap& lhs)
	:
	width(lhs.width), height(lhs.height),
	currentPlayerMove(lhs.currentPlayerMove)
{
	map = new CellStatus * [height];
	for (int i = 0; i < height; ++i)
	{
		map[i] = new CellStatus[width];
		for (int j = 0; j < width; ++j)
		{
			map[i][j] = lhs.map[i][j];
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
std::vector<std::pair<int, int>> AmazonMap::VisibleCellsFromHere(int row, int col)
{
	std::vector<std::pair<int, int>> filler;

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


std::vector<std::pair<int, int>> AmazonMap::VisibleCellsFromHere(position mP)
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


void AmazonMap::UpdatePlayerPositions()
{
	for (auto [_, posSet] : playerPositions)
	{
		posSet.clear();
	}

	for (int cRow = 0; cRow < this->height; ++cRow)
	{
		for (int cCol = 0; cCol < this->width; ++cCol)
		{
			const CellStatus& curStatus = this->map[cRow][cCol];
			if (CellStatus::EMPTY != curStatus && CellStatus::BLOCKED != curStatus)
			{
				playerPositions[coherence[curStatus]].insert(position(cRow, cCol));
			}
		}
	}
}


void AmazonMap::MakeTheMove(std::tuple<position, position, position> nMove)
{
	auto [start, move, block] = nMove;

	this->map[move.first][move.second] = this->map[start.first][start.second];
	this->map[start.first][start.second] = CellStatus::EMPTY;
	this->map[block.first][block.second] = CellStatus::BLOCKED;

	this->SwitchPlayer();
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
			CellStatus cCell = am.map[cRow][cCol];
			out << '\t';
			if (CellStatus::EMPTY == cCell)
			{
				out << '*';
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
	
	return out;
}

