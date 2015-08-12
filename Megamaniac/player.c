#include "player.h"

#define SPRITE_WIDTH 1
#define SPRITE_HEIGHT 1

typedef char *string;

sprite_id player;
bool alive;
string alive_img = "$";
string dead_img = "_";

int score = 0;
int lives = 3;

void setup_player() {
	int x_player = screen_width() / 2;
	int y_player = screen_height() * 78 / 100;

	player = create_sprite((double)x_player, (double)y_player, 
			SPRITE_WIDTH, SPRITE_HEIGHT, alive_img);
}

void draw_player() {
	draw_sprite(player);
}

int get_score() {
	return score;
}

int get_lives() {
	return lives;
}