#ifndef __PLAYER__
#define __PLAYER__

#include <stdbool.h>
#include "cab202_graphics.h"
#include "cab202_sprites.h"

void setup_player();
void draw_player();
bool update_player(int key);
void cleanup_player();
int get_score();
int get_lives();
bool is_alive();

#endif 
