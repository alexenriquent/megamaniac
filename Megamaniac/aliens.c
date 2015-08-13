#include "aliens.h"

#define ORIGIN 0
#define SPRITE_WIDTH 1
#define SPRITE_HEIGHT 1
#define ALIEN_COUNT 10
#define ALIEN_UPDATE_TIME 150

typedef char *string;

sprite_id aliens[ALIEN_COUNT];
timer_id alien_timer;
string alien_img = "@";
string dead_alien_img = " ";

void setup_aliens() {
	create_aliens();
	alien_timer = create_timer(ALIEN_UPDATE_TIME);
}

void create_aliens() {
	for (int i = 0; i < ALIEN_COUNT; i++) {
		aliens[i] = create_sprite(ORIGIN, ORIGIN, 
					SPRITE_WIDTH, SPRITE_HEIGHT, alien_img);
	}
	reset_aliens();
}

bool update_aliens() {
	if (!timer_expired(alien_timer)) {
		return false;
	}

	for (int i = 0; i < ALIEN_COUNT; i++) {
		sprite_id alien = aliens[i];
		int x_next = (int) round(alien->x + alien->dx);

		if (x_next == screen_width()) {
			alien->x = 0;
		}

		alien->x += alien->dx;
	}

	return true;
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
		aliens[i]->dx = 1;
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
