#include "pch.h"
#include "misc.h"

namespace prev { namespace windows {

	void ChangeConsoleColor(COLORS color) {
		static HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hConsole, color);
	}

} }
