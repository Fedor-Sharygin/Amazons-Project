#include "AmazonMap.h"


AmazonMap::AmazonMap(int nWidth, int nHeight)
	:
	width(nWidth), height(nHeight)
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


/// <summary>
/// VisibleCellsFromHere takes in the coordinates of a cell
/// and returns vector of nodes that can be reached from here
/// 
/// will be used to analyze available moves for players:
/// 1st - where can the node in this cell move to
/// 2nd - what nodes can the node block from here
/// </summary>
/// <param name="row"></param>
/// <param name="col"></param>
/// <returns></returns>
std::vector<std::pair<int, int>> AmazonMap::VisibleCellsFromHere(int row, int col)
{

}
