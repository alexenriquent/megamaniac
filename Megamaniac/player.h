#ifndef __PLAYER__
#define __PLAYER__

#include <stdlib.h>
#include <stdbool.h>
#include "cab202_graphics.h"
#include "cab202_sprites.h"
#include "cab202_timers.h"
#include "game.h"
#include "aliens.h"

void setup_player();
void draw_player();
bool update_player(int key);
void shoot_player_bullet();
void update_player_bullet();
void shoot_left_curved_bullet(double degree);
void update_left_curved_bullet();
void shoot_right_curved_bullet(double degree);
void update_right_curved_bullet();
void reset_player();
void reset_player_location();
int x_pos();
int y_pos();
int get_score();
int get_lives();
void update_lives();
bool conquer();
bool is_alive();
void check_player_alive();
void change_player_status();
void cleanup_player();
void cleanup_player_bullet();

#endif 
