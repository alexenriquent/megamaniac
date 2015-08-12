#include "game.h"
#include "player.h"

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

void cleanup_game() {
	cleanup_screen();
}