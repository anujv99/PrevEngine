#include "pch.h"
#include "astart.h"

#include "astarvisualizer.h"

using namespace prev;

//credits :- Sebastian Lague https://www.youtube.com/channel/UCmtyQOKKmrMVaKuRXz02jbQ

void RetracePath(AStarNode * startNode, AStarNode * endNode) {
	std::vector<AStarNode *> path;
	AStarNode * currentNode = endNode;

	while (currentNode != startNode) {
		path.push_back(currentNode);
		currentNode = currentNode->m_Parent;
	}

	AStarVisualizer::ShowPath(path);
}

static void GetShortestPath(AStarGrid & grid, glm::ivec2 startPos, glm::ivec2 endPos) {

	if (startPos.x >= grid.GetXSize() ||
		startPos.y >= grid.GetYSize() ||
		endPos.x >= grid.GetXSize() ||
		endPos.y >= grid.GetYSize()) {
		PV_ERROR("Positions OUT_OF_BOUNDS");
		return;
	}

	std::vector<AStarNode *> openSet;
	std::vector<AStarNode *> closedSet;

	AStarNode * startNode = &grid[startPos.x][startPos.y];
	AStarNode * endNode = &grid[endPos.x][endPos.y];

	openSet.push_back(&grid[startPos.x][startPos.y]);
	
	while (openSet.size() > 0) {
		AStarNode * currentNode = openSet[0];
		for (unsigned int i = 1; i < openSet.size(); i++) {
			if (openSet[i]->GetFCost() < currentNode->GetFCost() ||
				openSet[i]->GetFCost() == currentNode->GetFCost() &&
				openSet[i]->m_HCost < currentNode->m_HCost) {
				currentNode = openSet[i];
			}
		}

		openSet.erase(std::remove(openSet.begin(), openSet.end(), currentNode), openSet.end());
		closedSet.push_back(currentNode);

		if (currentNode == endNode) {
			RetracePath(startNode, endNode);
			return;
		}

		auto neighbours = grid.GetNeighbours(currentNode);

		for (AStarNode * neighbourNode : neighbours) {
			if (!neighbourNode->m_IsWalkable || std::find(closedSet.begin(), closedSet.end(), neighbourNode) != closedSet.end())
				continue;

			int newMovedCostToNeighbour = currentNode->m_GCost + grid.GetDirectDistance(currentNode->GetPos(), neighbourNode->GetPos());

			if (newMovedCostToNeighbour < neighbourNode->m_GCost ||
				std::find(openSet.begin(), openSet.end(), neighbourNode) == openSet.end()) 
			{
				neighbourNode->m_GCost = newMovedCostToNeighbour;
				neighbourNode->m_HCost = grid.GetDirectDistance(neighbourNode->GetPos(), endNode->GetPos());
				neighbourNode->m_Parent = currentNode;

				if (std::find(openSet.begin(), openSet.end(), neighbourNode) == openSet.end()) {
					openSet.push_back(neighbourNode);
				}
			}
		}
	}
}

AStarGrid newGrid(20, 20);

void AStar::Init(prev::Application * app) {

	for (int i = 0; i < 6; i++) {
		newGrid[4][i].m_IsWalkable = false;
	}

	GetShortestPath(newGrid, { 0, 0 }, { 5, 5 });
}
