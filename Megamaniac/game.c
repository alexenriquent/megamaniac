#include "game.h"

#define SPRITE_WIDTH 1
#define SPRITE_HEIGHT 1
#define UPDATE_SCREEN 1
#define NORMAL_MODE 0
#define GAME_OVER -1
#define RESET -2
#define INTERVAL 35
#define SCORE_PER_ALIEN 30
#define SCORE_PER_LEVEL 500

typedef char *string;

string author = " Thanat Chokwijitkul (n9234900)";
string level_1 = "Level: 1 - Basic";
string score_str = "score: ";
string lives_str = "lives: ";
char line = '-';
string bullet_img = ".";
bool shooting = false;

void setup_game() {
	setup_screen();
	draw_screen();
	setup_player();
	setup_aliens();
	draw_player();
	show_screen();
}

void draw_screen() {
	int width = screen_width();
	int height = screen_height();

	draw_line(0, height * 80 / 100, width - 1, height * 80 / 100, line);
	draw_string(0, height * 85 / 100, author);
	draw_string(width * 70 / 100, height * 85 / 100, score_str);
	draw_int(width * 70 / 100 + strlen(score_str), height * 85 / 100, get_score());
	draw_string(width * 85 / 100, height * 85 / 100, lives_str);
	draw_int(width * 85 / 100 + strlen(lives_str), height * 85 / 100, get_lives());
	draw_string((width / 2) - (strlen(level_1) / 2), height * 95 / 100, level_1);
}

int play_game() {
	int key = get_char();

	if (key == 'Q' || key == 'q') {
		return GAME_OVER;
	} else if ( key == 'R' || key == 'r') {
		return RESET;
	}
	if (key == 'S' || key == 's') {
		if (!shooting) {
			shoot_bullet();
			shooting = false;
		}
		return UPDATE_SCREEN;
	}
	if (!is_alive()) {
		return NORMAL_MODE;
	}
	if (update_player(key)) {
		return UPDATE_SCREEN;
	}
	
	return NORMAL_MODE;
}

void shoot_bullet() {
	int key = get_char();
	int x = x_pos();
	int y = y_pos();
	shooting = true;
	sprite_id bullet = create_sprite((double)x, (double)y, 
					SPRITE_WIDTH, SPRITE_HEIGHT, bullet_img);

	while (bullet->y > 0) {
		clear_screen();
		update_player(key);
		draw_screen();
		draw_player();
		draw_aliens();
		update_bullet(bullet);
		draw_sprite(bullet);
		show_screen();
		key = get_char();
		timer_pause(INTERVAL);
	}
}

void update_bullet(sprite_id shot) {
	shot->dy = -1;
	shot->y += shot->dy;

	if (get_screen_char(shot->x, shot->y) == '@') {
		shot->is_visible = false;
		change_alien_status(shot->x, shot->y);
		shot->y = 0;
		update_score(SCORE_PER_ALIEN);
	}

}

void update_game() {
	clear_screen();
	draw_screen();
	draw_player();
	draw_aliens();
	show_screen();
}

void reset_game() {
	clear_screen();
	reset_player();
	draw_screen();
	setup_player();
	draw_player();
	setup_aliens();
	draw_aliens();
	show_screen();
}

void cleanup_game() {
	cleanup_screen();
}
