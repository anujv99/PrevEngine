#pragma once

#include <prev.h>

class AStarNode {
	friend class AStarGrid;
public:
	AStarNode(unsigned int gCost, unsigned int hCost) :
		m_GCost(gCost), m_HCost(hCost), m_IsWalkable(true) {
	}
	AStarNode(bool isWalkable = true) :
		m_GCost(0), m_HCost(0), m_IsWalkable(isWalkable) {
	}
public:
	inline unsigned int GetFCost() const { return m_HCost + m_GCost; }
	inline glm::ivec2 GetPos() const { return m_GridPos; }
public:
	inline bool operator==(const AStarNode & node) {
		if (node.m_GridPos == m_GridPos)
			return true;
		return false;
	}
	inline bool operator!=(const AStarNode & node) {
		if (node.m_GridPos == m_GridPos)
			return false;
		return true;
	}
public:
	unsigned int m_HCost, m_GCost;
	bool m_IsWalkable;
	AStarNode * m_Parent;
private:
	glm::ivec2 m_GridPos;
};
