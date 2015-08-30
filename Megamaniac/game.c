#include "game.h"

#define ORIGIN 0
#define SPRITE_WIDTH 1
#define SPRITE_HEIGHT 1
#define UPDATE_SCREEN 1
#define NORMAL_MODE 0
#define GAME_OVER -1
#define LEVEL_COUNT 5
#define FIRST_LEVEL 1
#define FINAL_LEVEL 5

typedef char *string;

string author = " Thanat Chokwijitkul (n9234900)";
string score_str = "score: ";
string lives_str = "lives: ";
sprite_id gameover_banner;
sprite_id levelup_banner;
sprite_id final_banner;
char line = '-';
int level = 3;

string levels[LEVEL_COUNT] = {
		"Level 1 - Basic Motion",
		"Level 2 - Harmonic Motion",
		"Level 3 - Falling Motion",
		"Level 4 - Drunken Motion",
		"Level 5 - Aggressive Motion"};

string gameover_banner_img = 
"+-----------------------------------------------+"
"|                  GAME OVER!                   |"
"|     Press 'q' to quit or 'r' to try again.    |"
"+-----------------------------------------------+";

string levelup_banner_img = 
"+-----------------------------------------------+"
"|                  LEVEL UP!                    |"
"|    Press 'l' to proceed to the next level.    |"
"|     Press 'q' to quit or 'r' to restart.      |"
"+-----------------------------------------------+";

string final_banner_img = 
"+-----------------------------------------------+"
"|      YOU HAVE COMPLETED THE FINAL LEVEL!      |"
"|          Press 'l' to keep playing.           |"
"|     Press 'q' to quit or 'r' to restart.      |"
"+-----------------------------------------------+";

void setup_game() {
	setup_screen();
	draw_screen();
	setup_player();
	setup_aliens();
	draw_player();
	draw_aliens();
	setup_levelup_banner();
	setup_final_banner();
	setup_gameover_banner();
	show_screen();
}

void draw_screen() {
	int width = screen_width();
	int height = screen_height();

	draw_line(0, height - 5, width - 1, height - 5, line);
	draw_string(0, height - 4, author);
	draw_string(width * 73 / 100, height - 4, score_str);
	draw_int(width * 73 / 100 + strlen(score_str), height - 4, get_score());
	draw_string(width * 88 / 100, height - 4, lives_str);
	draw_int(width * 88 / 100 + strlen(lives_str), height - 4, get_lives());
	draw_string((width / 2) - (strlen(levels[get_level() - 1]) / 2), 
				height - 2, levels[get_level() - 1]);
}

int play_game() {
	int result = NORMAL_MODE;
	int key = get_char();

	if (key == 'Q' || key == 'q') {
		return GAME_OVER;
	} else if (key == 'R' || key == 'r') {
		reset_game();
		return NORMAL_MODE;
	} else if ((key == 'L' || key == 'l') &&
				alive_aliens_count() == 0) {
		level_up();
		reset_player();
		reset_aliens();
		result = UPDATE_SCREEN;
	}
	if (conquer()) {
		if (level == FINAL_LEVEL) {
			display_final_banner();
		} else {
			display_levelup_banner();
		}
		return NORMAL_MODE;
	}
	if (!is_alive()) {
		display_gameover_banner();
		return NORMAL_MODE;
	}
	if (update_player(key)) {
		result = UPDATE_SCREEN;
	} else if (update_aliens()) {
		result = UPDATE_SCREEN;
	} 
	if (shoot_alien_bullets()) {
		result = UPDATE_SCREEN;
	}
	if (level == FINAL_LEVEL) {
		if (aggressive_motion()) {
		result = UPDATE_SCREEN;
		} 
	}

	check_player_alive();
	
	return result;
}

void update_game() {
	clear_screen();
	draw_screen();	
	draw_player();
	draw_aliens();
	draw_alien_bullets();

	if (level == FINAL_LEVEL) {
		draw_aggressive_alien();
	}

	show_screen();
}

void reset_game() {
	level = FIRST_LEVEL;

	clear_screen();
	draw_screen();
	setup_player();
	draw_player();
	setup_aliens();
	draw_aliens();
	show_screen();
}

int get_level() {
	return level;
}

void level_up() {
	if (level == FINAL_LEVEL) {
		level = FINAL_LEVEL;
	} else {
		level++;
	}
}

void setup_levelup_banner() {
	int width = screen_width();
	int height = screen_height();
	int banner_width = strlen(levelup_banner_img) / 5;
	int banner_height = 5;

	levelup_banner = create_sprite(ORIGIN, ORIGIN, banner_width, 
			 banner_height, levelup_banner_img);
	levelup_banner->x = (width - banner_width) / 2;
	levelup_banner->y = (height - banner_height) / 2;
	levelup_banner->is_visible = false;
}

void draw_levelup_banner() {
	draw_sprite(levelup_banner);
	levelup_banner->is_visible = true;
}

void display_levelup_banner() {
	change_player_status();
	clear_screen();
	draw_screen();	
	draw_player();
	draw_aliens();
	draw_levelup_banner();
}

void setup_final_banner() {
	int width = screen_width();
	int height = screen_height();
	int banner_width = strlen(final_banner_img) / 5;
	int banner_height = 5;

	final_banner = create_sprite(ORIGIN, ORIGIN, banner_width, 
			 banner_height, final_banner_img);
	final_banner->x = (width - banner_width) / 2;
	final_banner->y = (height - banner_height) / 2;
	final_banner->is_visible = false;
}

void draw_final_banner() {
	draw_sprite(final_banner);
	final_banner->is_visible = true;
}

void display_final_banner() {
	change_player_status();
	clear_screen();
	draw_screen();	
	draw_player();
	draw_aliens();
	draw_final_banner();
}

void setup_gameover_banner() {
	int width = screen_width();
	int height = screen_height();
	int banner_width = strlen(gameover_banner_img) / 4;
	int banner_height = 4;

	gameover_banner = create_sprite(ORIGIN, ORIGIN, banner_width, 
			 banner_height, gameover_banner_img);
	gameover_banner->x = (width - banner_width) / 2;
	gameover_banner->y = (height - banner_height) / 2;
	gameover_banner->is_visible = false;
}

void draw_gameover_banner() {
	draw_sprite(gameover_banner);
	gameover_banner->is_visible = true;
}

void display_gameover_banner() {
	change_player_status();
	clear_screen();
	draw_screen();	
	draw_player();
	draw_aliens();
	draw_gameover_banner();
}

void cleanup_game() {
	cleanup_player();
	cleanup_aliens();
	cleanup_alien_bullets();
	cleanup_player_bullet();
	cleanup_aggressive_alien();
	cleanup_screen();
}
