#include "game.h"

#define SPRITE_WIDTH 1
#define SPRITE_HEIGHT 1
#define UPDATE_SCREEN 1
#define NORMAL_MODE 0
#define GAME_OVER -1
#define RESET -2

typedef char *string;

string author = " Thanat Chokwijitkul (n9234900)";
string level_1 = "Level: 1 - Basic";
string score_str = "score: ";
string lives_str = "lives: ";
char line = '-';

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
	int result = NORMAL_MODE;
	int key = get_char();

	if (key == 'Q' || key == 'q') {
		return GAME_OVER;
	} else if ( key == 'R' || key == 'r') {
		return RESET;
	}
	if (!is_alive()) {
		return NORMAL_MODE;
	}
	if (update_player(key)) {
		result = UPDATE_SCREEN;
	} else if (update_aliens()) {
		result = UPDATE_SCREEN;
	} 
	if (shoot_alien_bullets()) {
		return UPDATE_SCREEN;
	} 
	
	return result;
}

void update_game() {
	clear_screen();
	draw_screen();	
	draw_player();
	draw_aliens();
	draw_alien_bullets();
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
