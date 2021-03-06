#ifndef __ALIENS__
#define __ALIENS__

#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "cab202_graphics.h"
#include "cab202_sprites.h"
#include "cab202_timers.h"
#include "game.h"
#include "player.h"

void setup_aliens();
void create_aliens();
bool update_aliens();
void update_aliens_basic();
void update_aliens_harmonic();
void update_aliens_falling();
void update_aliens_druken();
void update_aliens_aggressive();
void reset_aliens();
void reset_levels(sprite_id alien, int level);
void draw_aliens();
void change_alien_status(int x, int y);
bool is_alien(int x, int y);
int get_random_alien();
int alive_aliens_count();
bool alien_is_alive(int alien_num);

void create_alien_bullets();
void draw_alien_bullets();
bool shoot_alien_bullets();
void update_alien_bullets();
void reset_aliens_bullets();

void alien_crash(sprite_id alien);
void random_motion(sprite_id alien);
void create_aggressive_alien();
void draw_aggressive_alien();
int aggressive_alien_x_pos();
int aggressive_alien_y_pos();
bool aggressive_motion();
void move_alien();
void update_agressive_motion();
void aggressive_alien_crash();
bool collide();
bool moving();
bool reach_edge();

void cleanup_aliens();
void cleanup_alien_bullets();
void cleanup_aggressive_alien();

#endif 
