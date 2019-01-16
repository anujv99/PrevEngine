#pragma once

namespace prev { namespace linux {

	enum COLORS {
		BLUE		= 34,
		GREEN		= 32,
		CYAN		= 36,
		RED			= 31,
		MAGENTA		= 35,
		YELLOW		= 33,
		WHITE		= 37,

		RED_OVER	= 41
	};

	void ChangeConsoleColor(COLORS color);
    void ChangeConsoleColorToNormal();
} }
