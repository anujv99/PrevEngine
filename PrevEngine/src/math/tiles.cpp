#include "pch.h"
#include "tiles.h"

#include "application.h"
#include "math.h"

namespace prev {

	Tiles::Tiles(unsigned int xSize, unsigned int ySize) :
		m_TileSize(xSize, ySize) {
		m_TileSize *= Math::GetWorldScale();
	}

	Tiles::~Tiles() {}

	glm::vec2 Tiles::GetTilePosition(int x, int y) const {
		glm::vec2 basePosition = { x * m_TileSize.x, y * m_TileSize.y };
		basePosition += m_TileSize / 2.0f;
		return basePosition;
	}

}