#include <stdbool.h>
#include "cam.h"
#include "player.h"

bool quit (void){
  SDL_Event event;
  while(SDL_PollEvent(&event)){
    if(event.type == SDL_QUIT){
      return true;
    }
  }

  return false;
}

void mouse_edit(tile MAP[], camera cam, person player){
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

int main(void){
  SDL_Window* win = SDL_CreateWindow("bit newwise", 0, 0, SCR_W, SCR_H, SDL_WINDOW_RESIZABLE);
  SDL_Renderer* rend = SDL_CreateRenderer(win, -1, SDL_RENDERER_PRESENTVSYNC);

  tile MAP[MAP_W * MAP_H];
  diag(MAP);
  
  person player = { SCR_W/2, SCR_H/2, 0, 0};
  camera cam;
  
  int last_time = 0;
  while(!quit()){
    int elapsed = SDL_GetTicks();
    set_cam(&cam, &player);

    SDL_SetRenderDrawColor(rend, 255, 255, 255, 255);
    SDL_RenderClear(rend);

    int interval = elapsed - last_time;
    draw_map(MAP,rend,cam);
    manage_player(&player,MAP,cam,rend,interval);

    mouse_edit(MAP,cam,player);

    SDL_RenderPresent(rend);

    last_time = elapsed;

  }

  SDL_DestroyRenderer(rend);
  SDL_DestroyWindow(win);
  save_map(MAP);


  return 0;

}
