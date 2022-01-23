#pragma once
#include<stdbool.h>
#include "map.h"

#define PL_W (CELL_W/2)
#define PL_H (CELL_H/2)
#define PL_S .1
#define GRAV .4

typedef struct person {
  float x, y;
  float vx, vy;
} person;

bool collide_man(tile MAP[], int x, int y, int w, int h){ 
  return collide(MAP,x,y) || collide(MAP,x+w,y) || collide(MAP,x+w,y+h) || collide(MAP,x,y+h);
}

bool on_ground(person *player, tile MAP[]){
  //Add one, because when on ground we are not colliding, so check if block underneath is collidable
  return collide(MAP,player->x,player->y+PL_H + 4) || collide(MAP,player->x+PL_W,player->y+PL_H + 4);
}

void manage_player(person *player, tile MAP[], camera cam, SDL_Renderer *rend, int interval){
  SDL_SetRenderDrawColor(rend,0,0,0,255);
  SDL_Rect rect;
  rect.h = PL_H;
  rect.w = PL_W;

  const Uint8* keys = SDL_GetKeyboardState( NULL );
  //Dont predict next position
  if( keys[SDL_SCANCODE_RIGHT] ){
    player->vx = (PL_S * interval);
  }
  if( keys[SDL_SCANCODE_LEFT] ){
    player->vx = (PL_S * interval) * -1;
  }
  if( keys[SDL_SCANCODE_UP] ){
    player->vy = (PL_S * interval) * -1;
  }
  if( keys[SDL_SCANCODE_DOWN] ){
    player->vy = (PL_S * interval);
  }

  player->x += player->vx;
  player->y += player->vy;

  if(collide_man(MAP,player->x,player->y,rect.w,rect.h)) { player->x -= player->vx; player->y -= player->vy; }

  rect.x = player->x - cam.x;
  rect.y = player->y - cam.y;

  player->vx = 0;
  player->vy = 0;


  SDL_RenderFillRect(rend, &rect);
  /*
  if(collide_man(MAP,player->x,player->y)) {
    int tmp = player->y/CELL_H;
    int snap = player->y*CELL_H;
    player->y = snap;
    player->vy = 0;
    
  }  
  */
 
}

void set_cam(camera *cam, person *player){
  cam->x = player->x - SCR_W/2;
  cam->y = player->y - SCR_H/2;
}
