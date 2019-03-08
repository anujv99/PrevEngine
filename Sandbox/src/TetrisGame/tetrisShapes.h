#pragma once

namespace shapes {

	struct Node {
		glm::vec2 m_Offset = { 0, 0 };
	};

	struct Square {
		friend void CreateShapes();
	private:
		static void CreateShape() {
			m_Shape[1].m_Offset = { 1, 0 };
			m_Shape[2].m_Offset = { 0, 1 };
			m_Shape[3].m_Offset = {-1, 0 };
			m_StartIndex		= { 4, 1 };
		}
	public:
		static glm::vec2 m_StartIndex;
		static Node m_Shape[4]; //index 0 is topmost
	};

	struct L {
		friend void CreateShapes();
	private:
		static void CreateShape() {
			m_Shape[1].m_Offset = { 0,-1 }; // 1, 0 || 0, 1 || -1, 0
			m_Shape[2].m_Offset = { 0,-1 };	// 1, 0	|| 0, 1 || -1, 0
			m_Shape[3].m_Offset = {-1, 0 };	// 0,-1	|| 1, 0 ||  0, 1
			m_StartIndex		= { 6, 2 };
			Rotate();
			Rotate();
		}
	public:
		static glm::vec2 m_StartIndex;
		static Node m_Shape[4]; //index 0 is topmost
	public:
		static void Rotate() {
			for (unsigned int i = 0; i < 4; i++) {
				if (m_Shape[i].m_Offset.x != 0) {
					m_Shape[i].m_Offset.y = m_Shape[i].m_Offset.x;
					m_Shape[i].m_Offset.x = 0;
				} else {
					m_Shape[i].m_Offset.x = -m_Shape[i].m_Offset.y;
					m_Shape[i].m_Offset.y = 0;
				}
			}
		}
	};

	struct LReverse {
		friend void CreateShapes();
	private:
		static void CreateShape() {
			m_Shape[1].m_Offset = { 0,-1 };
			m_Shape[2].m_Offset = { 0,-1 };
			m_Shape[3].m_Offset = { 1, 0 };
			m_StartIndex		= { 4, 2 };
		}
	public:
		static glm::vec2 m_StartIndex;
		static Node m_Shape[4]; //index 0 is topmost
	};

	static void CreateShapes() {
		Square::CreateShape();
		L::CreateShape();
		LReverse::CreateShape();
	}

	Node Square::m_Shape[];
	glm::vec2 Square::m_StartIndex;

	Node L::m_Shape[];
	glm::vec2 L::m_StartIndex;

	Node LReverse::m_Shape[];
	glm::vec2 LReverse::m_StartIndex;

}
