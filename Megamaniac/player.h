#ifndef __PLAYER__
#define __PLAYER__

#include <stdbool.h>
#include "cab202_graphics.h"
#include "cab202_sprites.h"
#include "cab202_timers.h"
#include "game.h"

void setup_player();
void draw_player();
bool update_player(int key);
void shoot_bullet();
void update_bullet(sprite_id shot);
int get_score();
int get_lives();
bool is_alive();
void reset_player();

#endif 
