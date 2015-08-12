#include "player.h"

#define SPRITE_WIDTH 1
#define SPRITE_HEIGHT 1
#define RIGHT 1
#define LEFT -1

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

	alive = true;
	player = create_sprite((double)x_player, (double)y_player, 
			SPRITE_WIDTH, SPRITE_HEIGHT, alive_img);
}

void draw_player() {
	draw_sprite(player);
}

bool update_player(int key) {
	if (key == 'A' || key == 'a') {
		if (player->x <= 0) {
			player->x = 0;
		} else {
			player->x += LEFT;
		}
		return true;
	} else if (key == 'D' || key == 'd') {
		if (player->x >= screen_width() - 1) {
			player->x = screen_width() - 1;
		} else {
			player->x += RIGHT;
		}
		return true;
	} else if (key == 'S' || key == 's') {

	}
	return false;
}

int get_score() {
	return score;
}

int get_lives() {
	return lives;
}

bool is_alive() {
	if (lives > 0) {
		alive = true;
	} else {
		alive = false;
	}
	return alive;
}

void reset_player() {
	int x_player = screen_width() / 2;
	int y_player = screen_height() * 78 / 100;

	score = 0;
	lives = 3;
	alive = true;
	player->x = x_player;
	player->y = y_player;
}