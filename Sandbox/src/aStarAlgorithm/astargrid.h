#pragma once

#include "astarnode.h"

#define DIAGONAL_COST	14
#define LINEAR_COST		10

class AStarGrid {
public:
	AStarGrid(unsigned int xSize, unsigned int ySize);
	~AStarGrid();
public:
	unsigned int GetDirectDistance(glm::vec2 nodeA, glm::vec2 nodeB);
	std::vector<AStarNode *> GetNeighbours(const AStarNode * node);
public:
	inline AStarNode * operator[](unsigned int __index) { return m_NodeArray[__index]; }
	inline unsigned int GetXSize() const { return m_XSize; }
	inline unsigned int GetYSize() const { return m_YSize; }
private:
	unsigned int m_XSize;
	unsigned int m_YSize;

	AStarNode ** m_NodeArray;
};
