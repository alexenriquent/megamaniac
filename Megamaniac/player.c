#include "player.h"

#define SPRITE_WIDTH 1
#define SPRITE_HEIGHT 1
#define RIGHT 1
#define LEFT -1
#define INTERVAL 35
#define SCORE_PER_ALIEN 30
#define SCORE_PER_LEVEL 500

typedef char *string;

sprite_id player;
sprite_id bullet;
bool alive;
string alive_img = "$";
string dead_img = "_";
string bullet_img = "|";
bool shooting = false;
int score = 3;
int lives = 1;

void setup_player() {
	int x_player = screen_width() / 2;
	int y_player = screen_height() * 78 / 100;

	alive = true;
	player = create_sprite((double)x_player, (double)y_player, 
			SPRITE_WIDTH, SPRITE_HEIGHT, alive_img);
	reset_player();
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
		if (!shooting) {
			shoot_player_bullet();
			shooting = false;
		} 
		return true;
	}

	return false;
}

void shoot_player_bullet() {
	int key = get_char();
	shooting = true;
	bullet = create_sprite((double)player->x, (double)player->y, 
					SPRITE_WIDTH, SPRITE_HEIGHT, bullet_img);

	while (bullet->y > 0) {
		clear_screen();
		update_player(key);
		update_aliens();
		draw_screen();
		draw_player();
		draw_aliens();
		draw_alien_bullets();
		update_player_bullet();
		draw_sprite(bullet);

		if (get_level() == 5) {
			draw_aggressive_alien();
		}

		show_screen();
		key = get_char();
		timer_pause(INTERVAL);
	}
}

void update_player_bullet() {
	bullet->dy = -1;
	bullet->y += bullet->dy;

	if (get_screen_char(bullet->x, bullet->y) == '@') {
		bullet->is_visible = false;
		change_alien_status(bullet->x, bullet->y);
		bullet->y = 0;
		score += SCORE_PER_ALIEN;
		if (alive_aliens_count() == 0) {
			score += SCORE_PER_LEVEL;
			level_up();
			reset_aliens();
		}
	}
}

void reset_player() {
	int x_player = screen_width() / 2;
	int y_player = screen_height() * 78 / 100;

	score = 0;
	lives = 3;
	alive = true;
	player->is_visible = true;
	player->x = x_player;
	player->y = y_player;
}

void reset_player_location() {
	int x_player = screen_width() / 2;
	int y_player = screen_height() * 78 / 100;

	player->x = x_player;
	player->y = y_player;
	
	while (get_screen_char(player->x, player->y) == '@') {
		int random = rand() % screen_width();
		player->x = x_player;
		player->y = random;
	} 
}

int x_pos() {
	int x = player->x;
	return x;
}

int y_pos() {
	int y = player->y;
	return y;
}

int get_score() {
	return score;
}

int get_lives() {
	return lives;
}

void update_lives() {
	lives--;
}

bool is_alive() {
	return alive;
}

void check_player_alive() {
	if (lives > 0) {
		alive = true;
	} else {
		alive = false;
	}
}

void change_player_status() {
	player->is_visible = false;
}

void cleanup_player() {
	destroy_sprite(player);
}