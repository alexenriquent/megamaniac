#include "player.h"

#define SPRITE_WIDTH 1
#define SPRITE_HEIGHT 1
#define RIGHT 1
#define LEFT -1
#define INTERVAL 35
#define SCORE_PER_ALIEN 30
#define SCORE_PER_LEVEL 500
#define FINAL_LEVEL 5
#define KEYPRESS_INTERVAL 600

typedef char *string;

sprite_id player;
sprite_id bullet;
sprite_id curved_bullet;
timer_id keypress_timer;
string alive_img = "$";
string bullet_img = "|";
int score;
int lives;
int key_hold;
bool alive;
bool shooting = false;
bool holding = false;

void setup_player() {
	int x_player = screen_width() / 2;
	int y_player = screen_height() - 6;

	score = 0;
	lives = 3; 
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
	if (get_level() == FINAL_LEVEL) {
		if (key == 'Z' || key == 'z') {
			keypress_timer = create_timer(KEYPRESS_INTERVAL);
			double degree = 90;

			while (!timer_expired(keypress_timer) &&
				!shooting) {
				if (key == 'Z' || key == 'z') {
					reset_timer(keypress_timer);
					degree -= 1.5;
					if (degree <= 0) {
						degree = 0;
					}
					clear_screen();
					update_aliens();
					draw_screen();
					draw_player();
					draw_aliens();
					draw_alien_bullets();
					if (get_level() == FINAL_LEVEL) {
						draw_aggressive_alien();
					}
					show_screen();
				}
				key = get_char();
			}

			if (!shooting) {
				shoot_left_curved_bullet(degree);
				shooting = false;
			} 
		} else if (key == 'C' || key == 'c') {
			keypress_timer = create_timer(KEYPRESS_INTERVAL);
			double degree = 90;
			
			while (!timer_expired(keypress_timer) &&
				!shooting) {
				if (key == 'C' || key == 'c') {
					reset_timer(keypress_timer);
					degree += 1.5;
					if (degree >= 180) {
						degree = 180;
					}
					clear_screen();
					update_aliens();
					draw_screen();
					draw_player();
					draw_aliens();
					draw_alien_bullets();
					if (get_level() == FINAL_LEVEL) {
						draw_aggressive_alien();
					}
					show_screen();
				}
				key = get_char();
			}

			if (!shooting) {
				shoot_right_curved_bullet(degree);
				shooting = false;
			} 
		}
	}

	return false;
}

void shoot_player_bullet() {
	int key = get_char();
	shooting = true;
	bullet = create_sprite((double)player->x, (double)player->y, 
					SPRITE_WIDTH, SPRITE_HEIGHT, bullet_img);
	bullet->dy = -1;

	while (bullet->y > 0) {
		clear_screen();
		update_player(key);
		update_aliens();
		draw_screen();
		draw_player();
		draw_aliens();
		draw_alien_bullets();

		if (get_level() == FINAL_LEVEL) {
			draw_aggressive_alien();
		}

		update_player_bullet();
		draw_sprite(bullet);
		show_screen();
		key = get_char();
		timer_pause(INTERVAL);
	}
}

void update_player_bullet() {
	bullet->y += bullet->dy;

	int bullet_x = (int) round(bullet->x);
	int bullet_y = (int) round(bullet->y);

	if (is_alien(bullet_x, bullet_y)) {
		bullet->is_visible = false;
		change_alien_status(bullet_x, bullet_y);
		bullet->y = 0;
		score += SCORE_PER_ALIEN;
		if (alive_aliens_count() == 0) {
			score += SCORE_PER_LEVEL;
		}
	}

	if (get_level() == FINAL_LEVEL) {
		if (bullet_x == aggressive_alien_x_pos() &&
			bullet_y == aggressive_alien_y_pos()) {
			bullet->is_visible = false;
			change_alien_status(bullet_x, bullet_y);
			bullet->y = 0;
			score += SCORE_PER_ALIEN;
			if (alive_aliens_count() == 0) {
				score += SCORE_PER_LEVEL;
			}
		}
	}
}

void shoot_left_curved_bullet(double degree) {
	int key = get_char();
	shooting = true;
	curved_bullet = create_sprite((double)player->x, (double)player->y, 
					SPRITE_WIDTH, SPRITE_HEIGHT, bullet_img);
	curved_bullet->dy = -1;
	turn_sprite(curved_bullet, degree);


	while (curved_bullet->y > 0 &&
		curved_bullet->x > 0) {
		clear_screen();
		update_player(key);
		update_aliens();
		draw_screen();
		draw_player();
		draw_aliens();
		draw_alien_bullets();
		update_left_curved_bullet();
		draw_sprite(curved_bullet);

		if (get_level() == FINAL_LEVEL) {
			draw_aggressive_alien();
		}

		show_screen();
		key = get_char();
		timer_pause(INTERVAL);
	}
}

