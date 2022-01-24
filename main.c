#include <stdbool.h>
#include "cam.h"
#include "player.h"
#include "rend.h"


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



void make_friends(tile MAP[], camera cam){
  int x, y;
  Uint32 mouse = SDL_GetMouseState(&x,&y);

  x = (x + cam.x)/CELL_W;
  y = (y + cam.y)/CELL_H;
  
  if((mouse & SDL_BUTTON_LMASK) != 0 && in_map(x,y)){
    MAP[y * MAP_W + x].friend = WANT_FRIEND;
  }
  if((mouse & SDL_BUTTON_RMASK) != 0 && in_map(x,y)){
    int i;
    for( i = 0; i < MAP_W * MAP_H; i++){
      if(MAP[i].friend == WANT_FRIEND){
        MAP[y * MAP_W + x].friend = i;
        MAP[i].friend = y * MAP_W + x;
        return;
      }
    }
  }
}

void print_friend(tile MAP[], camera cam){
  int x, y;
  Uint32 mouse = SDL_GetMouseState(&x,&y);

  x = (x + cam.x)/CELL_W;
  y = (y + cam.y)/CELL_H;

  if(in_map(x,y)){
    printf("friend: %d\n",MAP[y * MAP_W + x].friend);
    printf("X: %d Y: %d\n",MAP[y * MAP_W + x].x, MAP[y * MAP_W + x].y);
    //printf("Bit: %d\n", MAP[y * MAP_W + x].state);
  }
}

void bitwise_NOT(tile MAP[]){
  printf("Work\n");
	int i;
	for( i = 0; i < MAP_W * MAP_H; i++){
		if(MAP[i].friend != NO_FRIEND && MAP[i].friend != WANT_FRIEND){
      int tmp = MAP[i].state;
      MAP[i].state = MAP[MAP[i].friend].state;
      MAP[MAP[i].friend].state = tmp;
      return;
		}
	}
}


void recursive_NOT(tile MAP[], int start){
  int i;
  for( i = start; i < MAP_W * MAP_H; i++){
    if(MAP[i].friend != NO_FRIEND && MAP[i].friend != WANT_FRIEND){
      int tmp = MAP[i].state;
      MAP[i].state = MAP[MAP[i].friend].state;
      MAP[MAP[i].friend].state = tmp;
      //Ensure we don't do a tangled swap, no cheating
      recursive_NOT(MAP,return_max(i,MAP[i].friend) + 1);
      return;
		}
  }
}

void reg_NOT(tile MAP[]){
  int i;
  for( i = 0; i < MAP_W * MAP_H; i++){
    if(MAP[i].state == BINARY_ON){
      MAP[i].state = BINARY_OFF;
      continue;
    }
    if(MAP[i].state == BINARY_OFF){
      MAP[i].state = BINARY_ON;
      continue;
    }
  }
}
void control_bitNOT(tile MAP[]){
	const Uint8* keys = SDL_GetKeyboardState( NULL );

	if(keys[SDL_SCANCODE_PAGEUP]){
		//reg_NOT(MAP);
    recursive_NOT(MAP, 0);
    SDL_Delay(200);
    return;
	}

}

int main(int argc, char* argv[]){
  SDL_Window* win = SDL_CreateWindow("bit newwise", 0, 0, SCR_W, SCR_H, SDL_WINDOW_SHOWN);
  SDL_Renderer* rend = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

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
    draw_map(MAP,rend,cam);
    manage_player(&player,MAP,interval);
    draw_player(rend,player,cam);
    mouse_edit(MAP,cam,player);
    print_friend(MAP,cam);
    control_bitNOT(MAP);
    make_friends(MAP,cam);
    draw_friends(rend,MAP,cam, 0);
    SDL_RenderPresent(rend);

    last_time = elapsed;

  }

  SDL_DestroyRenderer(rend);
  SDL_DestroyWindow(win);
  save_map(MAP);
  save_player_pos(player);


  return 0;

}
