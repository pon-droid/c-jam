#include <stdbool.h>
#include "cam.h"
#include "player.h"
#include "rend.h"
#include "bit.h"


bool quit (void){
  SDL_Event event;
  while(SDL_PollEvent(&event)){
    if(event.type == SDL_QUIT){
      return true;
    }
  }

  return false;
}

void mouse_edit(tile MAP[], camera cam){
  int x, y;
  Uint32 mouse = SDL_GetMouseState(&x, &y);
  //Translate screen position to array position
  x = (x + cam.x)/CELL_W;
  y = (y + cam.y)/CELL_H;
  
  if((mouse & SDL_BUTTON_LMASK) != 0 && in_map(x,y)){

    MAP[y * MAP_W + x].state = BINARY_ON;
  }
  if((mouse & SDL_BUTTON_RMASK) != 0 && in_map(x,y)){
    MAP[y * MAP_W + x].state = BINARY_OFF;
  }
  if((mouse & SDL_BUTTON_X2MASK) != 0 && in_map(x,y)){
    MAP[y * MAP_W + x].state = BLOCK;
  }
  if((mouse & SDL_BUTTON_X1MASK) != 0 && in_map(x,y)){
    MAP[y * MAP_W + x].state = BINARY_OFF;
  }

}


void print_friend(tile MAP[], camera cam){
  int x, y;
  SDL_GetMouseState(&x,&y);

  x = (x + cam.x)/CELL_W;
  y = (y + cam.y)/CELL_H;

  if(in_map(x,y)){
    printf("friend: %d\n",MAP[y * MAP_W + x].friend);
    printf("X: %d Y: %d\n",MAP[y * MAP_W + x].x, MAP[y * MAP_W + x].y);
    //printf("Bit: %d\n", MAP[y * MAP_W + x].state);
  }
}

int main(int argc, char* argv[]){
  SDL_Init(SDL_INIT_EVERYTHING);
  IMG_Init(IMG_INIT_PNG);

  
  SDL_Window* win = SDL_CreateWindow("bit newwise", 0, 0, SCR_W, SCR_H, SDL_WINDOW_SHOWN);
  SDL_Renderer* rend = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

  SDL_Texture *buffer [4];
  populate_textures(rend, buffer);
  tile MAP[MAP_W * MAP_H];
  read_map(MAP);
  
  person player = read_player_pos();
  camera cam;
  
  int last_time = 0;
  

  while(!quit()){
    int elapsed = SDL_GetTicks();
    set_cam(&cam, &player);

    SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
    SDL_RenderClear(rend);

    int interval = elapsed - last_time;
    draw_map(MAP,rend,cam, buffer);
    manage_player(&player,MAP,interval);
    draw_player(rend,player,cam, buffer);
    
    //mouse_edit(MAP,cam,player);
    
    control_bitwise(MAP);
    make_friends(MAP,cam);
    draw_friends(rend,MAP,cam, 0);
    
    SDL_RenderPresent(rend);

    last_time = elapsed;

  }

  
  cleanup_textures(buffer);
  SDL_DestroyRenderer(rend);
  SDL_DestroyWindow(win);
  save_map(MAP);
  save_player_pos(player);


  return 0;

}
