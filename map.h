#pragma once

#include<stdlib.h>
#include<stdio.h>
#include<SDL2/SDL.h>
#include "cam.h"

#define MAP_W 50
#define MAP_H 50
#define CELL_W 64
#define CELL_H 64

#define GET_MAP(y, x) (MAP[y * MAP_W + x])

enum tile_state { BLOCK, BINARY_ON, BINARY_OFF };

typedef struct tile {
  int state;
} tile;

void save_map(tile MAP[]){
  FILE *out;

  out = fopen("map.dat", "w");

  if(out == NULL) { printf("Could not save map!\n"); }

  int i;
  for( i = 0; i < MAP_W * MAP_H; i++){
    fwrite(&MAP[i], sizeof(tile), 1, out);
  }

  fclose(out);

}

void diag(tile MAP[]){
  int i;
  
    for( i = 0; i < MAP_W * MAP_H; i++){
    tile tmp;
    if(i % 3 == 0){
      tmp.state = BINARY_ON;
    } else {
      tmp.state = BINARY_OFF;
    }
    MAP[i] = tmp;
  }
}

void read_map(tile MAP[]){
  FILE *in;

  in = fopen("map.dat","r");

  if(in == NULL) { printf("Could not read map!\n"); diag(MAP); fclose(in); return; }

  int i;
  for( i = 0; i < MAP_W * MAP_H; i++){
    fread(&MAP[i], sizeof(tile), 1, in);
  }

  fclose(in);

}

void draw_map(tile MAP[], SDL_Renderer *rend, camera cam){
  SDL_Rect rect;
  rect.h = CELL_H;
  rect.w = CELL_W;

  tile tmp;

  int y, x;

  for( y = 0; y < MAP_H; y++){
    for( x = 0; x < MAP_W; x++){
      tmp = GET_MAP(y, x);

      if(tmp.state == BLOCK){ SDL_SetRenderDrawColor(rend, 0, 0, 255, 255); }
      if(tmp.state == BINARY_ON){ SDL_SetRenderDrawColor(rend, 0, 255, 0, 255); }
      if(tmp.state == BINARY_OFF){ SDL_SetRenderDrawColor(rend, 255, 0, 0, 255); }
      rect.x = (x * CELL_W) - cam.x;
      rect.y = (y * CELL_H) - cam.y;
      
      SDL_RenderFillRect(rend, &rect);
    }
  }
}

bool in_map(int x, int y){
  return x < MAP_W && x >= 0 && y < MAP_H && y >= 0;
}

bool collide(tile MAP[], int x, int y){

  x = x/CELL_W;
  y = y/CELL_H;


  if(in_map(x,y)){
    if(MAP[y * MAP_W + x].state == BLOCK || MAP[y * MAP_W + x].state == BINARY_ON){ return false; }
  } else {
    return true;
  }

  return true;

}