void update_left_curved_bullet() {
	curved_bullet->x += curved_bullet->dx;
	curved_bullet->y += curved_bullet->dy;

	int curved_bullet_x = (int) round(curved_bullet->x);
	int curved_bullet_y = (int) round(curved_bullet->y);

	if (is_alien(curved_bullet_x, curved_bullet_y)) {
		curved_bullet->is_visible = false;
		change_alien_status(curved_bullet_x, curved_bullet_y);
		curved_bullet->y = 0;
		score += SCORE_PER_ALIEN;
		if (alive_aliens_count() == 0) {
			score += SCORE_PER_LEVEL;
		}
	}

	if (get_level() == FINAL_LEVEL) {
		if (curved_bullet_x == aggressive_alien_x_pos() &&
			curved_bullet_y == aggressive_alien_y_pos()) {
			curved_bullet->is_visible = false;
			change_alien_status(curved_bullet_x, curved_bullet_y);
			curved_bullet->y = 0;
			score += SCORE_PER_ALIEN;
			if (alive_aliens_count() == 0) {
				score += SCORE_PER_LEVEL;
			}
		}
	}
}

void shoot_right_curved_bullet(double degree) {
	int key = get_char();
	shooting = true;
	curved_bullet = create_sprite((double)player->x, (double)player->y, 
					SPRITE_WIDTH, SPRITE_HEIGHT, bullet_img);
	curved_bullet->dy = -1;
	turn_sprite(curved_bullet, degree);

	while (curved_bullet->y > 0 &&
		curved_bullet->x < screen_width()) {
		clear_screen();
		update_player(key);
		update_aliens();
		draw_screen();
		draw_player();
		draw_aliens();
		draw_alien_bullets();
		update_right_curved_bullet();
		draw_sprite(curved_bullet);

		if (get_level() == FINAL_LEVEL) {
			draw_aggressive_alien();
		}

		show_screen();
		key = get_char();
		timer_pause(INTERVAL);
	}
}

void update_right_curved_bullet() {
	curved_bullet->x += curved_bullet->dx;
	curved_bullet->y += curved_bullet->dy;

	int curved_bullet_x = (int) round(curved_bullet->x);
	int curved_bullet_y = (int) round(curved_bullet->y);

	if (is_alien(curved_bullet_x, curved_bullet_y)) {
		curved_bullet->is_visible = false;
		change_alien_status(curved_bullet_x, curved_bullet_y);
		curved_bullet->y = 0;
		score += SCORE_PER_ALIEN;
		if (alive_aliens_count() == 0) {
			score += SCORE_PER_LEVEL;
		}
	}

	if (get_level() == FINAL_LEVEL) {
		if (curved_bullet_x == aggressive_alien_x_pos() &&
			curved_bullet_y == aggressive_alien_y_pos()) {
			curved_bullet->is_visible = false;
			change_alien_status(curved_bullet_x, curved_bullet_y);
			curved_bullet->y = 0;
			score += SCORE_PER_ALIEN;
			if (alive_aliens_count() == 0) {
				score += SCORE_PER_LEVEL;
			}
		}
	}
}

void reset_player() {
	int x_player = screen_width() / 2;
	int y_player = screen_height() - 6;

	alive = true;
	player->is_visible = true;
	player->x = x_player;
	player->y = y_player;
}

void reset_player_location() {
	int x_player = screen_width() / 2;
	int y_player = screen_height() - 6;

	player->x = x_player;
	player->y = y_player;
	
	while (is_alien(player->x, player->y)) {
		int random = rand() % screen_width();
		player->x = x_player;
		player->y = random;
	} 
}

int x_pos() {
	int x = (int) round(player->x);
	return x;
}

int y_pos() {
	int y = (int) round(player->y);
	return y;
}

int get_score() {
	return score;
}

void update_score(int points) {
	score += points; 
}

int get_lives() {
	return lives;
}

void update_lives() {
	lives--;
}

bool conquer() {
	if (alive_aliens_count() == 0) {
		return true;
	}
	return false;
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

void cleanup_player_bullet() {
	destroy_sprite(bullet);
}