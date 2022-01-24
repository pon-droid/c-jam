#pragma once

#include<stdlib.h>
#include<stdio.h>

#define MAP_W 50
#define MAP_H 50
#define CELL_W 64
#define CELL_H 64

#define GET_MAP(y, x) (MAP[y * MAP_W + x])

enum tile_state { BLOCK, BINARY_ON, BINARY_OFF };
enum friend_state { NO_FRIEND = -1, WANT_FRIEND = -2, HAS_FRIEND = -3 };

typedef struct tile {
  int friend;
  int state;
} tile;

void save_map(tile MAP[]);

void diag(tile MAP[]);

void read_map(tile MAP[]);





