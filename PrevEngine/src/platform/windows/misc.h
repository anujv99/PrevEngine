#pragma once

namespace prev { namespace windows {

	enum COLORS {
		BLUE		= 9,
		GREEN		= 10,
		CYAN		= 11,
		RED			= 12,
		MAGENTA		= 13,
		YELLOW		= 14,
		WHITE		= 15,

		RED_OVER	= 20,
		NORMAL		= 7
	};

	void ChangeConsoleColor(COLORS color);
} }
