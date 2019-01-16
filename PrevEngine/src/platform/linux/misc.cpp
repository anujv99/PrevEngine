#include "pch.h"
#include "misc.h"

namespace prev { namespace linux {

	void ChangeConsoleColor(COLORS color) {
		printf("\033[1;%dm", color);
	}

    void ChangeConsoleColorToNormal() {
        printf("\033[0m");
    }

} }