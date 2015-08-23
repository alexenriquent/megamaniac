#include <stdlib.h>
#include "cab202_graphics.h"
#include "game.h"

#define UPDATE_SCREEN 1
#define NORMAL_MODE 0
#define GAME_OVER -1

int main() {

	int mode = NORMAL_MODE;

	setup_game();

	while (mode != GAME_OVER) {
		mode = play_game();
		if (mode == UPDATE_SCREEN) {
			update_game();
		} 
	}

	cleanup_game();

	return EXIT_SUCCESS;
}
