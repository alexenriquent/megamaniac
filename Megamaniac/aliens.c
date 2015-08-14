#include "aliens.h"

#define ORIGIN 0
#define SPRITE_WIDTH 1
#define SPRITE_HEIGHT 1
#define ALIEN_COUNT 10
#define ALIEN_UPDATE_TIME 500
#define ALIEN_BULLET_UPDATE_TIME 3000
#define BULLET_COUNT 4
#define HARMONIC_STEP 10
#define HORIZONTAL_MOTION 100
#define DELAY 6000

typedef char *string;

sprite_id aliens[ALIEN_COUNT];
sprite_id bullets[BULLET_COUNT];
timer_id alien_timer;
timer_id alien_bullet_timer;
timer_id aggressive_motion_timer;
string alien_img = "@";
string invisible_alien_img = " ";
string alien_bullet_img = ".";
int steps;
int step_count;
sprite_id aggressive_alien;
int random_alien_number = -1;
int moving_steps;
bool parabolic_motion = false;
bool bounce = false;
int x_player;
int y_player;

void setup_aliens() {
	create_aliens();
	alien_timer = create_timer(ALIEN_UPDATE_TIME);
	alien_bullet_timer = create_timer(ALIEN_BULLET_UPDATE_TIME);

	aggressive_motion_timer = create_timer(DELAY);
}

void create_aliens() {
	for (int i = 0; i < ALIEN_COUNT; i++) {
		aliens[i] = create_sprite(ORIGIN, ORIGIN, 
					SPRITE_WIDTH, SPRITE_HEIGHT, alien_img);
	}

	if (get_level() == 5) {
		create_aggressive_alien();
	}

	create_alien_bullets();
	reset_aliens();
}

bool update_aliens() {
	if (!timer_expired(alien_timer)) {
		return false;
	}

	int level = get_level();

	if (level == 1) {
		for (int i = 0; i < ALIEN_COUNT; i++) {
			sprite_id alien = aliens[i];
			int x_next = (int) round(alien->x + alien->dx);

			if (x_next == screen_width()) {
				alien->x = 0;
			}
			alien->x += alien->dx;
			alien->y += alien->dy;
		}
	} else if (level == 2) {
		for (int i = 0; i < ALIEN_COUNT; i++) {
			sprite_id alien = aliens[i];
			int x_next = (int) round(alien->x + alien->dx);

			if (x_next == screen_width()) {
				alien->x = 0;
			}
			if (steps == 1) {
				alien->dy = 1;
			} else if (steps == 0 || steps == 5) {
				alien->dy = 0;
			} else if (steps == 6) {
				alien->dy = -1;
			}
			alien->x += alien->dx;
			alien->y += alien->dy;
		}
		steps = (steps + 1) % HARMONIC_STEP;
	} else if (level == 3) {
		for (int i = 0; i < ALIEN_COUNT; i++) {
			sprite_id alien = aliens[i];
			int x_next = (int) round(alien->x + alien->dx);
			int y_next = (int) round(alien->y + alien->dy);

			if (x_next == screen_width()) {
				alien->x = 0;
			}
			if (y_next == screen_height() * 80 / 100) {
				alien->y = 0;
			}
			alien->x += alien->dx;
			alien->y += alien->dy;
			alien_crash(alien);
		}
	} else if (level == 4) {
		for (int i = 0; i < ALIEN_COUNT; i++) {
			sprite_id alien = aliens[i];
			int x_next = (int) round(alien->x + alien->dx);
			int y_next = (int) round(alien->y + alien->dy);

			if (x_next >= screen_width()) {
				alien->x = 0;
			}
			if (x_next <= 0) {
				alien->x = screen_width() - 1;
			}
			if (y_next == screen_height() * 80 / 100) {
				alien->y = 0;
			}
			random_motion(alien);
			alien->x += alien->dx;
			if (get_screen_char(alien->x + alien->dx, alien->y) == '@' ||
				get_screen_char(alien->x + alien->dx + 1, alien->y) == '@' /*||
				get_screen_char(alien->x + alien->dx + 2, alien->y) == '@'*/) {
				alien->dx = -alien->dx;
				alien->x += alien->dx;
			}
			alien->y += alien->dy;
			alien_crash(alien);
		}
	} else if (level == 5) {
		for (int i = 0; i < ALIEN_COUNT; i++) {
			sprite_id alien = aliens[i];
			int x_next = (int) round(alien->x + alien->dx);
			int y_next = (int) round(alien->y + alien->dy);

			if (x_next == screen_width()) {
				alien->x = 0;
			}
			if (y_next == screen_height() * 80 / 100) {
				alien->y = 0;
			}
			alien->x += alien->dx;
			alien->y += alien->dy;
			alien_crash(alien);
		}

		if (parabolic_motion) {
			if (moving()) {
				move_alien();
			}

			if (!reach_edge()) {
				update_agressive_motion();
			} 

			if (reach_edge())  {
				aggressive_alien->is_visible = false;
				aliens[random_alien_number]->bitmap = alien_img;
				bounce = false;
				random_alien_number = -1;
				parabolic_motion = false;
			}
		}
	}

	update_alien_bullets();

	return true;
}

