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
string bullet_img = ".";

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
	} 
	if (key == 'S' || key == 's') {
		shoot_bullet();
		return true;
	}
	return false;
}

void shoot_bullet() {
	int key;
	sprite_id bullet = create_sprite((double)player->x, (double)player->y, 
					SPRITE_WIDTH, SPRITE_HEIGHT, bullet_img);
	while (bullet->y > 0) {
		clear_screen();
		update_player(key);
		draw_screen();
		draw_player();
		update_bullet(bullet);
		draw_sprite(bullet);
		show_screen();
		key = get_char();
		timer_pause(35);
	}
}

void update_bullet(sprite_id shot) {
	shot->dy = -1;
	shot->y += shot->dy;

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