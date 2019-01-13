#include <map>
#include <stdexcept>
#include <queue>
#include <iostream>

#include "board.h"

using namespace std;
using namespace boost::numeric::ublas;

Goban::Goban()
{
	boardSize = 19;
	board = matrix<int>(boardSize, boardSize, 0);
}

Goban::Goban(int boardSize)
{	
	if (find(validBoardSizes.begin(), validBoardSizes.end(), boardSize) == validBoardSizes.end())
	{
		throw invalid_argument("Board size must be 9, 13 or 19.");
	}
	this->boardSize = boardSize;
	board = matrix<int>(boardSize, boardSize, 0);
}

Goban::Goban(Goban& goban)
{
	board = goban.board;
	boardSize = goban.getBoardSize();
}

void Goban::placeStone(int stone, int row, int column)
{	
	if (isInvalidStone(stone))
	{
		throw std::invalid_argument("Stone is not valid.");
	}

	if (isNotInRange(row, column))
	{
		throw std::logic_error("Stone coordinates is not in range.");
	}
	
	board (row, column) = stone;
}

void Goban::placeStone(int stone, std::tuple<int, int> index)
{
	int row = std::get<0>(index);
	int column = std::get<1>(index);
	Goban::placeStone(stone, row, column);
}

int Goban::getBoardSize()
{
    return boardSize;
}

void Goban::operator=(const Goban & goban)
{
	boardSize = goban.boardSize;
	board = goban.board;
}

std::vector<std::tuple<int, int>> Goban::getGroup(int row, int column)
{
	std::queue<std::tuple<int, int>> neighborQueue;
	std::vector<std::tuple<int, int>> stoneGroup;
	getAdjacentNeighborsAndPush(row, column, neighborQueue, stoneGroup);

	while (!neighborQueue.empty())
	{	
		std::tuple<int, int> neighbor = neighborQueue.front();
		stoneGroup.push_back(neighbor);

		int neighborRow = std::get<0>(neighbor);
		int neighborColumn = std::get<1>(neighbor);

		getAdjacentNeighborsAndPush(neighborRow, neighborColumn, neighborQueue, stoneGroup);
		neighborQueue.pop();
	}
	return stoneGroup;
}

void Goban::getAdjacentNeighborsAndPush(int row, int column, std::queue<std::tuple<int, int>>& queue, std::vector<std::tuple<int, int>> group)
{
	std::vector<std::tuple<int, int>> adjacentNeighbors = getNeighbors(row, column);
	for (auto it = adjacentNeighbors.begin(); it != adjacentNeighbors.end(); ++it)
	{
		if (notInGroup(group, *it))
		{
			queue.push(*it);
		}
	}
}

bool notInGroup(std::vector<std::tuple<int, int>>& groupElements, std::tuple<int, int> stoneIndex)
{
	const auto position = std::find(groupElements.begin(), groupElements.end(), stoneIndex);
	if (position == groupElements.end())
	{
		return true;
	}
	return false;
}

int Group::getLiberties()
{
    return liberties;
}

bool Group::isCaptured()
{
    if (liberties == 0)
    {
        return true;
    }
    return false;
}

bool isInvalidStone(int stone)
{
	return stone != BLACK && stone != WHITE && stone == EMPTY;
}

std::vector<std::tuple<int, int>> Goban::getNeighbors(int row, int column)
{
	std::vector<std::tuple<int, int>> neighbors = {
		std::make_tuple(row - 1, column - 1),
		std::make_tuple(row + 1, column - 1),
		std::make_tuple(row - 1, column + 1),
		std::make_tuple(row + 1, column + 1),
	};

	std::vector<std::tuple<int, int>> validNeighbors = {};
		
	int boardSize = getBoardSize();

	for (auto it = neighbors.begin(); it != neighbors.end(); ++it)
	{
		int neighborRow = std::get<0>(*it);
		int neighborColumn = std::get<1>(*it);
		
		if (isNotInRange(neighborRow, neighborColumn) && (board(row, column) == board(neighborRow, neighborColumn)))
		{
			validNeighbors.push_back(*it);
		}
	}
	return validNeighbors;
}

bool Goban::isNotInRange(int row, int column)
{
	bool withinRow = (row >= 0 && row < getBoardSize());
	bool withinColumn = (column >= 0 && row < getBoardSize());

	if (withinRow && withinColumn)
	{
		return false;
	}
	return true;
}

void Goban::displayBoard()
{
	for (int i = 0; i < board.size1(); ++i)
	{
		for (int j = 0; j < board.size2(); ++j)
		{
			std::cout << board(i, j);
		}
		std::cout << std::endl;
	}
}
