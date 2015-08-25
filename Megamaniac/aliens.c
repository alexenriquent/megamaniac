#include "aliens.h"

#define ORIGIN 0
#define SPRITE_WIDTH 1
#define SPRITE_HEIGHT 1
#define ALIEN_COUNT 10
#define ALIEN_UPDATE_TIME 500
#define ALIEN_BULLET_UPDATE_TIME 3000
#define BULLET_COUNT 4
#define HARMONIC_STEP 12
#define HORIZONTAL_MOTION 100
#define DELAY 6000
#define FINAL_LEVEL 5

typedef char *string;

sprite_id aliens[ALIEN_COUNT];
sprite_id bullets[BULLET_COUNT];
sprite_id aggressive_alien;
timer_id alien_timer;
timer_id alien_bullet_timer;
timer_id aggressive_motion_timer;
string alien_img = "@";
string invisible_alien_img = " ";
string alien_bullet_img = ".";
int steps;
int step_count;
int random_alien_number;
int moving_steps;
bool parabolic_motion = false;
bool bounce = false;
int x_player;
int y_player;

void setup_aliens() {
	create_aliens();
	create_aggressive_alien();
	alien_timer = create_timer(ALIEN_UPDATE_TIME);
	alien_bullet_timer = create_timer(ALIEN_BULLET_UPDATE_TIME);
	aggressive_motion_timer = create_timer(DELAY);
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

	int level = get_level();

	switch (level) {
		case 1:
			update_aliens_basic();
			break;
		case 2:
			update_aliens_harmonic();
			break;
		case 3:	
			update_aliens_falling();
			break;
		case 4:
			update_aliens_druken();
			break;
		case 5:
			update_aliens_aggressive();
			break;
	}

	update_alien_bullets();

	return true;
}

void update_aliens_basic() {
	for (int i = 0; i < ALIEN_COUNT; i++) {
		sprite_id alien = aliens[i];
		int x_next = (int) round(alien->x + alien->dx);

		if (x_next == screen_width()) {
			alien->x = 0;
		}
		alien->x += alien->dx;
		alien->y += alien->dy;
	}
}

void update_aliens_harmonic() {
	for (int i = 0; i < ALIEN_COUNT; i++) {
		sprite_id alien = aliens[i];
		int x_next = (int) round(alien->x + alien->dx);

		if (x_next == screen_width()) {
			alien->x = 0;
		}
		if (steps == 2) {
			alien->dy = 1;
		} else if (steps == 0 || steps == 1 || 
			steps == 6 || steps == 7) {
			alien->dy = 0;
		} else if (steps == 8) {
			alien->dy = -1;
		}
		alien->x += alien->dx;
		alien->y += alien->dy;
	}
	steps = (steps + 1) % HARMONIC_STEP;
}

void update_aliens_falling() {
	for (int i = 0; i < ALIEN_COUNT; i++) {
		sprite_id alien = aliens[i];
		int x_next = (int) round(alien->x + alien->dx);
		int y_next = (int) round(alien->y + alien->dy);

		if (x_next == screen_width()) {
			alien->x = 0;
		}
		if (y_next == screen_height() - 5) {
			alien->y = 0;
		}
		alien->x += alien->dx;
		alien->y += alien->dy;
		alien_crash(alien);
	}
}

void update_aliens_druken() {
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
		if (y_next == screen_height() - 5) {
			alien->y = 0;
		}
		// random_motion(alien);
		alien->x += alien->dx;
		if (get_screen_char(alien->x + alien->dx, alien->y) == '@' ||
			get_screen_char(alien->x + alien->dx + 1, alien->y) == '@') {
			alien->dx = -alien->dx;
			alien->x += alien->dx;
		}
		alien->y += alien->dy;
		alien_crash(alien);
	}
}

void update_aliens_aggressive() {
	for (int i = 0; i < ALIEN_COUNT; i++) {
		sprite_id alien = aliens[i];
		int x_next = (int) round(alien->x + alien->dx);
		int y_next = (int) round(alien->y + alien->dy);

		if (x_next == screen_width()) {
			alien->x = 0;
		}
		if (y_next == screen_height() - 5) {
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
			if (collide()) {
				aliens[random_alien_number]->is_visible = false;
			}
		} 

		if (reach_edge())  {
			aggressive_alien->is_visible = false;
			aggressive_alien->x = ORIGIN;
			aggressive_alien->y = ORIGIN;
			aliens[random_alien_number]->is_visible = true;
			bounce = false;
			parabolic_motion = false;
		}
	}
}

