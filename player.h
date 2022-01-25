#pragma once

#include<stdbool.h>
#include<SDL2/SDL.h>
#include "map.h"

#define PL_W (CELL_W/2)
#define PL_H (CELL_H/2)
#define PL_S .8
#define GRAV .00004
#define FRICT .09
typedef struct person {
  float x, y;
  float vx, vy;
} person;

bool in_map(int x, int y);

bool collide(tile MAP[], int x, int y);

bool collide_man(tile MAP[], int x, int y, int w, int h);

bool on_ground(person *player, tile MAP[]);

void manage_player(person *player, tile MAP[], int interval);

person read_player_pos(void);

void save_player_pos(person player);
