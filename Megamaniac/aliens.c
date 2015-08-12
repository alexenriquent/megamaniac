#include "aliens.h"

#define ORIGIN 0
#define SPRITE_WIDTH 1
#define SPRITE_HEIGHT 1
#define ALIEN_COUNT 10

typedef char *string;

sprite_id aliens[ALIEN_COUNT];
string alien_img = "@";
string dead_alien_img = " ";


void setup_aliens() {
	create_aliens();
}

void create_aliens() {
	for (int i = 0; i < ALIEN_COUNT; i++) {
		aliens[i] = create_sprite(ORIGIN, ORIGIN, 
					SPRITE_WIDTH, SPRITE_HEIGHT, alien_img);
	}
	reset_aliens();
}

void reset_aliens() {
	int x_odd = screen_width() * 5 / 100;
	int y_odd = screen_height() * 5 / 100;
	int x_even = 0;
	int y_even = screen_height() * 10 / 100;
	int pattern_count = 0;

	for (int i = 0; i < ALIEN_COUNT; i++) {
		if (pattern_count == 0) {
			aliens[i]->x = x_even;
			aliens[i]->y = y_even;
			x_even += screen_width() * 10 / 100;
		} else if (pattern_count == 1) {
			aliens[i]->x = x_odd;
			aliens[i]->y = y_odd;
			y_odd += screen_height() * 10 / 100;
		} else if (pattern_count == 2) {
			aliens[i]->x = x_odd;
			aliens[i]->y = y_odd;
			x_odd += screen_width() * 10 / 100;
			y_odd -= screen_height() * 10 / 100;
		}
		pattern_count = (pattern_count + 1) % 3;
	}
	draw_aliens();
}


void draw_aliens() {
	for (int i = 0; i < ALIEN_COUNT; i++) {
		draw_sprite(aliens[i]);
	}
}

void change_alien_status(int x, int y) {
	for (int i = 0; i < ALIEN_COUNT; i++) {
		if (aliens[i]->x == x && aliens[i]->y == y) {
			aliens[i]->is_visible = false;
		}
	}
}