void reset_aliens() {
	int x_odd = screen_width() * 5 / 100;
	int y_odd = screen_height() * 5 / 100;
	int x_even = 0;
	int y_even = screen_height() * 15 / 100;
	int pattern_count = 0;
	int current_level = get_level();

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
		reset_levels(aliens[i], current_level);
		pattern_count = (pattern_count + 1) % 3;
	}

	create_aggressive_alien();
	aggressive_motion_timer = create_timer(DELAY);

	if (get_level() == FINAL_LEVEL) {
		// create_aggressive_alien();
		draw_aggressive_alien();
		// aggressive_motion_timer = create_timer(DELAY);
	}

	draw_aliens();
	reset_aliens_bullets();
}

void reset_levels(sprite_id alien, int level) {
	switch (level) {
		case 1:
			alien->dx = 1;
			alien->dy = 0;
			break;
		case 2:
			steps = 0;
			alien->dx = 1;
			alien->dy = 1;
			break;
		case 3:
			alien->dx = 1;
			alien->dy = 1;
			break;
		case 4:
			alien->dy = 1;
			break;
		case 5:
			alien->dx = 1;
			alien->dy = 1;
			break;
	}
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

	if (get_level() == FINAL_LEVEL && aggressive_alien->x == x &&
		aggressive_alien->y == y) {
		aggressive_alien->x = ORIGIN;
		aggressive_alien->y = ORIGIN;
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
		if (aliens[i]->bitmap == invisible_alien_img) {
			aliens[i]->bitmap = alien_img;
			aliens[i]->is_visible = false;
		}
	}

	for (int i = 0; i < ALIEN_COUNT; i++) {
		if (alien_is_alive(i)) {
			count++;
		}
	}

	if (get_level() == FINAL_LEVEL) {
		if (parabolic_motion && count == 0) {
			count = 1;
			aggressive_alien->is_visible = false;
			aggressive_alien->x = ORIGIN;
			aggressive_alien->y = ORIGIN;
			aliens[random_alien_number]->is_visible = true;
			bounce = false;
			parabolic_motion = false;
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
			bullets[i]->y = aliens[random_alien]->y + 1;
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
		} else if (bullets[i]->y >= screen_height() - 6) {
			bullets[i]->is_visible = false;
			bullets[i]->x = ORIGIN;
			bullets[i]->y = ORIGIN;
		} 
		bullets[i]->y += bullets[i]->dy;
	}
}

void reset_aliens_bullets() {
	for (int i = 0; i < BULLET_COUNT; i++) {
		bullets[i]->is_visible = false;
		bullets[i]->x = ORIGIN;
		bullets[i]->y = ORIGIN;
	}
}

void alien_crash(sprite_id alien) {
	bool alive = alien->is_visible;
	if (alive) {
		if (get_screen_char(alien->x, alien->y) == '$') {
			change_alien_status(alien->x, alien->y);
			update_lives();
			reset_player_location();
		}
	}
}

void random_motion(sprite_id alien) {
	int motion = rand() % HORIZONTAL_MOTION;

	if (motion % 2 == 0 || motion % 3 == 0 ||
		motion % 5 == 0) {
		alien->dx = 2;
	} else {
		alien->dx = -2;
	} 
}

bool aggressive_motion() {
	if (!timer_expired(aggressive_motion_timer) ||
		alive_aliens_count() <= 1 ||
		parabolic_motion) {
		return false;
	}

	random_alien_number = get_random_alien();
	while (random_alien_number == 7) {
		random_alien_number = get_random_alien();
	}
	aggressive_alien->x = aliens[random_alien_number]->x;
	aggressive_alien->y = aliens[random_alien_number]->y;
	aggressive_alien->is_visible = true;
	aliens[random_alien_number]->is_visible = false;
	parabolic_motion = true;
	x_player = x_pos();
	y_player = y_pos();

	int x_distance = abs(x_player - aggressive_alien->x);
	int y_distance = y_player - aggressive_alien->y;

	while (x_distance != y_distance) {
		if (x_distance > y_distance) {
			x_distance--;
		} else if (x_distance < y_distance) {
			x_distance++;
		}
		moving_steps++;
	}

	moving_steps = moving_steps / 2;

	return true;
}

