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
void reset_player();
int get_score();
int get_lives();
void update_score(int points);
void update_lives();
bool is_alive();
int x_pos();
int y_pos();

#endif 
