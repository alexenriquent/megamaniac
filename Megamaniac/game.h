#ifndef __GAME__
#define __GAME__

#include <string.h>
#include "cab202_graphics.h"
#include "cab202_sprites.h"
#include "cab202_timers.h"
#include "player.h"
#include "aliens.h"

void setup_game();
void draw_screen();
int play_game();
void shoot_bullet();
void update_bullet(sprite_id shot);
void update_game();
void reset_game();
void cleanup_game();

#endif 