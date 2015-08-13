#ifndef __ALIENS__
#define __ALIENS__

#include <stdbool.h>
#include <math.h>
#include "cab202_graphics.h"
#include "cab202_sprites.h"
#include "cab202_timers.h"

void setup_aliens();
void create_aliens();
bool update_aliens();
void reset_aliens();
void draw_aliens();
void change_alien_status(int x, int y);

#endif 