void reset_aliens() {
	int x_odd = screen_width() * 5 / 100;
	int y_odd = screen_height() * 5 / 100;
	int x_even = 0;
	int y_even = screen_height() * 15 / 100;
	int pattern_count = 0;
	int level = get_level();

	for (int i = 0; i < ALIEN_COUNT; i++) {
		if (pattern_count == 0) {
			aliens[i]->x = x_even;
			aliens[i]->y = y_even;
			aliens[i]->is_visible = true;
			x_even += screen_width() * 10 / 100;
		} else if (pattern_count == 1) {
			aliens[i]->x = x_odd;
			aliens[i]->y = y_odd;
			aliens[i]->is_visible = true;
			y_odd += screen_height() * 20 / 100;
		} else if (pattern_count == 2) {
			aliens[i]->x = x_odd;
			aliens[i]->y = y_odd;
			aliens[i]->is_visible = true;
			x_odd += screen_width() * 10 / 100;
			y_odd -= screen_height() * 20 / 100;
		}
		if (level == 1) {
			aliens[i]->dx = 1;
			aliens[i]->dy = 0;
		} else if (level == 2) {
			steps = 0;
			aliens[i]->dx = 1;
			aliens[i]->dy = 1;
		} else if (level == 3) {
			aliens[i]->dx = 1;
			aliens[i]->dy = 1;
		} else if (level == 4) {
			aliens[i]->dy = 1;
		} else if (level == 5) {
			aliens[i]->dx = 1;
			aliens[i]->dy = 1;
			aggressive_motion_timer = create_timer(DELAY);
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

	if (get_level() == 5 && aggressive_alien->x == x &&
		aggressive_alien->y == y) {
		aggressive_alien->is_visible = false;
		aliens[random_alien_number]->is_visible = false;
		parabolic_motion = false;
		bounce = false;
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
			reset_player_location();
		} else if (bullets[i]->y >= screen_height() * 80 / 100 - 1) {
			bullets[i]->is_visible = false;
			bullets[i]->x = ORIGIN;
			bullets[i]->y = ORIGIN;
		} 
		bullets[i]->y += bullets[i]->dy;
	}
}

void alien_crash(sprite_id alien) {
	if (get_screen_char(alien->x, alien->y) == '$') {
		change_alien_status(alien->x, alien->y);
		update_lives();
		reset_player_location();
	}
}

void random_motion(sprite_id alien) {
	int motion = rand() % HORIZONTAL_MOTION;

	if (motion % 2 == 0 || motion % 3 == 0 ||
		motion % 5 == 0) {
		alien->dx = 1;
	} else {
		alien->dx = -1;
	} 
}

bool aggressive_motion() {
	if (!timer_expired(aggressive_motion_timer) ||
		alive_aliens_count() == 0 ||
		parabolic_motion) {
		return false;
	}

	random_alien_number = get_random_alien();
	aggressive_alien->x = aliens[random_alien_number]->x;
	aggressive_alien->y = aliens[random_alien_number]->y;
	aggressive_alien->is_visible = true;
	aliens[random_alien_number]->bitmap = invisible_alien_img;

	moving_steps = rand() % (int)aggressive_alien->y;
	parabolic_motion = true;
	x_player = x_pos();
	y_player = y_pos();

	return true;
}

void create_aggressive_alien() {
	aggressive_alien = create_sprite(ORIGIN,ORIGIN, 
						SPRITE_WIDTH, SPRITE_HEIGHT, alien_img);
	aggressive_alien->is_visible = false;
}

void draw_aggressive_alien() {
	draw_sprite(aggressive_alien);
}

void move_alien() {
	aggressive_alien->x += -2;
	aggressive_alien->y += -2;
 	moving_steps = moving_steps - 1;

 	if (aggressive_alien->x > x_player) {
 		aggressive_alien->dx = -1;
 		aggressive_alien->dy = 1;
 	} else if (aggressive_alien->x < x_player) {
 		aggressive_alien->dx = 1;
 		aggressive_alien->dy = 1;
 	}
}

void update_agressive_motion() {
	if (bounce) { 
		aggressive_alien->x += aggressive_alien->dx;
		aggressive_alien->y -= aggressive_alien->dy;
	} else if (aggressive_alien->y == y_player) {
		aggressive_alien->x += aggressive_alien->dx;
		aggressive_alien->y += 0;
		step_count++;
		alien_crash(aggressive_alien);
		if (aggressive_alien->x == x_player ||
			step_count == 3) {
			step_count = 0;
			bounce = true;
		}
	} else {
		aggressive_alien->x += aggressive_alien->dx;
		aggressive_alien->y += aggressive_alien->dy;
		alien_crash(aggressive_alien);
	}
}

bool moving() {
	if (moving_steps != 0) {
		return true;
	} 
	return false;
}

bool reach_edge() {
	int y = aggressive_alien->y;
	if (y > 0) {
		return false;
	}
	return true;
}

void cleanup_aliens() {
	for (int i = 0; i < ALIEN_COUNT; i++) {
		destroy_sprite(aliens[i]);
	}
}