void create_aggressive_alien() {
	aggressive_alien = create_sprite(ORIGIN, ORIGIN, 
						SPRITE_WIDTH, SPRITE_HEIGHT, alien_img);
	aggressive_alien->is_visible = false;
}

void draw_aggressive_alien() {
	draw_sprite(aggressive_alien);
}


int aggressive_alien_x_pos() {
	int x = aggressive_alien->x;
	return x;
}

int aggressive_alien_y_pos() {
	int y = aggressive_alien->y;
	return y;
}

void move_alien() {
 	if (aggressive_alien->x > x_player) {
 		aggressive_alien->x += -1;
 		aggressive_alien->y += -1;
 		aggressive_alien->dx = -1;
 		aggressive_alien->dy = 1;
 	} else if (aggressive_alien->x < x_player) {
 		aggressive_alien->x += 1;
 		aggressive_alien->y += -1;
 		aggressive_alien->dx = 1;
 		aggressive_alien->dy = 1;
 	} else {
 		aggressive_alien->y += -1;
 		aggressive_alien->dx = 1;
 		aggressive_alien->dy = 1;
 	}

 	moving_steps--;
}

void update_agressive_motion() {
	if (bounce) { 
		if (get_screen_char(aggressive_alien->x - aggressive_alien->dx, 
			aggressive_alien->y + aggressive_alien->dy) == '@' ||
			get_screen_char(aggressive_alien->x + aggressive_alien->dx, 
			aggressive_alien->y + aggressive_alien->dy) == '@' ||
			get_screen_char(aggressive_alien->x + aggressive_alien->dx + aggressive_alien->dx, 
			aggressive_alien->y + aggressive_alien->dy + aggressive_alien->dy) == '@' ||
			get_screen_char(aggressive_alien->x, 
			aggressive_alien->y + aggressive_alien->dy) == '@' ||
			get_screen_char(aggressive_alien->x, 
			aggressive_alien->y - 1) == '@') {
			aggressive_alien->dx = -aggressive_alien->dx;
			aggressive_alien->x += aggressive_alien->dx;
			aggressive_alien->y -= aggressive_alien->dy;
			aggressive_alien->dx = -aggressive_alien->dx;
		} else {
			aggressive_alien->x += aggressive_alien->dx;
			aggressive_alien->y -= aggressive_alien->dy;
		}
	} else if (aggressive_alien->y == y_player) {
		aggressive_alien->x += aggressive_alien->dx;
		aggressive_alien->y += 0;
		step_count++;
		aggressive_alien_crash();
		if (step_count == 3) {
			step_count = 0;
			bounce = true;
		}
	} else {
		if (aggressive_alien->x == x_player) {
			aggressive_alien->y += aggressive_alien->dy;
			aggressive_alien_crash();
		} else {
			aggressive_alien->x += aggressive_alien->dx;
			aggressive_alien->y += aggressive_alien->dy;
			aggressive_alien_crash();
		}
	}
}

void aggressive_alien_crash() {
	if (collide()) {
		aggressive_alien->x = ORIGIN;
		aggressive_alien->y = ORIGIN;
		aggressive_alien->is_visible = false;
		aliens[random_alien_number]->bitmap = invisible_alien_img;
		bounce = false;
		parabolic_motion = false;
		update_lives();
		reset_player_location();
	}	
}

bool collide() {
	if (get_screen_char(aggressive_alien->x, 
		aggressive_alien->y) == '$') {
		return true;
	}
	return false;
}

bool moving() {
	if (moving_steps != 0) {
		return true;
	} 
	return false;
}

bool reach_edge() {
	int x = aggressive_alien->x;
	int y = aggressive_alien->y;
	if (y > 0 &&
		(x > 0 && x < screen_width())) {
		return false;
	}
	return true;
}

void cleanup_aliens() {
	for (int i = 0; i < ALIEN_COUNT; i++) {
		destroy_sprite(aliens[i]);
	}
}

void cleanup_alien_bullets() {
	for (int i = 0; i < BULLET_COUNT; i++) {
		destroy_sprite(bullets[i]);
	}
}

void cleanup_aggressive_alien() {
	destroy_sprite(aggressive_alien);
}
