#include "pch.h"
#include "astargrid.h"

AStarGrid::AStarGrid(unsigned int xSize, unsigned int ySize) :
	m_XSize(xSize), m_YSize(ySize), m_NodeArray(nullptr) {

	m_NodeArray = new AStarNode * [xSize];
	for (unsigned int i = 0; i < xSize; i++) {
		m_NodeArray[i] = new AStarNode[ySize];
	}

	for (unsigned int i = 0; i < xSize; i++) {
		for (unsigned int j = 0; j < ySize; j++) {
			m_NodeArray[i][j].m_GridPos = { i ,j };
		}
	}
}

AStarGrid::~AStarGrid() {
	for (unsigned int i = 0; i < m_XSize; i++) {
		delete[] m_NodeArray[i];
	}
	delete[] m_NodeArray;
}

unsigned int AStarGrid::GetDirectDistance(glm::vec2 nodeA, glm::vec2 nodeB) {
	if (nodeA.x >= m_XSize || nodeA.y >= m_YSize || nodeB.x >= m_XSize || nodeB.y >= m_YSize)
		return 0;

	int x = std::abs(nodeB.x - nodeA.x);
	int y = std::abs(nodeB.y - nodeA.y);

	if (x > y)
		return (DIAGONAL_COST * y) + (LINEAR_COST * (x - y));
	else
		return (DIAGONAL_COST * x) + (LINEAR_COST * (y - x));
}

std::vector<AStarNode *> AStarGrid::GetNeighbours(const AStarNode * node) {
	std::vector<AStarNode *> neighbours;

	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			if (i == 0 && j == 0)
				continue;

			int checkX = node->m_GridPos.x + i;
			int checkY = node->m_GridPos.y + j;

			if (checkX >= 0 && checkX < m_XSize && checkY >= 0 && checkY < m_YSize)
				neighbours.push_back(&m_NodeArray[checkX][checkY]);
		}
	}

	return neighbours;
}
