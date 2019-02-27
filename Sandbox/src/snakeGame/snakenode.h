#pragma once

#include <prev.h>

class SnakeNode {
public:
	SnakeNode() {}
	SnakeNode(glm::vec2 tilePos, entityx::Entity entity, SnakeNode * nextNode = nullptr) :
		m_TilePos(tilePos), m_Entity(entity), m_NextNode(nextNode) {}
public:
	glm::vec2 m_TilePos		= {-1, -1};
	entityx::Entity m_Entity;
	SnakeNode * m_NextNode	= nullptr;
};

class FoodNode {
public:
	FoodNode() {}
	FoodNode(glm::vec2 tilePos, entityx::Entity entity) :
		m_TilePos(tilePos), m_Entity(entity){}
public:
	glm::vec2 m_TilePos = { -1, -1 };
	entityx::Entity m_Entity;
};