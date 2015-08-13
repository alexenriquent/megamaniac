#include "aliens.h"

#define ORIGIN 0
#define SPRITE_WIDTH 1
#define SPRITE_HEIGHT 1
#define ALIEN_COUNT 10
#define ALIEN_UPDATE_TIME 500
#define ALIEN_BULLET_UPDATE_TIME 3000
#define BULLET_COUNT 4

typedef char *string;

sprite_id aliens[ALIEN_COUNT];
sprite_id bullets[BULLET_COUNT];
timer_id alien_timer;
timer_id alien_bullet_timer;
string alien_img = "@";
string dead_alien_img = " ";
string alien_bullet_img = ".";

void setup_aliens() {
	create_aliens();
	alien_timer = create_timer(ALIEN_UPDATE_TIME);
	alien_bullet_timer = create_timer(ALIEN_BULLET_UPDATE_TIME);
}

void create_aliens() {
	for (int i = 0; i < ALIEN_COUNT; i++) {
		aliens[i] = create_sprite(ORIGIN, ORIGIN, 
					SPRITE_WIDTH, SPRITE_HEIGHT, alien_img);
	}
	create_alien_bullets();
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

	update_alien_bullets();

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

int get_random_alien() {
	int random = rand() % 10;

	while (!alien_is_alive(random) &&
			alive_aliens_count() != 0) {
		random = rand() % 10;
	}
	return random;
}

int alive_aliens_count() {
	int count = 0;

	for (int i = 0; i < ALIEN_COUNT; i++) {
		if (alien_is_alive(i)) {
			count++;
		}
	}
	return count;
}

bool alien_is_alive(int alien_num) {
	if (aliens[alien_num]->is_visible == true) {
		return true;
	}
	return false;
}

void create_alien_bullets() {
	for (int i = 0; i < BULLET_COUNT; i++) {
		bullets[i] = create_sprite(ORIGIN, ORIGIN, 
					SPRITE_WIDTH, SPRITE_HEIGHT, alien_bullet_img);
		bullets[i]->dy = 1;
		bullets[i]->is_visible = false;
	}
}

void draw_alien_bullets() {
	for (int i = 0; i < BULLET_COUNT; i++) {
		draw_sprite(bullets[i]);
	}
}

bool shoot_alien_bullets() {
	if (!timer_expired(alien_bullet_timer) ||
		alive_aliens_count() == 0) {
		return false;
	}

	int random_alien = get_random_alien();

	for (int i = 0; i < BULLET_COUNT; i++) {
		if (bullets[i]->is_visible == false) {
			bullets[i]->x = aliens[random_alien]->x;
			bullets[i]->y = aliens[random_alien]->y;
			bullets[i]->is_visible = true;
			return true;
		}
	}

	return false;
}

void update_alien_bullets() {
	for (int i = 0; i < BULLET_COUNT; i++) {
		if (get_screen_char(bullets[i]->x, bullets[i]->y + 1) == '$') {
			bullets[i]->is_visible = false;
			bullets[i]->x = ORIGIN;
			bullets[i]->y = ORIGIN;
			update_lives();
		} else if (bullets[i]->y >= screen_height() * 80 / 100 - 1) {
			bullets[i]->is_visible = false;
			bullets[i]->x = ORIGIN;
			bullets[i]->y = ORIGIN;
		} 
		bullets[i]->y += bullets[i]->dy;
	}
}