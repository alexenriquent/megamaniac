#ifndef __PLAYER__
#define __PLAYER__

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
void reset_player();
int get_score();
int get_lives();
void update_lives();
bool is_alive();

#endif 
