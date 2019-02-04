#pragma once

namespace prev {

	class Tiles {
	public:
		Tiles(unsigned int xSize, unsigned int ySize);
		~Tiles();
	public:
		glm::vec2 GetTilePosition(int x, int y) const;
		inline glm::vec2 GetTileSize() const { return m_TileSize; }
	private:
		glm::ivec2 m_Divisions;
		glm::vec2 m_TileSize;
	};

}