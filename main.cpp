#include "CGAME.h"

int main() {
	CGAME* Game = CGAME::getGame();
	Game->Run();

	return 0;
}
