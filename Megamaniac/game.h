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
void update_game();
void reset_game();
int get_level();
void level_up();
void setup_levelup_banner();
void draw_levelup_banner();
void display_levelup_banner();
void setup_gameover_banner();
void draw_gameover_banner();
void display_gameover_banner();
void cleanup_game();

#endif 
